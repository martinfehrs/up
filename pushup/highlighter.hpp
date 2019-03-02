#pragma once

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:

    Highlighter(QTextDocument* p_doc_ptr = nullptr);

protected:

    void highlightBlock(const QString& p_text) override;

private:

    struct HighlightingRule
    {
        HighlightingRule(QString p_pattern, QColor p_color, int p_weight = QFont::Normal, bool p_italic = false)
            : m_pattern{ std::move(p_pattern) }
        {
            m_format.setForeground(std::move(p_color));
            m_format.setFontWeight(p_weight);
            m_format.setFontItalic(p_italic);
        }

        QRegularExpression m_pattern;
        QTextCharFormat m_format;
    };

    std::vector<HighlightingRule> m_highlighting_rules;

    QRegularExpression m_commentStartExpression;
    QRegularExpression m_commentEndExpression;

    QTextCharFormat m_numberFormat;
    QTextCharFormat m_stringFormat;
};
