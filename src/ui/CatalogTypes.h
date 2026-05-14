#pragma once

#include <QString>
#include <QVector>

struct CatalogColumn
{
    QString name;
    QString type;
    QString nullable;
    QString key;
};

struct CatalogTable
{
    QString name;
    QVector<CatalogColumn> columns;
};

struct CatalogDatabase
{
    QString name;
    QVector<CatalogTable> tables;
};

struct CatalogUser
{
    QString name;
    QString privileges;
};

struct CatalogSnapshot
{
    QVector<CatalogDatabase> databases;
    QVector<CatalogUser> users;
};
