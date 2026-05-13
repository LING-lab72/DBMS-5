#pragma once

#include "CatalogTypes.h"
#include "../engine/DBEngine.h"
#include <QObject>
#include <QString>

class QtSessionAdapter : public QObject
{
    Q_OBJECT

public:
    explicit QtSessionAdapter(const QString &dataDir, QObject *parent = nullptr);

    const Session &session() const;

public slots:
    void executeSql(const QString &sql);
    void connectUser(const QString &user, const QString &password);
    void refreshCatalog();
    void loadTable(const QString &database, const QString &table);

signals:
    void resultReady(const QueryResult &result);
    void tableReady(const QString &database, const QString &table, const QueryResult &result);
    void catalogReady(const CatalogSnapshot &catalog);
    void sessionChanged(const QString &user, const QString &database, bool inTransaction);

private:
    void emitSessionChanged();

    DBEngine engine_;
    Session session_;
};
