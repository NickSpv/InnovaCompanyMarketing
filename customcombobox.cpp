#include "customcombobox.h"


void CustomComboBox::updateComboBoxItems(const QString &text) {
    clear();

    QStringList values;
    for (int i = 0; i < model->rowCount(); ++i) {
        QString value = model->index(i, this->my_index).data().toString();
        if (value.contains(text) or text.isEmpty()) {
            values << value;
        }
    }
    values.removeDuplicates();
    blockSignals(true);
    addItems(values);
    setCurrentText(text);
    blockSignals(false);
}

void CustomComboBox::setModel(QSqlTableModel *model) {
    this->model = model;
}

void CustomComboBox::setMyIndex(int my_index) {
    this->my_index = my_index;
}
