# ---------------------------------------------
# IMPORTS
# ---------------------------------------------

import argparse
import os
import sys
import time
from datetime import datetime, timezone
from typing import Dict, Optional, Tuple, List

import pandas as pd
import requests


# ---------------------------------------------
# HELPER FUNCTION: GET CURRENT UTC TIME
# ---------------------------------------------
def utc_now_iso() -> str:
    """
    Returns the current time in UTC in ISO format.

    Example:
    2026-03-12T14:30:00.123456+00:00
    """
    return datetime.now(timezone.utc).isoformat()


# ---------------------------------------------
# HELPER FUNCTION: CLEAN / NORMALIZE EMAILS
# ---------------------------------------------
def norm_email(x) -> str:
    """
    Normalizes an email value so comparisons are reliable.

    - Blank/NaN becomes ""
    - Removes spaces around the value
    - Converts to lowercase
    """
    if pd.isna(x):
        return ""
    return str(x).strip().lower()


# ---------------------------------------------
# HELPER FUNCTION: GET VALUE FROM ARG OR ENV
# ---------------------------------------------
def require_env_or_arg(val: Optional[str], env_name: str) -> str:
    """
    Returns a value from either:
    1. a command-line argument
    2. an environment variable

    Raises an error if neither exists.
    """
    if val:
        return val

    env = os.getenv(env_name)
    if env:
        return env

    raise ValueError(f"Missing required value for {env_name} (arg or env var).")


# ---------------------------------------------
# CLASS: 3CX API CLIENT
# ---------------------------------------------
class ThreeCXClient:
    """
    Handles communication with the 3CX API.

    This class knows how to:
    - log into 3CX
    - test the API connection
    - download the 3CX user list
    """

    def __init__(self, pbx_base_url: str, client_id: str, client_secret: str, timeout_s: int = 30):
        self.pbx = pbx_base_url.rstrip("/")
        self.client_id = client_id
        self.client_secret = client_secret
        self.timeout_s = timeout_s
        self._token: Optional[str] = None
        self._token_obtained_at: float = 0.0

    def get_token(self) -> str:
        """
        Gets an access token from 3CX.

        Reuses the current token if it is still fresh.
        """
        if self._token and (time.time() - self._token_obtained_at) < 55 * 60:
            return self._token

        url = f"{self.pbx}/connect/token"
        data = {
            "client_id": self.client_id,
            "client_secret": self.client_secret,
            "grant_type": "client_credentials",
        }

        r = requests.post(url, data=data, timeout=self.timeout_s)
        r.raise_for_status()

        js = r.json()
        token = js.get("access_token")
        if not token:
            raise RuntimeError(f"Token response missing access_token: {js}")

        self._token = token
        self._token_obtained_at = time.time()
        return token

    def _headers(self) -> Dict[str, str]:
        """
        Builds the Authorization header for API requests.
        """
        return {"Authorization": f"Bearer {self.get_token()}"}

    def test_defs(self) -> None:
        """
        Sends a simple test request to make sure the API is reachable
        and the credentials work.
        """
        url = f"{self.pbx}/xapi/v1/Defs"
        params = {"$select": "Id"}

        r = requests.get(url, headers=self._headers(), params=params, timeout=self.timeout_s)
        r.raise_for_status()

    def list_users_df(self) -> pd.DataFrame:
        """
        Downloads 3CX users and returns them as a pandas DataFrame.

        Expected columns:
        - Id
        - Number
        - Email
        - DID
        """
        url = f"{self.pbx}/xapi/v1/Users"
        params = {"$select": "Id,Number,Email,DID"}

        r = requests.get(url, headers=self._headers(), params=params, timeout=self.timeout_s)
        r.raise_for_status()

        rows = r.json().get("value", [])
        df = pd.DataFrame(rows)

        # Ensure required columns exist
        for col in ["Id", "Number", "Email", "DID"]:
            if col not in df.columns:
                df[col] = pd.NA

        df["Email_norm"] = df["Email"].apply(norm_email)
        return df


# ---------------------------------------------
# FUNCTION: LOAD MICROSOFT CSV
# ---------------------------------------------
def load_ms_csv_df(csv_path: str, email_column_candidates: List[str]) -> Tuple[pd.DataFrame, str]:
    """
    Loads the Microsoft CSV into a pandas DataFrame.

    Since Microsoft exports can use different names for the email column,
    we try several possible column names.

    Returns:
    - DataFrame
    - the actual email column used
    """
    df = pd.read_csv(csv_path, dtype="string", encoding="utf-8-sig")

    if df.empty:
        raise ValueError("Microsoft CSV appears to be empty.")

    lower_map = {c.lower(): c for c in df.columns}
    chosen = None

    for cand in email_column_candidates:
        if cand.lower() in lower_map:
            chosen = lower_map[cand.lower()]
            break

    if not chosen:
        raise ValueError(
            f"Could not find an email/UPN column. Found columns: {list(df.columns)}. "
            f"Tried: {email_column_candidates}"
        )

    df["Email_norm"] = df[chosen].apply(norm_email)
    df = df[df["Email_norm"] != ""].copy()

    return df, chosen


# ---------------------------------------------
# FUNCTION: LOAD FAKE 3CX CSV FOR OFFLINE TESTING
# ---------------------------------------------
def load_fake_3cx_csv_df(csv_path: str) -> pd.DataFrame:
    """
    Loads a local CSV file that imitates the 3CX user list.

    This is used for offline testing so the script can run without
    contacting the real 3CX server.

    Expected columns in the fake CSV:
    - Id
    - Number
    - Email
    - DID

    If any are missing, they will be added as blank columns so the rest
    of the script still works.
    """
    df = pd.read_csv(csv_path, dtype="string", encoding="utf-8-sig")

    if df.empty:
        raise ValueError("Fake 3CX CSV appears to be empty.")

    for col in ["Id", "Number", "Email", "DID"]:
        if col not in df.columns:
            df[col] = pd.NA

    df["Email_norm"] = df["Email"].apply(norm_email)
    return df


# ---------------------------------------------
# MAIN PROGRAM
# ---------------------------------------------
def main() -> int:
    """
    Main script flow:

    1. Read command-line arguments
    2. Load Microsoft CSV
    3. Load either:
       - fake 3CX CSV (offline mode), or
       - real 3CX API data (live mode)
    4. Compare both lists by normalized email
    5. Save overlap results to CSV
    """
    ap = argparse.ArgumentParser(
        description="Find users that appear in both MS CSV and 3CX users."
    )

    # Real 3CX connection options
    ap.add_argument("--pbx", help="PBX base URL e.g. https://example.my3cx.com")
    ap.add_argument("--client-id", help="3CX API client_id")
    ap.add_argument("--client-secret", help="3CX API client_secret")

    # Microsoft CSV input
    ap.add_argument("--ms-csv", required=True, help="Path to Microsoft CSV")

    # Offline testing option
    ap.add_argument(
        "--fake-3cx-csv",
        help="Path to local CSV file to use instead of contacting the 3CX API"
    )

    # Output file
    ap.add_argument("--out", default="ms_3cx_overlap.csv", help="Output CSV of overlapping entries")

    args = ap.parse_args()

    run_id = str(int(time.time()))
    ts = utc_now_iso()

    # -----------------------------------------
    # STEP 1: LOAD MICROSOFT CSV
    # -----------------------------------------
    ms_df, ms_col = load_ms_csv_df(
        args.ms_csv,
        email_column_candidates=[
            "User principal name",
            "UPN",
            "Email",
            "Email address",
            "UserPrincipalName",
        ],
    )

    # Keep only the normalized email column and remove duplicates
    ms_small = ms_df[["Email_norm"]].drop_duplicates()

    # -----------------------------------------
    # STEP 2: LOAD 3CX USERS
    # -----------------------------------------
    if args.fake_3cx_csv:
        # OFFLINE MODE:
        # Use a local CSV instead of contacting 3CX
        cx_df = load_fake_3cx_csv_df(args.fake_3cx_csv)
        pbx_label = "OFFLINE_TEST_MODE"
    else:
        # LIVE MODE:
        # Connect to real 3CX API
        pbx = require_env_or_arg(args.pbx, "PBX_URL")
        client_id = require_env_or_arg(args.client_id, "THREECX_CLIENT_ID")
        client_secret = require_env_or_arg(args.client_secret, "THREECX_CLIENT_SECRET")

        cx = ThreeCXClient(pbx, client_id, client_secret)
        cx.test_defs()
        cx_df = cx.list_users_df()
        pbx_label = pbx

    # Keep only the columns we care about
    cx_small = cx_df[["Id", "Number", "Email", "DID", "Email_norm"]].copy()

    # -----------------------------------------
    # STEP 3: FIND OVERLAP
    # -----------------------------------------
    # Inner merge = keep only rows where Email_norm exists in both tables
    overlap = ms_small.merge(cx_small, on="Email_norm", how="inner")

    # -----------------------------------------
    # STEP 4: ADD METADATA
    # -----------------------------------------
    overlap.insert(0, "timestamp_utc", ts)
    overlap.insert(1, "run_id", run_id)
    overlap.insert(2, "pbx", pbx_label)
    overlap.insert(3, "ms_email_column_used", ms_col)

    # -----------------------------------------
    # STEP 5: SAVE RESULTS
    # -----------------------------------------
    overlap.to_csv(args.out, index=False)

    # -----------------------------------------
    # STEP 6: PRINT SUMMARY
    # -----------------------------------------
    print(f"MS rows: {len(ms_small)}")
    print(f"3CX rows: {len(cx_small)}")
    print(f"Overlap rows: {len(overlap)}")
    print(f"Output written to: {args.out}")

    if args.fake_3cx_csv:
        print("Mode used: OFFLINE TEST MODE")
        print(f"Fake 3CX CSV used: {args.fake_3cx_csv}")
    else:
        print("Mode used: LIVE 3CX API MODE")

    return 0


# ---------------------------------------------
# SCRIPT ENTRY POINT
# ---------------------------------------------
if __name__ == "__main__":
    try:
        raise SystemExit(main())

    except requests.HTTPError as e:
        resp = getattr(e, "response", None)
        if resp is not None:
            print(f"HTTP {resp.status_code}: {resp.text}", file=sys.stderr)
        raise
