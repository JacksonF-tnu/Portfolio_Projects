#include "Blackjack.h"

Blackjack::Blackjack()
{
    playerMoney = 1000;
    roundOver = false;
}

void Blackjack::startNewRound(int betAmount)
{
    deck = Deck();
    deck.Shuffle();

    dealerHand.clear();
    playerHands.clear();
    bets.clear();
    currentHandIndex = 0;
    roundOver = false;

    dealInitialHands(betAmount);
}

void Blackjack::dealInitialHands(int betAmount)
{
    playerHands.clear();
    bets.clear();

    playerMoney -= betAmount;

    Hand h;
    h.Hit(deck.Deal());
    h.Hit(deck.Deal());

    playerHands.push_back(h);
    bets.push_back(betAmount);

    dealerHand.clear();
    dealerHand.Hit(deck.Deal());
    dealerHand.Hit(deck.Deal());

    currentHandIndex = 0;
}

bool Blackjack::playerCanSplit() const
{
    if (playerHands.size() > 1) return false;

    const Hand& h = playerHands[0];
    if (h.cards.size() != 2) return false;

    int v1 = h.cards[0].GetValue().toInt();
    int v2 = h.cards[1].GetValue().toInt();

    return v1 == v2;
}

bool Blackjack::isSplitAceHand(int index) const
{
    const Hand& h = playerHands[index];
    if (h.cards.size() != 2) return false;
    return h.cards[0].GetValue().toInt() == 1;
}

void Blackjack::playerSplit()
{
    if (!playerCanSplit()) return;

    Hand h1, h2;
    h1.Hit(playerHands[0].cards[0]);
    h2.Hit(playerHands[0].cards[1]);

    int baseBet = bets[0];
    playerMoney -= baseBet;

    playerHands.clear();
    bets.clear();

    h1.Hit(deck.Deal());
    h2.Hit(deck.Deal());

    playerHands.push_back(h1);
    playerHands.push_back(h2);

    bets.push_back(baseBet);
    bets.push_back(baseBet);


    if (isSplitAceHand(0)) currentHandIndex = 1;
    if (isSplitAceHand(1)) currentHandIndex = 2;
}

void Blackjack::playerHit()
{
    if (playerHands.empty() || currentHandIndex >= (int)playerHands.size())
        return;

    Hand& h = playerHands[currentHandIndex];
    h.Hit(deck.Deal());

    if (roundOver) return;

    if (h.isBusted() || isSplitAceHand(currentHandIndex))
    {
        currentHandIndex++;
        if (currentHandIndex >= (int)playerHands.size())
        {
            dealerPlay();
            resolveBets();
        }
    }
}

void Blackjack::playerStand()
{
    currentHandIndex++;
    if (currentHandIndex >= (int)playerHands.size())
    {
        dealerPlay();
        resolveBets();
    }
}

void Blackjack::playerDouble()
{
    Hand& h = playerHands[currentHandIndex];
    playerMoney -= bets[currentHandIndex];
    bets[currentHandIndex] *= 2;

    h.Hit(deck.Deal());

    currentHandIndex++;
    if (currentHandIndex >= (int)playerHands.size())
    {
        dealerPlay();
        resolveBets();
    }
}

void Blackjack::dealerPlay()
{
    while (dealerHand.getValue() < 17)
        dealerHand.Hit(deck.Deal());
}

void Blackjack::resolveBets()
{
    roundOver = true;
    int dealerVal = dealerHand.getValue();

    for (int i = 0; i < (int)playerHands.size(); i++)
    {
        int p = playerHands[i].getValue();
        int b = bets[i];

        if (p > 21) continue;

        if (dealerVal > 21 || p > dealerVal)
            playerMoney += b * 2;
        else if (p == dealerVal)
            playerMoney += b; // push
    }
}

bool Blackjack::isRoundOver() const { return roundOver; }

QString Blackjack::resultString() const
{
    QString out = "Round Results:\n";

    int dealerVal = dealerHand.getValue();
    out += "Dealer: " + QString::number(dealerVal) + "\n\n";

    for (int i = 0; i < (int)playerHands.size(); i++)
    {
        int p = playerHands[i].getValue();
        out += "Hand " + QString::number(i + 1) +
               " (" + QString::number(p) + "): ";

        if (p > 21) out += "Bust (Lose)";
        else if (dealerVal > 21) out += "Dealer Busts (Win)";
        else if (p > dealerVal) out += "Win";
        else if (p < dealerVal) out += "Lose";
        else out += "Push";

        out += "\n";
    }

    out += "\nBankroll: $" + QString::number(playerMoney);
    return out;
}

// Accessors
const std::vector<Hand>& Blackjack::getPlayerHands() const { return playerHands; }
const Hand& Blackjack::getDealerHand() const { return dealerHand; }
int Blackjack::getCurrentHandIndex() const { return currentHandIndex; }
int Blackjack::getPlayerMoney() const { return playerMoney; }
int Blackjack::getBetForHand(int index) const { return bets[index]; }
