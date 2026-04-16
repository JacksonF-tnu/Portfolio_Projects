#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "Deck.h"
#include "Hand.h"
#include <vector>
#include <QString>

class Blackjack
{
public:
    Blackjack();


    void startNewRound(int betAmount);


    void playerHit();
    void playerStand();
    void playerDouble();
    bool playerCanSplit() const;
    void playerSplit();


    bool isRoundOver() const;
    QString resultString() const;


    const vector<Hand>& getPlayerHands() const;
    const Hand& getDealerHand() const;
    int getCurrentHandIndex() const;
    int getPlayerMoney() const;
    int getBetForHand(int index) const;

private:
    Deck deck;
    std::vector<Hand> playerHands;
    std::vector<int> bets;
    Hand dealerHand;

    int currentHandIndex;
    bool roundOver;
    int playerMoney;

    void dealInitialHands(int betAmount);
    void dealerPlay();
    void resolveBets();
    bool isSplitAceHand(int index) const;
};

#endif
