#include <string>
#include <cctype>
#include <cassert>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QFontDatabase>
#include <QEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QToolButton>
#include <QTextCodec>
#include <QSplitter>
#include <QScrollArea>
#include <QSettings>
#include <QMessageBox>
#include <QErrorMessage>
#include <QDebug>

#include "sessionwindow.hpp"
#include "dataviewdock.hpp"
#include "inputbar.hpp"
#include "savelayoutsdialog.hpp"
#include "managelayoutsdialog.hpp"


using namespace std;


static QTableWidgetItem* create_table_entry(const QVariant& p_content, int p_alignment)
{
    auto l_item_ptr = new QTableWidgetItem;

    l_item_ptr->setFlags(l_item_ptr->flags() ^ Qt::ItemIsEditable);
    l_item_ptr->setTextAlignment(p_alignment);
    l_item_ptr->setData(0, p_content);

    return l_item_ptr;
}

static QTableWidgetItem* create_index_table_entry(int p_content)
{
    return create_table_entry(p_content, Qt::AlignRight | Qt::AlignVCenter);
}

static QTableWidgetItem* create_value_table_entry(up::base_types::num_t p_content)
{
    return create_table_entry(QString("%1").arg(p_content), Qt::AlignRight | Qt::AlignVCenter);
}

static QTableWidgetItem* create_value_table_entry(up::base_types::fnum_t p_content)
{
    return create_table_entry(QString("%1").arg(p_content), Qt::AlignRight | Qt::AlignVCenter);
}

template <typename tp_storage>
static QTableWidgetItem* create_value_table_entry(const up::dictionary::entry_template<tp_storage>& p_content)
{            
    return create_table_entry(QString::fromUtf8(reinterpret_cast<const char*>(p_content.word().data()), p_content.word().size()), Qt::AlignLeft | Qt::AlignVCenter);
}

template <typename tp_container_type>
void update_table_view(QTableWidget* p_table_view_ptr, tp_container_type& p_stack)
{
    p_table_view_ptr->clearContents();
    p_table_view_ptr->setRowCount(p_stack.size());

    int l_row = 0;

    for(auto&& l_entry : p_stack)
    {
        p_table_view_ptr->setItem(l_row, 0, create_index_table_entry(l_row));
        p_table_view_ptr->setItem(l_row, 1, create_value_table_entry(l_entry));

        l_row++;
    }
}

SessionWindow::SessionWindow(QWidget* p_parent_ptr)
    : QMainWindow{ p_parent_ptr }
{
    m_stack_dock_ptr  = new DataViewDock{ this, "stack",                 ""     };
    m_fstack_dock_ptr = new DataViewDock{ this, "floating point stack",  ""     };
    m_rstack_dock_ptr = new DataViewDock{ this, "return stack",          ""     };
    m_dict_dock_ptr   = new DataViewDock{ this, "dictionary",            "word" };

    addDockWidget(Qt::LeftDockWidgetArea,  m_stack_dock_ptr );
    addDockWidget(Qt::LeftDockWidgetArea,  m_fstack_dock_ptr);
    addDockWidget(Qt::RightDockWidgetArea, m_dict_dock_ptr  );
    addDockWidget(Qt::RightDockWidgetArea, m_rstack_dock_ptr);

    setDockOptions(AnimatedDocks | AllowTabbedDocks | VerticalTabs);

    setCorner(Qt::TopLeftCorner,     Qt::LeftDockWidgetArea );
    setCorner(Qt::BottomLeftCorner,  Qt::LeftDockWidgetArea );
    setCorner(Qt::TopRightCorner,    Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    m_stack_dock_ptr->table()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(
        m_stack_dock_ptr->table(), &DataViewDock::customContextMenuRequested,
        this, &SessionWindow::on_custom_context_menu_requested
    );

    m_dock_window_settings_menu = create_dock_window_settings_menu();

    m_output_ptr = new QTextEdit{ this };
    m_output_ptr->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    m_output_ptr->setReadOnly(true);
    m_output_ptr->setProperty("class", "output-window");

    m_input_bar_ptr = new InputBar{ this };

    m_splitter = new QSplitter{ Qt::Vertical, this };
    m_splitter->addWidget(m_output_ptr);
    m_splitter->addWidget(m_input_bar_ptr);
    m_splitter->setStretchFactor(0, 1);
    m_splitter->setStretchFactor(1, 0);
    m_splitter->setCollapsible(0, false);
    m_splitter->setCollapsible(1, false);
    m_splitter->setProperty("class", "central-widget");

    layout()->update();
    layout()->activate();

    m_initial_splitter_sizes = m_splitter->sizes();

    for (int i = 0; i < m_splitter->count(); i++)
    {
       m_splitter->handle(i)->setEnabled(false);
    }

    setCentralWidget(m_splitter);

    connect(m_input_bar_ptr, &InputBar::input_accepted,    this, &SessionWindow::push_input_data);
    connect(m_input_bar_ptr, &InputBar::line_mode_changed, this, &SessionWindow::on_line_mode_changed);

    m_text_codec_ptr = QTextCodec::codecForName("Windows-1250");

    assert(m_text_codec_ptr);
}

void SessionWindow::push_input_data(QString p_str)
{
    m_input_queue.push(p_str);
}

up::base_types::dynamic_string_t SessionWindow::refill_input_buffer()
{
    up::base_types::dynamic_string_t l_str = m_input_queue.pop().toStdString();

    return l_str;
}

void SessionWindow::write_str(up::base_types::string_view_t p_str, up::io::format p_fmt)
{
    switch(p_fmt)
    {
    case up::io::format::ERROR:
        m_output_ptr->setTextColor(Qt::red);
        break;
    case up::io::format::INPUT:
        m_output_ptr->setTextColor(QColor{ 100, 149, 237 });
        break;
    case up::io::format::SUCCESS:
        m_output_ptr->setTextColor(Qt::green);
        break;
    case up::io::format::DEFAULT:
        break;
    };

    m_output_ptr->insertPlainText(m_text_codec_ptr->toUnicode(p_str.data(), static_cast<int>(p_str.size())));
    scroll_to_bottom();
}

void SessionWindow::write_chr(up::base_types::char_t p_chr, up::base_types::unum_t p_count, up::io::format p_fmt)
{
    up::base_types::dynamic_string_t l_str(p_count, p_chr);

    write_str(l_str, p_fmt);
}

void SessionWindow::update(SessionWindow::environment_type& p_env)
{
    update_table_view(m_stack_dock_ptr->table(),  p_env.stack);
    update_table_view(m_rstack_dock_ptr->table(), p_env.rstack);
    update_table_view(m_fstack_dock_ptr->table(), p_env.fstack);
    update_table_view(m_dict_dock_ptr->table(),   p_env.dict);
}

void SessionWindow::on_custom_context_menu_requested(const QPoint& p_pos [[maybe_unused]])
{
    /*
    QMenu menu{ this };

    const auto l_indices = m_stack_dock_ptr->table()->selectionModel()->selectedRows();

    if(!l_indices.empty())
    {
        auto l_dup_ptr      = l_indices.size() == 1 ? menu.addAction("dup") : nullptr;
        auto l_swap_ptr     = l_indices.size() == 2 ? menu.addAction("swap") : nullptr;
        auto l_drop_ptr     = menu.addAction("drop");
        auto l_selected_ptr = menu.exec(m_stack_dock_ptr->table()->viewport()->mapToGlobal(p_pos));

        if(l_selected_ptr != nullptr)
        {
            if (l_selected_ptr == l_drop_ptr)
            {
                for_each(l_indices.rbegin(), l_indices.rend(), [this](const auto index)
                {
                    m_env.stack.erase(index.row());
                });
            }
            else
            if(l_selected_ptr == l_dup_ptr)
            {
                const auto l_row = l_indices[0].row();

                m_env.stack.insert(l_row , m_env.stack.get(l_row));
            }
            else
            if(l_selected_ptr == l_swap_ptr)
            {
                auto l_tmp = m_env.stack.get(l_indices[0].row());

                m_env.stack.set(l_indices[0].row(), m_env.stack.get(l_indices[1].row()));
                m_env.stack.set(l_indices[1].row(), l_tmp);
            }
        }
    }
    */
}

QMenu* SessionWindow::create_dock_window_settings_menu()
{
    auto l_dock_window_settings_menu_ptr = new QMenu{ this };

    l_dock_window_settings_menu_ptr->addAction(m_stack_dock_ptr->CreateVisibilityToggleAction());
    l_dock_window_settings_menu_ptr->addAction(m_fstack_dock_ptr->CreateVisibilityToggleAction());
    l_dock_window_settings_menu_ptr->addAction(m_rstack_dock_ptr->CreateVisibilityToggleAction());
    l_dock_window_settings_menu_ptr->addAction(m_dict_dock_ptr->CreateVisibilityToggleAction());
    l_dock_window_settings_menu_ptr->addSeparator();
    l_dock_window_settings_menu_ptr->addAction("save layout", this, &SessionWindow::on_save_layout);
    l_dock_window_settings_menu_ptr->addAction("manage layouts", this, &SessionWindow::on_manage_layouts);
    l_dock_window_settings_menu_ptr->addAction("restore default layout", this, &SessionWindow::on_restore_default_layout);
    l_dock_window_settings_menu_ptr->addAction("as initial layout", this, &SessionWindow::on_as_initial_layout);

    return l_dock_window_settings_menu_ptr;
}

QHBoxLayout* SessionWindow::create_input_bar(const QFont&)
{
    QHBoxLayout* l_input_layout_ptr = new QHBoxLayout{};
    l_input_layout_ptr->setSpacing(0);

    QToolButton* l_run_button_ptr = new QToolButton;
    l_run_button_ptr->setIcon(QIcon{ ":/neutral/icons/run.png" });
    l_run_button_ptr->setIconSize({ 32, 32 });
    l_run_button_ptr->setProperty("class", "input-button");
    l_input_layout_ptr->addWidget(l_run_button_ptr);

    //m_input_ptr = new QLineEdit{};
    //m_input_ptr->setFont(p_font);
    //m_input_ptr->setPlaceholderText("type here...");
    //p_input->setClearButtonEnabled(true);
    //m_input_ptr->installEventFilter(this);
    //m_input_switcher_ptr->addWidget(m_input_ptr);

    QToolButton* l_multiline_button_ptr = new QToolButton;
    l_multiline_button_ptr->setIcon(QIcon{ ":/neutral/icons/multiline.png" });
    l_multiline_button_ptr->setIconSize({ 32, 32 });
    l_multiline_button_ptr->setProperty("class", "input-button");
    l_input_layout_ptr->addWidget(l_multiline_button_ptr);

    QToolButton* l_clear_button_ptr = new QToolButton;
    l_clear_button_ptr->setIcon(QIcon{ ":/neutral/icons/clear.png" });
    l_clear_button_ptr->setIconSize({ 32, 32 });
    l_clear_button_ptr->setProperty("class", "input-button");
    l_input_layout_ptr->addWidget(l_clear_button_ptr);

    return l_input_layout_ptr;
}

void SessionWindow::set_session_id(int p_session_id) noexcept
{
    m_session_id = p_session_id;
}

int SessionWindow::get_session_id() const noexcept
{
    return m_session_id;
}

void SessionWindow::on_line_mode_changed(LineMode p_line_mode)
{
    if(p_line_mode == LineMode::SingleLineMode)
    {
        for (int i = 0; i < m_splitter->count(); i++)
        {
           m_splitter->handle(i)->setEnabled(false);
        }

        m_splitter->setStretchFactor(0, 1);
        m_splitter->setStretchFactor(1, 0);
        m_splitter->setSizes(m_initial_splitter_sizes);
    }
    else
    {
        for (int i = 0; i < m_splitter->count(); i++)
        {
            m_splitter->handle(i)->setEnabled(true);
        }

        auto sizes = m_splitter->sizes();
        const double total_height = std::accumulate(std::begin(sizes), std::end(sizes), 0);
        m_splitter->setSizes({ static_cast<int>(total_height * 2/3), static_cast<int>(total_height * 1/3) });
    }
}

void SessionWindow::scroll_to_bottom()
{
    auto cursor = m_output_ptr->textCursor();

    cursor.movePosition(QTextCursor::End);
    m_output_ptr->setTextCursor(cursor);
}

std::pair<int, QString> SessionWindow::run_save_layout_dialog()
{
    SaveLayoutDialog l_save_layout_dialog{ this };

    const auto l_execution_result = l_save_layout_dialog.exec();

    const auto l_layout_path = (l_execution_result == QDialog::Accepted) ?
        l_save_layout_dialog.get_layout_path() :
        QString{};

    return { l_execution_result, l_layout_path };
}

void SessionWindow::on_save_layout()
{
    int l_dialog_code;
    QString l_layout_path;

    std::tie(l_dialog_code, l_layout_path) = run_save_layout_dialog();

    if(l_dialog_code != QDialog::Accepted)
        return;

    assert(!l_layout_path.isEmpty());

    QFile l_file{ l_layout_path };

    if(l_file.open(QIODevice::WriteOnly))
    {
        QDataStream l_data_stream(&l_file);

        l_data_stream << saveState();
        l_file.flush();
        l_file.close();
    }
    else
    {
        const auto l_msg = QString{ "Unable to save layout %1" }.arg(l_layout_path);

        QMessageBox{}.critical(this, "Filesystem Error", l_msg);
    }
}

void SessionWindow::on_manage_layouts()
{
    ManageLayoutsDialog l_manage_layouts_dialog{ this };

    if(l_manage_layouts_dialog.exec() == QDialog::Accepted)
    {
        QFile l_file{ l_manage_layouts_dialog.get_layout_path() };

        if(!l_file.open(QFile::ReadOnly))
        {
            const auto l_msg = QString("Unable to open layout file '%1'").arg(l_manage_layouts_dialog.get_layout_path());

            QMessageBox{}.critical(this, "Layout File Not Found", l_msg);
        }

        QDataStream l_data_stream(&l_file);
        QByteArray l_state_data;

        l_data_stream >> l_state_data;

        if(!restoreState(l_state_data))
        {
            const auto l_msg = QString("Unable to read layout data from '%1'").arg(l_manage_layouts_dialog.get_layout_path());

            QMessageBox{}.critical(this, "Invalid Layout Data", l_msg);
        }

        l_file.close();
    }
}

void SessionWindow::on_restore_default_layout()
{
    const auto l_layout_path_ptr = ":/neutral/layouts/default.lay";

    QFile l_file(l_layout_path_ptr);

    if(!l_file.open(QFile::ReadOnly))
    {
        const auto l_msg = QString("Unable to open layout file '%1'").arg(l_layout_path_ptr);

        QMessageBox{}.critical(this, "Layout File Not Found", l_msg);
    }

    QDataStream l_data_stream(&l_file);
    QByteArray l_state_data;

    l_data_stream >> l_state_data;

    if(!restoreState(l_state_data))
    {
        const auto l_msg = QString("Unable to load layout '%1'").arg(l_layout_path_ptr);

        QMessageBox{}.critical(this, "Invalid Layout", l_msg);
    }

    l_file.close();
}

void SessionWindow::on_as_initial_layout()
{

    QDir l_default_layout_dir;

    l_default_layout_dir.setPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

    const auto l_default_layout_path = l_default_layout_dir.filePath("initial_layout");
    QFile l_file{ l_default_layout_dir.filePath("initial_layout") };

    if(l_file.open(QIODevice::WriteOnly))
    {
        QDataStream l_data_stream(&l_file);

        l_data_stream << saveState();
        l_file.close();
    }
    else
    {
        const auto l_msg = QString{ "Unable to save layout %1" }.arg(l_default_layout_path);

        QMessageBox{}.critical(this, "Filesystem Error", l_msg);
    }
}

void SessionWindow::try_to_load_initial_layout()
{
    QDir l_initial_layout_dir;

    l_initial_layout_dir.setPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

    const auto l_initial_layout_path = l_initial_layout_dir.filePath("initial_layout");
    const QFileInfo l_initial_layout_file_info{ l_initial_layout_path };
    const auto l_has_initial_layout_file = l_initial_layout_file_info.exists() && l_initial_layout_file_info.isFile();

    if(l_has_initial_layout_file)
    {
        QFile l_initial_layout_file{ l_initial_layout_path };

        if(l_initial_layout_file.open(QIODevice::ReadOnly))
        {
            QDataStream l_initial_layout_data_stream(&l_initial_layout_file);
            QByteArray l_initial_layout_state_data;

            l_initial_layout_data_stream >> l_initial_layout_state_data;

            if(!restoreState(l_initial_layout_state_data))
            {
                const auto l_msg = QString("Unable to read layout '%1'").arg(l_initial_layout_path);

                QMessageBox{}.critical(this, "Invalid Layout", l_msg);
            }

            l_initial_layout_file.flush();
            l_initial_layout_file.close();
        }
    }
}

void SessionWindow::showEvent(QShowEvent* p_show_event_ptr)
{
    try_to_load_initial_layout();

    up::interpreter::interpret(
        up::interpreter::ASYNC,
        *this,
        m_stack_storage,
        m_fstack_storage,
        m_rstack_storage,
        m_dict_storage
    );

    QMainWindow::showEvent(p_show_event_ptr);

}

