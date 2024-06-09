#include "database.h"

Database::Database()
{
    database = QSqlDatabase::addDatabase("QPSQL");
    database.setHostName("0.0.0.0");
    database.setUserName("habrpguser");
    database.setDatabaseName("habrdb");
    database.setPassword("pgpwd4habr");
    if (!database.open()) {
        std::cout << database.lastError().text().toStdString();
    } else std::cout << "Success opened database\n";
    //database.exec("PRAGMA foreign_keys = ON");
    tables = database.tables();
    sqlquery = QSqlQuery(database);
}

QSqlRecord Database::selectRecord(QString)
{

}

bool Database::insertRecord(QString sql)
{
    std::cout << sql.toStdString() << std::endl;
    QSqlQuery query;
    bool isDone = query.exec(sql);
    if (!isDone) {
        std::cout << query.lastError().text().toStdString() << std::endl;
        QMessageBox::warning(new QWidget(), "Уведомление",
                             "Не удалось добавить запись\n" +
                                 query.lastError().text());
    }
    return isDone;
}

QSqlDatabase* Database::getDatebase()
{
    return &database;
}

bool Database::updateRecord(QString sql)
{
    std::cout << "Update: " << sql.toStdString() << std::endl;
    QSqlQuery query;
    bool isDone = query.exec(sql);
    if (!isDone) {
        std::cout << query.lastError().text().toStdString() << std::endl;
        QMessageBox::warning(new QWidget(), "Уведомление",
                             "Не удалось обновить запись\n" +
                                 query.lastError().text());
    }
    return isDone;
}

QSqlTableModel* Database::getModelView(QString model_name) {
    // Создание модели данных на основе результата запроса
    QSqlTableModel *model = new QSqlTableModel(nullptr, database);
    model->setTable(model_name);
    model->select();
    return model;
}
