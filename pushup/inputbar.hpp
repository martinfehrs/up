#pragma once

#include <deque>

#include <QFrame>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QStackedWidget>
#include <QToolButton>
#include <QPlainTextEdit>
#include <QStyleOption>
#include <QPainter>
#include <QAction>

class Highlighter;

enum class LineMode
{
    SingleLineMode,
    MultilineMode
};

class InputBar : public QFrame
{
    Q_OBJECT

  public:

    InputBar(QWidget* p_parent);

    void set_line_mode(LineMode p_line_mode);

    QString text() const;
    QString text(QString p_input);

  private:

    void history_push(const QString& p_entry);
    void history_up();
    void history_down();

    bool process_keys(const QKeyEvent* p_key_press_event_ptr);

    bool eventFilter(QObject* p_watched_ptr, QEvent* p_event_ptr) override;
    virtual void paintEvent(QPaintEvent* /*p_paint_event_ptr*/) override;
    virtual void showEvent(QShowEvent* p_show_event_ptr) override;

    void switch_line_mode();
    void accept_input();

    QTextEdit* m_multiline_input_ptr = nullptr;
    QToolButton* m_line_mode_button_ptr = nullptr;
    QBoxLayout* m_left_button_bar_layout_ptr = nullptr;
    QBoxLayout* m_right_button_bar_layout_ptr = nullptr;
    Highlighter* m_highlighter_ptr = nullptr;
    int m_history_index = -1;
    int m_initial_height = 0;
    std::deque<QString> m_history;
    LineMode m_line_mode = LineMode::SingleLineMode;

  signals:

    void input_accepted(QString p_str);
    void line_mode_changed(LineMode p_line_mode);

};
