#include <QTableWidget>
#include <QAction>
#include <QHeaderView>

#include "dataviewdock.hpp"

DataViewDock::DataViewDock(QWidget* /*p_parent*/, const char* p_title_ptr, const char* p_column_title_ptr)
  : QDockWidget{ p_title_ptr }
  , m_title{ p_title_ptr }
{
    setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    m_table_ptr = create_table(p_column_title_ptr);
    setWidget(m_table_ptr);
    setObjectName(p_title_ptr);
}

QTableWidget* DataViewDock::table()
{
    return m_table_ptr;
}

QAction* DataViewDock::CreateVisibilityToggleAction()
{
    auto l_action_ptr = new QAction{ QString("show %1").arg(m_title) };

    l_action_ptr->setCheckable(true);
    l_action_ptr->setChecked(true);

    connect(l_action_ptr, &QAction::triggered, [this](bool p_visible)
    {
        this->setVisible(p_visible);
    });

    connect(this, &QDockWidget::visibilityChanged, [l_action_ptr](bool p_visible)
    {
        l_action_ptr->setChecked(p_visible);
    });

    return l_action_ptr;
}

QTableWidget* DataViewDock::create_table(QString p_column_title)
{
    auto p_table = new QTableWidget;

    p_table->setColumnCount(2);
    p_table->setShowGrid(true);
    p_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    p_table->setSortingEnabled(true);
    p_table->setHorizontalHeaderLabels(QString{"#;%1"}.arg(p_column_title).split(";"));

    p_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    p_table->horizontalHeader()->setMinimumSectionSize(1);
    p_table->horizontalHeader()->setStretchLastSection(true);

    p_table->verticalHeader()->hide();

    if(p_column_title.isEmpty())
    {
        p_table->horizontalHeader()->hide();
    }

    return p_table;
}
