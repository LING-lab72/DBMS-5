#include "SessionPanel.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

SessionPanel::SessionPanel(QWidget *parent)
    : QWidget(parent),
      userEdit_(new QLineEdit(this)),
      passwordEdit_(new QLineEdit(this)),
      statusLabel_(new QLabel(this))
{
    userEdit_->setText("root");
    userEdit_->setObjectName(QStringLiteral("sessionUserEdit"));
    passwordEdit_->setText("root");
    passwordEdit_->setObjectName(QStringLiteral("sessionPasswordEdit"));
    passwordEdit_->setEchoMode(QLineEdit::Password);

    auto *connectButton = new QPushButton(tr("Connect"), this);
    connectButton->setObjectName(QStringLiteral("connectButton"));
    connect(connectButton, &QPushButton::clicked, this, [this]() {
        emit connectRequested(userEdit_->text(), passwordEdit_->text());
    });

    auto *form = new QFormLayout;
    form->addRow(tr("User"), userEdit_);
    form->addRow(tr("Password"), passwordEdit_);
    form->addRow(connectButton);

    auto *group = new QGroupBox(tr("Session"), this);
    auto *groupLayout = new QVBoxLayout(group);
    groupLayout->addLayout(form);
    groupLayout->addWidget(statusLabel_);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(group);
    layout->addStretch();

    setSessionInfo(QString(), QString(), false);
}

void SessionPanel::setSessionInfo(const QString &user, const QString &database, bool inTransaction)
{
    const QString displayUser = user.isEmpty() ? tr("anonymous") : user;
    const QString displayDb = database.isEmpty() ? tr("none") : database;
    statusLabel_->setText(tr("User: %1\nDatabase: %2\nTransaction: %3")
                              .arg(displayUser, displayDb, inTransaction ? tr("active") : tr("none")));
}
