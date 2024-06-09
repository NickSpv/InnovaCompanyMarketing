#pragma once

#include <QMainWindow>
#include <QStringList>
#include <QComboBox>
#include <QLabel>
#include <QFormLayout>
#include <QDate>
#include <QTextTableCell>
#include <QTextTable>
#include <QPrinter>
#include <QFileDialog>
#include <QLineEdit>
#include <QSqlQuery>
#include <QSqlResult>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork>


#include "customcombobox.h"
#include "buttondelegate.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_delegate_button_clicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    Database* database;
    ButtonDelegate *buttonDelegate;
    QJsonArray jsonArray1;
    QJsonArray jsonArray2;
    QJsonArray jsonArray3;
    QJsonArray jsonArray4;
};
