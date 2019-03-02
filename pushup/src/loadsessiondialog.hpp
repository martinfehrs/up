#pragma once

#include <QDialog>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QListWidget>
#include <QStringList>
#include <QSettings>
#include <QLineEdit>

/*
class LoadSessionDialog : public QDialog
{
  public:

    SaveSessionDialog(QWidget* p_parent_ptr)
      : QDialog{ p_parent_ptr }
    {
        setWindowTitle("Load Layout");

        QSettings l_settings{ QSettings::UserScope, "pluto", "charon" };

        auto l_file_list = new QListWidget;

        l_file_list->addItems(l_settings.allKeys());

        m_input_ptr = new QLineEdit;

        auto l_load_button_ptr = new QPushButton{ "load" };

        l_load_button_ptr->setDefault(true);

        auto l_cancel_button_ptr = new QPushButton{ "cancel" };

        auto l_button_box_ptr = new QDialogButtonBox;

        l_button_box_ptr->addButton(l_load_button_ptr, QDialogButtonBox::AcceptRole);
        l_button_box_ptr->addButton(l_cancel_button_ptr, QDialogButtonBox::RejectRole);

        auto l_layout_ptr = new QVBoxLayout;

        l_layout_ptr->addWidget(l_file_list);
        l_layout_ptr->addWidget(m_input_ptr);
        l_layout_ptr->addWidget(l_button_box_ptr);

        setLayout(l_layout_ptr);

        connect(l_load_button_ptr, &QPushButton::clicked, [this]()
        {
            QSettings l_settings{ QSettings::UserScope, "pluto", "charon" };

            l_settings.setValue(m_input_ptr->text().trimmed(), "");
            accept();
        });

        connect(l_cancel_button_ptr, &QPushButton::clicked, [this]()
        {
            reject();
        });
    }
};
*/
