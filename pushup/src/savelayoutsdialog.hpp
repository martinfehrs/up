#pragma once

#include <QDialog>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QTreeView>
#include <QFileSystemModel>
#include <QStringList>
#include <QSettings>
#include <QLineEdit>
#include <QStandardPaths>
#include <QMessageBox>

class SaveLayoutDialog : public QDialog
{
  public:

    SaveLayoutDialog(QWidget* p_parent_ptr)
      : QDialog{ p_parent_ptr }
    {
        setWindowTitle("Save Layout");
        resize(640, 480);

        auto l_file_system_model_ptr = new QFileSystemModel{ this };

        m_layout_dir.setPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        m_layout_dir.mkpath("layouts");
        m_layout_dir.cd("layouts");

        l_file_system_model_ptr->setRootPath(m_layout_dir.path());

        auto l_file_list_ptr = new QTreeView{ this };

        l_file_list_ptr->setModel(l_file_system_model_ptr);
        l_file_list_ptr->setRootIndex(l_file_system_model_ptr->index(m_layout_dir.path()));
        l_file_list_ptr->header()->setSectionResizeMode(0, QHeaderView::Stretch);
        l_file_list_ptr->setSelectionBehavior(QAbstractItemView::SelectRows);
        l_file_list_ptr->setSelectionMode(QAbstractItemView::SingleSelection);
        l_file_list_ptr->hideColumn(1);
        l_file_list_ptr->hideColumn(2);
        l_file_list_ptr->setRootIsDecorated(false);
        l_file_list_ptr->setSortingEnabled(true);
        l_file_list_ptr->sortByColumn(0, Qt::SortOrder::AscendingOrder);

        m_input_ptr = new QLineEdit{ this };

        auto l_cancel_button_ptr = new QPushButton{ "cancel" };
        auto l_save_button_ptr = new QPushButton{ "save" };

        l_save_button_ptr->setDefault(true);
        l_save_button_ptr->setDisabled(true);
        l_cancel_button_ptr->setAutoDefault(false);

        auto l_button_box_ptr = new QDialogButtonBox{ this };

        l_button_box_ptr->addButton(l_cancel_button_ptr, QDialogButtonBox::RejectRole);
        l_button_box_ptr->addButton(l_save_button_ptr, QDialogButtonBox::AcceptRole);

        auto l_layout_ptr = new QVBoxLayout{ this };

        l_layout_ptr->addWidget(l_file_list_ptr);
        l_layout_ptr->addWidget(m_input_ptr);
        l_layout_ptr->addWidget(l_button_box_ptr);

        setLayout(l_layout_ptr);

        connect(l_file_list_ptr->selectionModel(), &QItemSelectionModel::currentRowChanged, [this, l_file_system_model_ptr](const auto& p_current_index, const auto& /*p_previous_index*/)
        {
            const auto l_row = p_current_index.row();
            const auto l_parent_index = p_current_index.parent();
            const auto l_first_column_index = l_file_system_model_ptr->index(l_row, 0, l_parent_index);

            const auto l_file_name = l_file_system_model_ptr->data(l_first_column_index).toString();

            this->m_input_ptr->setText(l_file_name);
        });

        connect(m_input_ptr, &QLineEdit::textChanged, [l_save_button_ptr](const QString& p_text)
        {
            l_save_button_ptr->setDisabled(p_text.isEmpty());
        });

        connect(l_save_button_ptr, &QPushButton::clicked, [this]()
        {
            auto l_selected_path = m_layout_dir.filePath(m_input_ptr->text().trimmed());

            assert(!l_selected_path.isEmpty());

            QFileInfo l_file_info{ l_selected_path };

            if(l_file_info.exists())
            {
                const auto l_dialog_title_ptr = "file exists";
                const auto l_dialog_msg_ptr = "file already exists. Do you really want to override it?";

                auto reply = QMessageBox::question(this, l_dialog_title_ptr, l_dialog_msg_ptr);

                if(reply == QMessageBox::No)
                {
                    return;
                }
            }

            m_selected_path = l_selected_path;
            accept();
        });

        connect(l_cancel_button_ptr, &QPushButton::clicked, [this]()
        {
            reject();
        });
    }

    virtual void showEvent(QShowEvent* p_show_event_ptr) override
    {
        m_input_ptr->setFocus();

        QDialog::showEvent(p_show_event_ptr);
    }

    QString get_layout_path() const
    {
        return m_selected_path;
    }

  private:

    QLineEdit* m_input_ptr = nullptr;
    QDir m_layout_dir;
    QString m_selected_path;
};
