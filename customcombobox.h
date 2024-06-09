#pragma once

#include <QComboBox>
#include <QSqlTableModel>
#include <QTextDocument>
#include <QTextCursor>

class CustomComboBox : public QComboBox {
public:
    CustomComboBox(QWidget *parent = nullptr) : QComboBox(parent) {
        this->setEditable(true);
    }

    void updateComboBoxItems(const QString &text);
    void setModel(QSqlTableModel *model);
    void setMyIndex(int my_index);

private:
    QSqlTableModel *model;
    int my_index;
};
