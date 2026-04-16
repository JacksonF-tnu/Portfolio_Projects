/********************************************************************************
** Form generated from reading UI file 'qsqlconnectiondialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QSQLCONNECTIONDIALOG_H
#define UI_QSQLCONNECTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QSqlConnectionDialogUi
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *connGroupBox;
    QFormLayout *formLayout;
    QComboBox *comboDriver;
    QLabel *textLabel4;
    QLabel *textLabel2;
    QLineEdit *editDatabase;
    QSpinBox *portSpinBox;
    QLabel *textLabel3;
    QLineEdit *editPassword;
    QLineEdit *editUsername;
    QLineEdit *editHostname;
    QLabel *textLabel5;
    QLabel *textLabel5_2;
    QLabel *textLabel4_2;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QCheckBox *dbCheckBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *QSqlConnectionDialogUi)
    {
        if (QSqlConnectionDialogUi->objectName().isEmpty())
            QSqlConnectionDialogUi->setObjectName("QSqlConnectionDialogUi");
        QSqlConnectionDialogUi->resize(315, 302);
        vboxLayout = new QVBoxLayout(QSqlConnectionDialogUi);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
        vboxLayout->setContentsMargins(8, 8, 8, 8);
        vboxLayout->setObjectName("vboxLayout");
        connGroupBox = new QGroupBox(QSqlConnectionDialogUi);
        connGroupBox->setObjectName("connGroupBox");
        formLayout = new QFormLayout(connGroupBox);
        formLayout->setObjectName("formLayout");
        comboDriver = new QComboBox(connGroupBox);
        comboDriver->setObjectName("comboDriver");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, comboDriver);

        textLabel4 = new QLabel(connGroupBox);
        textLabel4->setObjectName("textLabel4");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, textLabel4);

        textLabel2 = new QLabel(connGroupBox);
        textLabel2->setObjectName("textLabel2");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, textLabel2);

        editDatabase = new QLineEdit(connGroupBox);
        editDatabase->setObjectName("editDatabase");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, editDatabase);

        portSpinBox = new QSpinBox(connGroupBox);
        portSpinBox->setObjectName("portSpinBox");
        portSpinBox->setMaximum(65535);
        portSpinBox->setMinimum(-1);
        portSpinBox->setValue(-1);

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, portSpinBox);

        textLabel3 = new QLabel(connGroupBox);
        textLabel3->setObjectName("textLabel3");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, textLabel3);

        editPassword = new QLineEdit(connGroupBox);
        editPassword->setObjectName("editPassword");
        editPassword->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, editPassword);

        editUsername = new QLineEdit(connGroupBox);
        editUsername->setObjectName("editUsername");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, editUsername);

        editHostname = new QLineEdit(connGroupBox);
        editHostname->setObjectName("editHostname");

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, editHostname);

        textLabel5 = new QLabel(connGroupBox);
        textLabel5->setObjectName("textLabel5");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, textLabel5);

        textLabel5_2 = new QLabel(connGroupBox);
        textLabel5_2->setObjectName("textLabel5_2");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, textLabel5_2);

        textLabel4_2 = new QLabel(connGroupBox);
        textLabel4_2->setObjectName("textLabel4_2");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, textLabel4_2);


        vboxLayout->addWidget(connGroupBox);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName("hboxLayout");
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout->addItem(spacerItem);

        dbCheckBox = new QCheckBox(QSqlConnectionDialogUi);
        dbCheckBox->setObjectName("dbCheckBox");

        hboxLayout->addWidget(dbCheckBox);


        vboxLayout->addLayout(hboxLayout);

        buttonBox = new QDialogButtonBox(QSqlConnectionDialogUi);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        vboxLayout->addWidget(buttonBox);

#if QT_CONFIG(shortcut)
        textLabel4->setBuddy(editUsername);
        textLabel2->setBuddy(comboDriver);
        textLabel3->setBuddy(editDatabase);
        textLabel5->setBuddy(editHostname);
        textLabel5_2->setBuddy(portSpinBox);
        textLabel4_2->setBuddy(editPassword);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(comboDriver, editDatabase);
        QWidget::setTabOrder(editDatabase, editUsername);
        QWidget::setTabOrder(editUsername, editPassword);
        QWidget::setTabOrder(editPassword, editHostname);
        QWidget::setTabOrder(editHostname, portSpinBox);
        QWidget::setTabOrder(portSpinBox, dbCheckBox);

        retranslateUi(QSqlConnectionDialogUi);
        QObject::connect(dbCheckBox, &QCheckBox::toggled, connGroupBox, &QGroupBox::setDisabled);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, QSqlConnectionDialogUi, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, QSqlConnectionDialogUi, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(QSqlConnectionDialogUi);
    } // setupUi

    void retranslateUi(QDialog *QSqlConnectionDialogUi)
    {
        QSqlConnectionDialogUi->setWindowTitle(QCoreApplication::translate("QSqlConnectionDialogUi", "Connect...", nullptr));
        connGroupBox->setTitle(QCoreApplication::translate("QSqlConnectionDialogUi", "Connection settings", nullptr));
        textLabel4->setText(QCoreApplication::translate("QSqlConnectionDialogUi", "&Username:", nullptr));
        textLabel2->setText(QCoreApplication::translate("QSqlConnectionDialogUi", "D&river", nullptr));
        portSpinBox->setSpecialValueText(QCoreApplication::translate("QSqlConnectionDialogUi", "Default", nullptr));
        textLabel3->setText(QCoreApplication::translate("QSqlConnectionDialogUi", "Database Name:", nullptr));
        textLabel5->setText(QCoreApplication::translate("QSqlConnectionDialogUi", "&Hostname:", nullptr));
        textLabel5_2->setText(QCoreApplication::translate("QSqlConnectionDialogUi", "P&ort:", nullptr));
        textLabel4_2->setText(QCoreApplication::translate("QSqlConnectionDialogUi", "&Password:", nullptr));
        dbCheckBox->setText(QCoreApplication::translate("QSqlConnectionDialogUi", "Us&e predefined in-memory database", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QSqlConnectionDialogUi: public Ui_QSqlConnectionDialogUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QSQLCONNECTIONDIALOG_H
