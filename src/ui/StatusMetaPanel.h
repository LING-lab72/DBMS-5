#pragma once

#include "../types.h"
#include <QWidget>

class QLabel;
class QTextEdit;

class StatusMetaPanel : public QWidget
{
    Q_OBJECT

public:
    explicit StatusMetaPanel(QWidget *parent = nullptr);

public slots:
    void showResultMeta(const QueryResult &result);
    void showError(const QString &message);

private:
    QLabel *summaryLabel_;
    QTextEdit *logView_;
};
