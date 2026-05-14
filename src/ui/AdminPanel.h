#pragma once

#include <QWidget>

class QCheckBox;
class QComboBox;
class QLineEdit;

class AdminPanel : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPanel(QWidget *parent = nullptr);

public slots:
    void setCurrentTable(const QString &database, const QString &table);

signals:
    void sqlRequested(const QString &sql);
    void refreshRequested();

private:
    QString targetName() const;
    QString quoteString(const QString &value) const;
    QStringList selectedPrivileges() const;
    void emitIfNotEmpty(const QString &sql);

    QLineEdit *databaseEdit_;
    QLineEdit *tableEdit_;
    QLineEdit *columnEdit_;
    QLineEdit *columnTypeEdit_;
    QLineEdit *defaultEdit_;
    QCheckBox *notNullCheck_;
    QLineEdit *indexEdit_;
    QLineEdit *indexColumnsEdit_;

    QLineEdit *userEdit_;
    QLineEdit *passwordEdit_;
    QLineEdit *privilegeTargetDbEdit_;
    QLineEdit *privilegeTargetTableEdit_;
    QComboBox *privilegeCombo_;
};
