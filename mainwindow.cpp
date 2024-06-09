#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    database = new Database();
    buttonDelegate = new ButtonDelegate();

    connect(buttonDelegate, &ButtonDelegate::buttonClicked, this, &MainWindow::on_delegate_button_clicked);

    // Создаем список QString
    QStringList stringList;
    stringList << "Заказы с фабриками и датами";

    // Создаем и настраиваем QComboBox
    QComboBox* comboBox = ui->comboBox;
    for(const QString& str : stringList) {
        comboBox->addItem(str);
    }
    comboBox->setCurrentIndex(-1);

    stringList.clear();
    stringList << "furniture_marketing";

    comboBox = ui->comboBox_2;
    for(const QString& str : stringList) {
        comboBox->addItem(str);
    }
    comboBox->setCurrentIndex(-1);

    this->ui->tableView->setSortingEnabled(true);
    //this->ui->tableView_2->setSortingEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    while (QLayoutItem *item = this->ui->formLayout_2->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
            delete widget;
        }
        delete item;
    }
    if (this->ui->tableView->model())
        delete this->ui->tableView->model();
    QSqlTableModel *model = database->getModelView(arg1);
    this->ui->tableView->setModel(model);
    QMap<QString, QString>* filters = new QMap<QString, QString>();
    for (int i = 0; i < model->columnCount(); ++i) {
        QLabel *label = new QLabel(model->headerData(i, Qt::Horizontal).toString());
        CustomComboBox *comboBox = new CustomComboBox();
        comboBox->setModel(model);
        comboBox->setMyIndex(i);

        QStringList values;
        for (int j = 0; j < model->rowCount(); ++j) {
            QString value = model->index(j, i).data().toString();
            values << value;
        }
        values.removeDuplicates();
        comboBox->blockSignals(true);
        comboBox->addItems(values);
        comboBox->clearEditText();
        comboBox->blockSignals(false);

        filters->insert(label->text(), "");

        //connect(comboBox, &QComboBox::currentTextChanged, comboBox, &CustomComboBox::updateComboBoxItems);
        connect(comboBox, &QComboBox::currentTextChanged, model, [model, label, filters](const QString &text){
            filters->insert(label->text(), text);
            QString filter = "";
            QMap<QString, QString>::const_iterator it;
            for (it = filters->constBegin(); it != filters->constEnd(); ++it) {
                if (it.value() != ""){
                filter += "'" + it.key() + "' LIKE '%" + it.value() + "%'";
                if (it + 1 != filters->constEnd())
                    filter += " AND ";
                }
            }
            std::cout << filter.toStdString();
            model->setFilter(filter);
            model->select();
        });

        ui->formLayout_2->addRow(label, comboBox);
    }
    this->ui->tableView->resizeColumnsToContents();

    for (int i = 0; i < this->ui->tableView->model()->columnCount(); ++i) {
        this->ui->tableView->setColumnWidth(i, this->ui->tableView->columnWidth(i) + 20);
    }
}


void MainWindow::on_pushButton_clicked()
{
    QTextDocument *doc = new QTextDocument;
    doc->setDocumentMargin(10);
    QTextCursor cursor(doc);

    QString htmlHeader = "<!DOCTYPE html>"
                         "<html>"
                         "<head>"
                         "<title>innova.nickspv.ru</title>"
                         "</head>"
                         "<body>"
                         "<h1 style=\"text-align:center\">Отчет</h1>"
                         "<p style=\"text-align:center\">" +
                         static_cast<QSqlTableModel*>(this->ui->tableView->model())->tableName() +
                         ".</p>";
/*
    for (int i = 0; i < this->ui->formLayout_2->rowCount(); ++i) {
        QLayoutItem *labelItem = this->ui->formLayout_2->itemAt(i, QFormLayout::LabelRole);
        QLabel *label = qobject_cast<QLabel *>(labelItem->widget());

        QLayoutItem *fieldItem = this->ui->formLayout_2->itemAt(i, QFormLayout::FieldRole);
        QComboBox *comboBox = qobject_cast<QComboBox *>(fieldItem->widget());

        if (label and comboBox and not comboBox->currentText().isEmpty()) {
            QString labelText = label->text();
            QString comboBoxText = comboBox->currentText();
            htmlHeader += "<p style=\"text-align:center\"> " + labelText +
                          " содержит " + comboBoxText + ".</p>";
        }
    }*/
    htmlHeader += "<p>&nbsp;</p>"
                  "<p>Отчёт составил: ___________________</p>"
                  "<p>Отчёт принял:&nbsp; &nbsp; ___________________</p>"
                  "<p style=\"text-align:center\">Дата составления отчёта: " + QDate::currentDate().toString() + "</p>"
                                                      "<p>&nbsp;</p>"
                                                      "</body>"
                                                      "</html>";
    std::cout << htmlHeader.toStdString() << std::endl;
    cursor.insertHtml(htmlHeader);
    cursor.movePosition(QTextCursor::NextBlock);

    QTextTable *tableT = cursor.insertTable(this->ui->tableView->model()->rowCount() + 1, this->ui->tableView->model()->columnCount());
    QTextTableCell headerCell;
    for (int i = 0; i < this->ui->tableView->model()->columnCount(); i++) {
        headerCell = tableT->cellAt(0, i);
        QTextCursor headerCellCursor = headerCell.firstCursorPosition();
        headerCellCursor.insertText(this->ui->tableView->model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
    }

    for (int i = 0; i < this->ui->tableView->model()->rowCount(); i++){
        for (int j = 0; j < this->ui->tableView->model()->columnCount(); j++) {
            QTextTableCell cell = tableT->cellAt(i + 1, j);
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.insertText(this->ui->tableView->model()->data(this->ui->tableView->model()->index(i, j), 0).toString());
        }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.insertBlock();

    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save PDF", "", "PDF Files (*.pdf)");

    //Print to PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    doc->print(&printer);
}



void MainWindow::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    if (this->ui->tableView_2->model())
        delete this->ui->tableView_2->model();
    QSqlTableModel *model = database->getModelView(arg1);
    std::cout << model->columnCount() << std::endl;

    //this->ui->tableView_2->setSortingEnabled(false);
    if (model->columnCount() > 0) {
        model->removeColumn(0);
        model->insertColumn(model->columnCount());
        model->setHeaderData(model->columnCount() - 1, Qt::Horizontal, "Популярный товар", Qt::DisplayRole);
        std::cout << model->columnCount() << std::endl;

        model->insertColumn(model->columnCount());
        model->setHeaderData(model->columnCount() - 1, Qt::Horizontal, "Скидка", Qt::DisplayRole);
        std::cout << model->columnCount() << std::endl;

        model->insertColumn(model->columnCount());
        model->setHeaderData(model->columnCount() - 1, Qt::Horizontal, "Новая модель", Qt::DisplayRole);
        std::cout << model->columnCount() << std::endl;

        model->insertColumn(model->columnCount());
        model->setHeaderData(model->columnCount() - 1, Qt::Horizontal, "Другое", Qt::DisplayRole);
        std::cout << model->columnCount() << std::endl;

        this->ui->tableView_2->setItemDelegateForColumn(model->columnCount()-1, buttonDelegate);
        this->ui->tableView_2->setItemDelegateForColumn(model->columnCount()-2, buttonDelegate);
        this->ui->tableView_2->setItemDelegateForColumn(model->columnCount()-3, buttonDelegate);
        this->ui->tableView_2->setItemDelegateForColumn(model->columnCount()-4, buttonDelegate);

    }

    this->ui->tableView_2->setModel(model);
    this->ui->tableView_2->resizeColumnsToContents();

    for (int i = 0; i < this->ui->tableView_2->model()->columnCount(); ++i) {
        this->ui->tableView_2->setColumnWidth(i, this->ui->tableView_2->columnWidth(i) + 20);
    }
    //this->ui->tableView_2->setSortingEnabled(true);
}

void MainWindow::on_delegate_button_clicked(const QModelIndex &index)
{
    std::cout << "Button clicked in row:" << index.row() << " column:" << index.column();
    int count_columns = this->ui->tableView_2->model()->columnCount();
    QString view_name =static_cast<QSqlTableModel*>(
                            this->ui->tableView_2->model()
                            )->tableName();
    std::cout << view_name.toStdString() << std::endl;
    //this->ui->tableView_2->setModel(model);
    QAbstractItemModel *model = database->getModelView(view_name);

    int row = index.row();
    QJsonObject jsonObject;

    for (int col = 0; col < model->columnCount(); ++col) {
        QString key = model->headerData(col, Qt::Horizontal).toString();
        QVariant value = model->data(model->index(row, col));

        if (value.isValid()) {
            jsonObject[key] = value.toString();
        } else {
                // обработка невалидных значений, если необходимо
        }
    }

    if (index.column() == count_columns - 1) {
        jsonArray1.append(jsonObject);
    } else if (index.column() == count_columns - 2) {
        jsonArray2.append(jsonObject);
    } else if (index.column() == count_columns - 3) {
        jsonArray3.append(jsonObject);
    } else if (index.column() == count_columns - 4) {
        jsonArray4.append(jsonObject);
    }
}


void sendDataToServer(const QJsonDocument& jsonDocument)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("http://innova.nickspv.ru/put_data")); // Замените на адрес вашего сервера

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = manager.post(request, jsonDocument.toJson());
    std::cout << jsonDocument.toJson().toStdString();

    // Ждем ответ
    while (!reply->isFinished()) {
        qApp->processEvents();
    }

    if (reply->error() == QNetworkReply::NoError) {
        std::cout << "Данные успешно отправлены на сервер!";
    } else {
        std::cout << "Ошибка при отправке данных на сервер:" << reply->errorString().toStdString();
    }

    reply->deleteLater();
}


void MainWindow::on_pushButton_2_clicked()
{
    QJsonObject result;

    result["sale_products"] = jsonArray1;
    result["top_products"] = jsonArray2;
    result["new_products"] = jsonArray3;
    result["another_products"] = jsonArray4;

    QJsonDocument jsonDocument(result);
    sendDataToServer(jsonDocument);
}



