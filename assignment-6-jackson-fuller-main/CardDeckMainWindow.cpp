#include "CardDeckMainWindow.h"
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>

CardDeckMainWindow::CardDeckMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    game.startNewRound(50);
    updateDisplay();
}

CardDeckMainWindow::~CardDeckMainWindow() {}

QPixmap CardDeckMainWindow::loadCardImage(const Card& c)
{
    QString value = QString::fromStdString(c.GetValue().toString());
    QString suit = QString::fromStdString(c.GetSuit().toString());

    return QPixmap(":CardImages/" + value + "_" + suit + ".png")
        .scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void CardDeckMainWindow::setupUI()
{
    central = new QWidget(this);
    setCentralWidget(central);
    central->setStyleSheet("background-color: #0A5A27;");

    QVBoxLayout* main = new QVBoxLayout(central);
    main->setContentsMargins(20, 20, 20, 20);
    main->setSpacing(20);


    bankrollLabel = new QLabel("Bankroll: $1000");
    bankrollLabel->setAlignment(Qt::AlignCenter);
    bankrollLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
    main->addWidget(bankrollLabel);


    QLabel* dealerTitle = new QLabel("DEALER");
    dealerTitle->setAlignment(Qt::AlignCenter);
    dealerTitle->setStyleSheet("color: #FFD700; font-size: 26px; font-weight: bold;");
    main->addWidget(dealerTitle);

    dealerCardLayout = new QHBoxLayout();
    dealerCardLayout->setSpacing(10);
    dealerCardLayout->setAlignment(Qt::AlignCenter);
    main->addLayout(dealerCardLayout);


    QFrame *divider = new QFrame();
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("color: white;");
    main->addWidget(divider);


    QLabel *playerTitle = new QLabel("PLAYER HANDS");
    playerTitle->setAlignment(Qt::AlignCenter);
    playerTitle->setStyleSheet("color: #FFD700; font-size: 24px; font-weight: bold;");
    main->addWidget(playerTitle);

    playerHandsLayout = new QVBoxLayout();
    playerHandsLayout->setSpacing(15);
    main->addLayout(playerHandsLayout);


    QFrame *divider2 = new QFrame();
    divider2->setFrameShape(QFrame::HLine);
    divider2->setStyleSheet("color: white;");
    main->addWidget(divider2);


    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->setSpacing(20);
    buttons->setAlignment(Qt::AlignCenter);

    auto makeButton = [&](QString text) {
        QPushButton *btn = new QPushButton(text);
        btn->setMinimumWidth(120);
        btn->setStyleSheet(
            "background-color: #FFFFFF; "
            "font-size: 18px; "
            "padding: 10px; "
            "border-radius: 6px;"
            );
        return btn;
    };

    QPushButton *hitBtn = makeButton("Hit");
    QPushButton *standBtn = makeButton("Stand");
    QPushButton *doubleBtn = makeButton("Double");
    QPushButton *splitBtn = makeButton("Split");

    buttons->addWidget(hitBtn);
    buttons->addWidget(standBtn);
    buttons->addWidget(doubleBtn);
    buttons->addWidget(splitBtn);

    main->addLayout(buttons);

    connect(hitBtn, &QPushButton::clicked, this, &CardDeckMainWindow::HitButtonClicked);
    connect(standBtn, &QPushButton::clicked, this, &CardDeckMainWindow::StandButtonClicked);
    connect(doubleBtn, &QPushButton::clicked, this, &CardDeckMainWindow::DoubleButtonClicked);
    connect(splitBtn, &QPushButton::clicked, this, &CardDeckMainWindow::SplitButtonClicked);

    setMinimumSize(1100, 800);
}

void CardDeckMainWindow::updateDisplay()
{

    std::function<void(QLayout*)> clearLayout = [&](QLayout* layout)
    {
        if (!layout) return;

        while (QLayoutItem* item = layout->takeAt(0))
        {
            if (QWidget* widget = item->widget())
            {
                widget->deleteLater();
            }
            else if (QLayout* childLayout = item->layout())
            {
                clearLayout(childLayout);
                delete childLayout;
            }

        }
    };


    clearLayout(dealerCardLayout);

    const Hand& dealer = game.getDealerHand();
    for (const Card& c : dealer.cards)
    {
        QLabel* card = new QLabel();
        card->setPixmap(loadCardImage(c));
        dealerCardLayout->addWidget(card);
    }


    clearLayout(playerHandsLayout);
    playerCardLayouts.clear();

    const vector<Hand>& hands = game.getPlayerHands();
    int active = game.getCurrentHandIndex();

    for (int i = 0; i < (int)hands.size(); i++)
    {
        QHBoxLayout *row = new QHBoxLayout();
        row->setSpacing(10);

        QString label = QString("Hand %1 | Bet: $%2")
                            .arg(i + 1)
                            .arg(game.getBetForHand(i));

        QLabel *handLabel = new QLabel(label);
        handLabel->setStyleSheet(
            i == active
                ? "color: #00FF00; font-size: 20px; font-weight: bold;"
                : "color: white; font-size: 18px;"
            );
        row->addWidget(handLabel);

        for (const Card& c : hands[i].cards)
        {
            QLabel *cardImg = new QLabel();
            cardImg->setPixmap(loadCardImage(c));
            row->addWidget(cardImg);
        }

        playerHandsLayout->addLayout(row);
        playerCardLayouts.push_back(row);
    }

    bankrollLabel->setText("Bankroll: $" + QString::number(game.getPlayerMoney()));
}


void CardDeckMainWindow::HitButtonClicked()
{
    game.playerHit();
    updateDisplay();

    if (game.isRoundOver()) {
        QMessageBox::information(this, "Round Over", game.resultString());
        game.startNewRound(50);
        updateDisplay();
    }
}

void CardDeckMainWindow::StandButtonClicked()
{
    game.playerStand();
    updateDisplay();

    if (game.isRoundOver()) {
        QMessageBox::information(this, "Round Over", game.resultString());
        game.startNewRound(50);
        updateDisplay();
    }
}

void CardDeckMainWindow::DoubleButtonClicked()
{
    game.playerDouble();
    updateDisplay();

    if (game.isRoundOver()) {
        QMessageBox::information(this, "Round Over", game.resultString());
        game.startNewRound(50);
        updateDisplay();
    }
}

void CardDeckMainWindow::SplitButtonClicked()
{
    if (!game.playerCanSplit()) {
        QMessageBox::warning(this, "Cannot Split",
                             "Your first two cards must match to split.");
        return;
    }

    game.playerSplit();
    updateDisplay();
}
