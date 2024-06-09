#include "mysqlquery.h"

MySqlQuery::MySqlQuery() {}

QString MySqlQuery::SELECT(QString table, QVector<QString> columns, QVector<QString> values, QVector<QString> conditions)
{
    QString query = "";
    /*QString query = "SELECT ";
  if (values.isEmpty()) {
    query += "* ";
  } else {
    for (int i = 0; i < columns.size(); i++) {
      query += columns[i] + "=" + values[i];
      query += ", ";
    }
    query = query.left(query.size() - 2);
    query += " ";
  }
  query += "FROM " + table;
  */
    if (conditions.size() != 0) {
        //query += " WHERE ";
        for (int i = 0; i < conditions.size(); i++) {
            query += "CAST(" + columns[i] + " as TEXT) LIKE " + conditions[i];
            query += " AND ";
        }
        query = query.left(query.size() - 5);
    }
    //query += ";";
    return query;
}

QString MySqlQuery::INSERT(QString table, QVector<QString> columns, QVector<QString> values)
{
    QString query = "INSERT INTO " + table + "(";
    for (auto i : columns) {
        query += i;
        query += ", ";
    }
    query = query.left(query.size() - 2);
    query += ") ";
    query += "VALUES(";
    for (auto i : values) {
        query += i;
        query += ", ";
    }
    query = query.left(query.size() - 2);
    query += ");";
    return query;
}

QString MySqlQuery::UPDATE(QString table, QVector<QString> columns, QVector<QString> values, QVector<QString> conditions)
{
    QString query = "UPDATE " + table + " SET ";
    for (int i = 0; i < columns.size(); i++) {
        if (values[i].isEmpty()) continue;
        query += columns[i] + "=" + values[i];
        query += ", ";
    }
    query = query.left(query.size() - 2);
    query += " WHERE ";
    for (int i = 0; i < conditions.size(); i++) {
        query += columns[i] + "=" + conditions[i];
        query += ", ";
    }
    query = query.left(query.size() - 2);
    query += ";";
    return query;
}

QString MySqlQuery::DELETE(QString table, QVector<QString> conditions)
{
    QString query = "DELETE FROM " + table + " WHERE ";
    for (auto i : conditions) {
        query += i;
        query += ", ";
    }
    query = query.left(query.size() - 2);
    query += ";";
    return query;
}
