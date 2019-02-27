#pragma once

#include <QMainWindow>

#include "up/interpreter.hpp"

#include "idpool.hpp"


class QTabWidget;
class QToolButton;
class SessionWindow;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

  public:

    explicit MainWindow(QWidget *parent_ptr = nullptr);

  private:

    SessionWindow* create_session();

    void on_new_tab_button_clicked();
    void on_tab_selection(int p_tab_index);
    void on_tab_close_requested(int p_tab_index);

    SessionWindow* m_current_session_ptr = nullptr;
    QTabWidget* m_tab_widget_ptr = nullptr;
    QToolButton* m_dock_window_settings_button = nullptr;
    IdPool<int, 1, 1> m_session_id_pool;
    int m_session_counter = 0;
};
