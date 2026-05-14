#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;

class SessionPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SessionPanel(QWidget *parent = nullptr);

public slots:
    void setSessionInfo(const QString &user, const QString &database, bool inTransaction);

signals:
    void connectRequested(const QString &user, const QString &password);

private:
    QLineEdit *userEdit_;
    QLineEdit *passwordEdit_;
    QLabel *statusLabel_;
};
