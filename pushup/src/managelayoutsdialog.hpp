#pragma once

#include <cassert>

#include <QDialog>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QTreeView>
#include <QFileSystemModel>
#include <QStringList>
#include <QSettings>
#include <QStandardPaths>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>

class ManageLayoutsDialog : public QDialog
{
  Q_OBJECT

  public:

    ManageLayoutsDialog(QWidget* p_parent_ptr)
      : QDialog{ p_parent_ptr }
    {
        setWindowTitle("Manage Layouts");
        resize(640, 480);

        QDir l_layout_dir{ QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };

        l_layout_dir.mkpath("layouts");
        l_layout_dir.cd("layouts");

        m_file_system_model_ptr = new QFileSystemModel{ this };
        m_file_system_model_ptr->setRootPath(l_layout_dir.path());
        m_file_system_model_ptr->setReadOnly(false);

        m_file_list_ptr = new QTreeView{ this };
        m_file_list_ptr->setModel(m_file_system_model_ptr);
        m_file_list_ptr->setRootIndex(m_file_system_model_ptr->index(l_layout_dir.path()));
        m_file_list_ptr->header()->setSectionResizeMode(0, QHeaderView::Stretch);
        m_file_list_ptr->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_file_list_ptr->setSelectionMode(QAbstractItemView::SingleSelection);
        m_file_list_ptr->hideColumn(1);
        m_file_list_ptr->hideColumn(2);
        m_file_list_ptr->setRootIsDecorated(false);
        m_file_list_ptr->setSortingEnabled(true);
        m_file_list_ptr->sortByColumn(0, Qt::SortOrder::AscendingOrder);

        auto l_rename_layout_button_ptr = new QPushButton{ "rename layout", this };
        auto l_delete_layout_button_ptr = new QPushButton{ "delete layout", this };
        auto l_use_layout_button_ptr    = new QPushButton{ "use layout"   , this };
        auto l_done_button_ptr          = new QPushButton{ "done"         , this };

        l_rename_layout_button_ptr->setDisabled(true);
        l_delete_layout_button_ptr->setDisabled(true);
        l_use_layout_button_ptr->setDisabled(true);

        l_use_layout_button_ptr->setDefault(true);
        l_done_button_ptr->setAutoDefault(false);
        l_rename_layout_button_ptr->setAutoDefault(false);
        l_delete_layout_button_ptr->setAutoDefault(false);

        auto l_button_box_ptr = new QDialogButtonBox{ this };

        l_button_box_ptr->addButton(l_rename_layout_button_ptr, QDialogButtonBox::ActionRole);
        l_button_box_ptr->addButton(l_delete_layout_button_ptr, QDialogButtonBox::ActionRole);
        l_button_box_ptr->addButton(l_use_layout_button_ptr   , QDialogButtonBox::ActionRole);
        l_button_box_ptr->addButton(l_done_button_ptr         , QDialogButtonBox::RejectRole);

        auto l_layout_ptr = new QVBoxLayout{ this };

        l_layout_ptr->addWidget(m_file_list_ptr);
        l_layout_ptr->addWidget(l_button_box_ptr);

        setLayout(l_layout_ptr);

        connect(m_file_list_ptr->selectionModel(), &QItemSelectionModel::selectionChanged, [=]()
        {
            l_use_layout_button_ptr->setDisabled   (!m_file_list_ptr->selectionModel()->hasSelection());
            l_delete_layout_button_ptr->setDisabled(!m_file_list_ptr->selectionModel()->hasSelection());
            l_rename_layout_button_ptr->setDisabled(!m_file_list_ptr->selectionModel()->hasSelection());
        });

        connect(m_file_system_model_ptr, &QFileSystemModel::fileRenamed, [](const auto& p_path, const auto& p_old_name, const auto& p_new_name)
        {
            QFile l_file{ p_path + p_old_name };

            l_file.rename(p_new_name);
        });

        connect(l_use_layout_button_ptr, &QPushButton::clicked, this, &ManageLayoutsDialog::on_layout_selection);

        connect(l_delete_layout_button_ptr, &QPushButton::clicked, [this]()
        {
            const auto l_selected_path = m_file_system_model_ptr->filePath(m_file_list_ptr->currentIndex());

            QFile l_file;

            if(!l_file.remove(l_selected_path))
            {
                const auto l_msg = QString{ "Unable to delete layout %1" }.arg(l_selected_path);

                QMessageBox{}.critical(this, "Filesystem Error", l_msg);
            }
        });

        connect(l_rename_layout_button_ptr, &QPushButton::clicked, [this]()
        {
            const auto l_file_system_model_index = m_file_list_ptr->currentIndex();
            const auto l_selected_file_name = m_file_system_model_ptr->fileName(l_file_system_model_index);
            const auto l_selected_file_path = m_file_system_model_ptr->filePath(l_file_system_model_index);

            m_file_list_ptr->edit(l_file_system_model_index);

            /*
            const auto l_new_layout_name = QInputDialog::getText(
                this,
                "Rename Layout",
                "Enter new layout name",
                QLineEdit::Normal,
                l_selected_file_name,
                nullptr,
                Qt::WindowFlags()
            );

            assert(!l_new_layout_name.isEmpty());

            QFileInfo l_file_info{ l_selected_file_path };

            assert(l_file_info.exists());

            const auto l_containing_dir = l_file_info.absoluteDir();
            const auto l_new_file_path = l_containing_dir.absoluteFilePath(l_new_layout_name);

            QFile::rename(l_selected_file_path, l_new_file_path);
            */
        });

        connect(l_done_button_ptr, &QPushButton::clicked, this, &ManageLayoutsDialog::reject);

        connect(m_file_system_model_ptr, &QFileSystemModel::directoryLoaded, [this](const auto& /*p_path*/)
        {
            auto l_index = m_file_system_model_ptr->index(0, 0, m_file_list_ptr->rootIndex());
            m_file_list_ptr->setCurrentIndex(l_index);
        });
    }

    virtual void showEvent(QShowEvent* p_show_event_ptr) override
    {
        QDialog::showEvent(p_show_event_ptr);
    }

    void on_layout_selection()
    {
        m_selected_path = m_file_system_model_ptr->filePath(m_file_list_ptr->currentIndex());

        accept();
    }

    QString get_layout_path() const
    {
        return m_selected_path;
    }

  private:

    QTreeView* m_file_list_ptr = nullptr;
    QFileSystemModel* m_file_system_model_ptr = nullptr;
    QString m_selected_path;
};



