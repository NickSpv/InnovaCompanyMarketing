#pragma once

#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlError>
#include <QMessageBox>
#include "mysqlquery.h"

#include <iostream>

class Database
{
public:
    Database();
    QSqlRecord selectRecord(QString);
    bool insertRecord(QString);
    bool updateRecord(QString);
    bool deleteRecord(QString);
    QSqlTableModel* getModelView(QString);

    QSqlDatabase* getDatebase();

public:
    QStringList tables;

public:
    QSqlDatabase database;
    QSqlQuery sqlquery;
};
