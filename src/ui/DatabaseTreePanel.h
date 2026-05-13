#pragma once

#include "CatalogTypes.h"
#include <QWidget>

class QTreeWidget;
class QTreeWidgetItem;

class DatabaseTreePanel : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseTreePanel(QWidget *parent = nullptr);

public slots:
    void setCatalog(const CatalogSnapshot &catalog);

signals:
    void refreshRequested();
    void tableOpenRequested(const QString &database, const QString &table);
    void sqlRequested(const QString &sql);

private:
    void openCurrentItem(QTreeWidgetItem *item);

    QTreeWidget *tree_;
};
