#ifndef CARDDECKMAINWINDOW_H
#define CARDDECKMAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <vector>

#include "blackjack.h"

class CardDeckMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CardDeckMainWindow(QWidget *parent = nullptr);
    ~CardDeckMainWindow();

private slots:
    void HitButtonClicked();
    void StandButtonClicked();
    void DoubleButtonClicked();
    void SplitButtonClicked();

private:

    Blackjack game;


    QWidget* central;

    QLabel* bankrollLabel;
    QLabel* dealerLabel;

    QHBoxLayout* dealerCardLayout;
    QVBoxLayout* playerHandsLayout;

    std::vector<QHBoxLayout*> playerCardLayouts;


    void setupUI();
    void updateDisplay();

    QPixmap loadCardImage(const Card& c);
};

#endif
