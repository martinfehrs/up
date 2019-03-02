#pragma once


#include <QMainWindow>
#include <QString>
#include <QHBoxLayout>

#include "uibackend.hpp"

#include "up/interpreter.hpp"
#include "up/environment.hpp"


class QWidget;
class QLineEdit;
class QTextEdit;
class QPlainTextEdit;
class QMenu;
class QPoint;
class QTextCodec;
class QStackedWidget;
class QHBoxLayout;
class QSplitter;

class DataViewDock;
class InputBar;

enum class LineMode;


class SessionWindow final : public QMainWindow
{

    Q_OBJECT

  public:

    using environment_type = up::environment::environment<
        SessionWindow,
        up::memory::dynamic_storage,
        up::memory::dynamic_storage,
        up::memory::dynamic_storage,
        up::memory::dynamic_storage
    >;

    explicit SessionWindow(QWidget *parent = nullptr);

    void set_session_id(int m_session_id) noexcept;
    int get_session_id() const noexcept;

    // overrides for up::io::terminal
    up::base_types::dynamic_string_t refill_input_buffer();
    void write_str(up::base_types::string_view_t p_output_buffer, up::io::format p_fmt);
    void write_chr(up::base_types::char_t p_chr, up::base_types::unum_t p_count, up::io::format p_fmt);
    void update(environment_type& p_env);

    InputBar* m_input_bar_ptr = nullptr;
    QMenu* m_dock_window_settings_menu = nullptr;

    DataViewDock* m_stack_dock_ptr = nullptr;
    DataViewDock* m_fstack_dock_ptr = nullptr;
    DataViewDock* m_rstack_dock_ptr = nullptr;
    DataViewDock* m_dict_dock_ptr = nullptr;

  private:

    // overrides for QMainWindow
    virtual void showEvent(QShowEvent*) override;

    QMenu* create_dock_window_settings_menu();
    QHBoxLayout* create_input_bar(const QFont& font);

    void try_to_load_initial_layout();
    void scroll_to_bottom();
    std::pair<int, QString> run_save_layout_dialog();

    void on_line_mode_changed(LineMode p_line_mode);
    void on_custom_context_menu_requested(const QPoint& point);

    void on_save_layout();
    void on_manage_layouts();
    void on_restore_default_layout();
    void on_as_initial_layout();
    void on_layout_selection(const QString& p_file_path);

    // dock table slots
    void update_stack_view();
    void update_rstack_view();
    void update_fstack_view();
    void update_dict_view();

    // input slots
    void push_input_data(QString p_str);

    QTextEdit* m_output_ptr = nullptr;
    QTextCodec* m_text_codec_ptr = nullptr;
    int m_session_id = 0;
    QSplitter* m_splitter = nullptr;
    QList<int> m_initial_splitter_sizes;
    blocking_queue_template<QString> m_input_queue;
    up::memory::dynamic_storage m_stack_storage;
    up::memory::dynamic_storage m_fstack_storage;
    up::memory::dynamic_storage m_rstack_storage;
    up::memory::dynamic_storage m_dict_storage;

};
