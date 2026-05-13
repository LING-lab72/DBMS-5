#pragma once

#include "../types.h"
#include <QWidget>

class QTableWidget;

class ResultTablePanel : public QWidget
{
    Q_OBJECT

public:
    explicit ResultTablePanel(QWidget *parent = nullptr);

public slots:
    void renderResult(const QueryResult &result);
    void clear();

private:
    static QString fieldValueToString(const FieldValue &value);

    QTableWidget *table_;
};
