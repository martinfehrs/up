#include <cassert>

#include "inputbar.hpp"
#include "highlighter.hpp"

static bool is_newline(QChar c)
{
    return c == '\n';
}

static QTextEdit* create_multiline_input()
{
    auto l_multiline_input_ptr = new QTextEdit;

    l_multiline_input_ptr = new QTextEdit;
    l_multiline_input_ptr->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
    l_multiline_input_ptr->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    l_multiline_input_ptr->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    l_multiline_input_ptr->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    l_multiline_input_ptr->setPlaceholderText("type here...");

    return l_multiline_input_ptr;
}

static QToolButton* create_input_button(QString p_icon_path, QString p_tooltip_text)
{
    auto l_run_button_ptr = new QToolButton;

    l_run_button_ptr->setIcon(QIcon{ std::move(p_icon_path) });
    l_run_button_ptr->setProperty("class", "input-button");
    l_run_button_ptr->setToolTip(std::move(p_tooltip_text));
    l_run_button_ptr->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    return l_run_button_ptr;
}

InputBar::InputBar(QWidget* p_parent)
    : QFrame{ p_parent }
{
    m_multiline_input_ptr = create_multiline_input();
    m_multiline_input_ptr->installEventFilter(this);
    //m_multiline_input_ptr->connect(m_multiline_input_ptr, &QTextEdit::textChanged, [this]() { text(m_multiline_input_ptr->toPlainText()); });

    m_highlighter_ptr = new Highlighter();
    m_highlighter_ptr->setDocument(m_multiline_input_ptr->document());

    const auto l_run_button_ptr = create_input_button(":/neutral/icons/run.png", "run and clear input");
    connect(l_run_button_ptr, &QToolButton::clicked, this, &InputBar::accept_input);

    const auto l_clear_button_ptr = create_input_button(":/neutral/icons/clear.png", "clear input");
    connect(l_clear_button_ptr, &QToolButton::clicked, [this](){ text(""); });

    m_line_mode_button_ptr = create_input_button(":/neutral/icons/multiline.png", "switch to multiline mode");
    connect(m_line_mode_button_ptr, &QToolButton::clicked, this, &InputBar::switch_line_mode);

    m_left_button_bar_layout_ptr = new QBoxLayout{ QBoxLayout::LeftToRight };
    m_left_button_bar_layout_ptr->addWidget(l_run_button_ptr, 0, Qt::AlignTop);
    m_left_button_bar_layout_ptr->addStretch();

    m_right_button_bar_layout_ptr = new QBoxLayout{ QBoxLayout::LeftToRight };
    m_right_button_bar_layout_ptr->addWidget(l_clear_button_ptr, 0, Qt::AlignTop);
    m_right_button_bar_layout_ptr->addWidget(m_line_mode_button_ptr, 0, Qt::AlignTop);
    m_right_button_bar_layout_ptr->addStretch();

    auto l_vertical_input_layout = new QVBoxLayout;
    l_vertical_input_layout->addWidget(m_multiline_input_ptr);

    auto l_layout_ptr = new QHBoxLayout{ this };

    l_layout_ptr->setSpacing(0);
    l_layout_ptr->setMargin(0);

    l_layout_ptr->addLayout(m_left_button_bar_layout_ptr);
    l_layout_ptr->addLayout(l_vertical_input_layout, 1);
    l_layout_ptr->addLayout(m_right_button_bar_layout_ptr);

    setLayout(l_layout_ptr);

    set_line_mode(LineMode::SingleLineMode);
}

void InputBar::set_line_mode(LineMode p_line_mode)
{
    m_line_mode = p_line_mode;

    if(p_line_mode == LineMode::SingleLineMode)
    {
        const auto lines = m_multiline_input_ptr->toPlainText().split("\n");

        if(!lines.empty())
        {
            m_multiline_input_ptr->setText(lines[0]);
        }

        m_multiline_input_ptr->setLineWrapMode(QTextEdit::NoWrap);
        m_multiline_input_ptr->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_line_mode_button_ptr->setIcon(QIcon{ ":/neutral/icons/multiline.png" });
        m_line_mode_button_ptr->setToolTip("switch to multiline mode");
        m_right_button_bar_layout_ptr->setDirection(QBoxLayout::LeftToRight);
    }
    else
    if(p_line_mode == LineMode::MultilineMode)
    {

        m_multiline_input_ptr->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_multiline_input_ptr->setLineWrapMode(QTextEdit::WidgetWidth);
        m_line_mode_button_ptr->setIcon(QIcon{ ":/neutral/icons/singleline.png" });
        m_line_mode_button_ptr->setToolTip("switch to single line mode");
        m_right_button_bar_layout_ptr->setDirection(QBoxLayout::TopToBottom);
    }

    emit line_mode_changed(p_line_mode);
}

void InputBar::paintEvent(QPaintEvent* /*p_paint_event_ptr*/)
{
    QStyleOption opt;
    opt.init(this);

    QPainter painter{ this };
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void InputBar::showEvent(QShowEvent* p_show_event_ptr)
{
    m_multiline_input_ptr->setFocus();

    QWidget::showEvent(p_show_event_ptr);
}

void InputBar::switch_line_mode()
{
    if(m_line_mode == LineMode::MultilineMode)
    {
        set_line_mode(LineMode::SingleLineMode);
    }
    else
    if(m_line_mode == LineMode::SingleLineMode)
    {
        set_line_mode(LineMode::MultilineMode);
    }
}

void InputBar::history_push(const QString& p_entry)
{
    m_history_index = -1;

    if(m_history.empty() || m_history.front() != p_entry)
    {
        m_history.push_front(p_entry);

        if(m_history.size() > 100)
        {
            m_history.pop_back();
        }
    }
}

QString InputBar::text() const
{
    return m_multiline_input_ptr->toPlainText().trimmed();
}

QString InputBar::text(QString p_input)
{
    using namespace std;

    QString l_old_input = text();

    const auto l_trimmed_input = std::move(p_input).trimmed();

    if(m_line_mode == LineMode::SingleLineMode && any_of(begin(l_trimmed_input), end(l_trimmed_input), is_newline))
    {
        switch_line_mode();
    }

    m_multiline_input_ptr->setPlainText(l_trimmed_input);

    return l_old_input;
}

void InputBar::history_up()
{
    m_history_index++;
    text(m_history.at(m_history_index));
}

void InputBar::history_down()
{
    m_history_index--;
    text(m_history.at(m_history_index));
}

bool InputBar::process_keys(const QKeyEvent* p_key_press_event_ptr)
{
    const auto l_key = p_key_press_event_ptr->key();

    if(l_key == Qt::Key_Up && m_history_index < static_cast<int>(m_history.size()) - 1)
    {
        history_up();

        return true;
    }
    else
    if(l_key == Qt::Key_Down && m_history_index > 0)
    {
        history_down();

        return true;
    }
    else
    if(l_key == Qt::Key_Return && !text().isEmpty())
    {
        accept_input();

        return true;
    }
    else
    if(l_key == Qt::Key_Return && text().isEmpty())
    {
        return true;
    }

    return false;
}

bool InputBar::eventFilter(QObject* p_watched_ptr, QEvent* p_event_ptr)
{
    if(p_event_ptr->type() == QEvent::KeyPress)
    {
        const auto l_key_press_event_ptr = static_cast<QKeyEvent*>(p_event_ptr);

        if(m_line_mode == LineMode::SingleLineMode)
        {
            return process_keys(l_key_press_event_ptr);
        }
        else
        if(m_line_mode == LineMode::MultilineMode && l_key_press_event_ptr->modifiers().testFlag(Qt::ControlModifier))
        {
            return process_keys(l_key_press_event_ptr);
        }
    }

    return QWidget::eventFilter(p_watched_ptr, p_event_ptr);
}

void InputBar::accept_input()
{
    emit input_accepted(text());
    history_push(text(""));
}
