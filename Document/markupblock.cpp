#include "markupblock.h"

MarkupBlock::MarkupBlock(QObject *parent):
    QObject(parent)
{
    ANALYZER;
}

MarkupBlock::MarkupBlock(const QString &name, const QString &attributes, const QString &data, QObject *parent):
    QObject(parent),
    m_name(name),
    m_data(data)
{
    ANALYZER;

    // parse attributes
    QRegularExpression pattern(R"((?P<param>[^=]+)\s*=\s*\\?"(?P<value>[^\\"]+)\\?")");
    QRegularExpressionMatchIterator iterator = pattern.globalMatch(attributes.trimmed());
    while (iterator.hasNext())
    {
        QRegularExpressionMatch match = iterator.next();
        m_attributes[match.captured("param").trimmed()] = match.captured("value").trimmed();
    }
}

bool MarkupBlock::isValid() const
{
    ANALYZER;

    return !m_name.isEmpty() && !m_data.isEmpty();
}

QString MarkupBlock::name() const
{
    ANALYZER;

    return m_name;
}

QHash<QString, QVariant> MarkupBlock::attributes() const
{
    return m_attributes;
}

QString MarkupBlock::toString() const
{
    ANALYZER;

    return m_data;
}

void MarkupBlock::appendChild(MarkupBlock *block)
{
    ANALYZER;

    m_blocks.append(block);
    block->setParent(this);
    qDebug() << name() << "APPEND CHILD" << block->name();
}

QList<MarkupBlock*> MarkupBlock::children() const
{
    ANALYZER;

    return m_blocks;
}

MarkupBlock *MarkupBlock::parentBlock() const
{
    ANALYZER;

    return qobject_cast<MarkupBlock*>(parent());
}

QList<MarkupBlock*> MarkupBlock::findBlocks(const QString &name, const QHash<QString, QVariant> attributes)
{
    ANALYZER;

    QList<MarkupBlock*> res;

    if (m_name == name)
    {
        bool match = true;
        QHash<QString, QVariant>::const_iterator iterator;
        for (iterator = attributes.constBegin(); iterator != attributes.constEnd(); ++iterator)
        {
            if (!m_attributes.contains(iterator.key()))
            {
                match = false;
                break;
            }

            if (m_attributes[iterator.key()] != iterator.value())
            {
                match = false;
                break;
            }
        }

        if (match)
            res << this;
    }

    for (auto child : children())
        res << child->findBlocks(name, attributes);

    return res;
}

TYPE MarkupBlock::type() const
{
    ANALYZER;

    if (m_name == "!doctype")
        return TYPE::DocType;

    if (m_name.startsWith("!--"))
        return TYPE::Comment;

    return TYPE::Element;
}

bool MarkupBlock::isEndTag() const
{
    ANALYZER;

    return m_name.startsWith("/") or m_data.endsWith("/>");
}
