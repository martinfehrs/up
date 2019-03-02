#pragma once

#include <QString>
#include <QDockWidget>

class QTableWidget;
class QAction;

class DataViewDock final : public QDockWidget
{
    Q_OBJECT

  public:

    DataViewDock(QWidget* p_parent, const char* p_title_ptr, const char* p_column_title_ptr);

    QTableWidget* table();

    QAction* CreateVisibilityToggleAction();

  private:

    QTableWidget* create_table(QString p_column_title);

    QString m_title;
    QTableWidget* m_table_ptr = nullptr;
};
