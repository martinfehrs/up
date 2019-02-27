#include <QHideEvent>

#include "highlighter.hpp"

const QColor  NUMBER_COLOR  { 80, 80, 255 };
const QColor  COMMENT_COLOR { 80, 80, 80  };
const QColor  STRING_COLOR  { 200, 69, 0  };
const QString KEYWORDS[]    { ":", ";", "IF", "THEN", "ELSE", "DO", "LOOP", "+LOOP", "BEGIN", "UNTIL", "REPEAT" };

Highlighter::Highlighter(QTextDocument* p_doc_ptr)
    : QSyntaxHighlighter{ p_doc_ptr }
{
    m_highlighting_rules.emplace_back("(?<!\\S)-?[0-9]{1}[0-9.,/']*([Ee]?(-?[0-9]{1}[0-9.,/']*)?)(?!\\S)", NUMBER_COLOR);
    m_highlighting_rules.emplace_back("(?<!\\S)\\({1}\\s+.*\\){1}(?!\\S)", COMMENT_COLOR, QFont::Normal, true);
    m_highlighting_rules.emplace_back("[\"]{1}\\s+.+[\"]{1}", STRING_COLOR);

    for(const auto& l_keyword : KEYWORDS)
    {
        m_highlighting_rules.emplace_back("(?<!\\S)" + l_keyword.toUpper() + "(?!\\S)", Qt::black, QFont::Bold);
        m_highlighting_rules.emplace_back("(?<!\\S)" + l_keyword.toLower() + "(?!\\S)", Qt::black, QFont::Bold);
    }
}

void Highlighter::highlightBlock(const QString &p_text)
{
    for (const auto &l_highlighting_rule : m_highlighting_rules)
    {
        auto l_match_iterator = l_highlighting_rule.m_pattern.globalMatch(p_text);

        while (l_match_iterator.hasNext())
        {
            auto l_match = l_match_iterator.next();

            setFormat(l_match.capturedStart(), l_match.capturedLength(), l_highlighting_rule.m_format);
        }
    }
}
