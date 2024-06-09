#pragma once

#include <QStyledItemDelegate>
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QTableView>
#include <QSqlTableModel>
#include <QStyledItemDelegate>
#include <QPushButton>
#include <QDebug>
#include <QMouseEvent>

#include <iostream>

class ButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT

signals:
    void buttonClicked(const QModelIndex &index);

public:
    ButtonDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (index.column() == index.model()->columnCount() - 1 ||
            index.column() == index.model()->columnCount() - 2 ||
            index.column() == index.model()->columnCount() - 3 ||
            index.column() == index.model()->columnCount() - 4) {
            QStyleOptionButton button;
            button.rect = option.rect;
            if (index.column() == index.model()->columnCount() - 1)
                button.text = "Топ";
            else if (index.column() == index.model()->columnCount() - 2)
                button.text = "Скидка";
            else if (index.column() == index.model()->columnCount() - 3)
                button.text = "Новинка";
            else
                button.text = "Другое";
            button.state = QStyle::State_Enabled;

            QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
    {
        if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                emit buttonClicked(index);
                return true;
            }
        }
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

};
