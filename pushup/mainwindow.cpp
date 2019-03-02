#include <QTabWidget>
#include <QTabBar>
#include <QToolButton>
#include <QStyle>
#include <QDesktopWidget>
#include <QLayout>
#include <QLineEdit>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QWindow>

#include "mainwindow.hpp"
#include "sessionwindow.hpp"
#include "inputbar.hpp"

using namespace std;

MainWindow::MainWindow(QWidget* p_parent)
  : QMainWindow{ p_parent }
{
    setObjectName("main-window");
    resize(QDesktopWidget().availableGeometry(this).size() * 0.8);
    setContentsMargins(0, 5, 0, 0);

    auto l_new_tab_button_ptr = new QToolButton{ this };
    l_new_tab_button_ptr->setIcon(QIcon{ ":/neutral/icons/add.png"});
    l_new_tab_button_ptr->setToolTip("create new session");
    connect(l_new_tab_button_ptr, &QToolButton::clicked, this, &MainWindow::on_new_tab_button_clicked);

    m_dock_window_settings_button = new QToolButton{ this };
    m_dock_window_settings_button->setIcon(QIcon{ ":/neutral/icons/docks.png"});
    m_dock_window_settings_button->setPopupMode(QToolButton::InstantPopup);
    m_dock_window_settings_button->setToolTip("manage dock windows");

    m_tab_widget_ptr = new QTabWidget{ this };
    m_tab_widget_ptr->setMovable(true);
    m_tab_widget_ptr->setCornerWidget(l_new_tab_button_ptr, Qt::TopLeftCorner);
    m_tab_widget_ptr->setCornerWidget(m_dock_window_settings_button, Qt::TopRightCorner);

    connect(m_tab_widget_ptr, &QTabWidget::tabCloseRequested, this, &MainWindow::on_tab_close_requested);
    connect(m_tab_widget_ptr, &QTabWidget::currentChanged, this, &MainWindow::on_tab_selection);

    setCentralWidget(m_tab_widget_ptr);
    create_session();
}

SessionWindow* MainWindow::create_session()
{
    const auto l_session_id = m_session_id_pool.RequestSessionId();

    auto l_session_window_ptr = new SessionWindow{ this };

    l_session_window_ptr->set_session_id(l_session_id);

    m_tab_widget_ptr->addTab(l_session_window_ptr, QString{ "session %1" }.arg(l_session_id));
    m_session_counter++;

    if(m_session_counter >= 2)
    {
        m_tab_widget_ptr->setTabsClosable(true);
    }

    l_session_window_ptr->m_input_bar_ptr->setFocus();

    return l_session_window_ptr;
}

void MainWindow::on_new_tab_button_clicked()
{
    m_tab_widget_ptr->setCurrentWidget(create_session());
}

void MainWindow::on_tab_selection(int p_tab_index)
{
    m_current_session_ptr = qobject_cast<SessionWindow*>(m_tab_widget_ptr->widget(p_tab_index));
    m_dock_window_settings_button->setMenu(m_current_session_ptr->m_dock_window_settings_menu);
}

void MainWindow::on_tab_close_requested(int p_tab_index)
{
    const auto l_session_window_ptr = qobject_cast<SessionWindow*>(m_tab_widget_ptr->widget(p_tab_index));
    m_session_id_pool.ReleaseSessionId(l_session_window_ptr->get_session_id());
    m_tab_widget_ptr->removeTab(p_tab_index);
    m_session_counter--;

    if(m_session_counter < 2)
    {
        m_tab_widget_ptr->setTabsClosable(false);
    }
}
