#pragma once

#include <QString>
#include <QVector>

class MySqlQuery
{
public:
    MySqlQuery();
    static QString SELECT(QString table, QVector<QString> columns, QVector<QString> values, QVector<QString> conditions);
    static QString INSERT(QString table, QVector<QString> columns, QVector<QString> values);
    static QString UPDATE(QString table, QVector<QString> columns, QVector<QString> values, QVector<QString> conditions);
    static QString DELETE(QString table, QVector<QString> conditions);
};
