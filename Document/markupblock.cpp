#include "markupblock.h"

MarkupBlock::MarkupBlock(QObject *parent):
    QObject(parent)
{
    ANALYZER;
    DebugInfo::add_object(this);
}

MarkupBlock::MarkupBlock(const QString &name, const QString &attributes, const QString &str_definition, QObject *parent):
    QObject(parent),
    m_name(name),
    m_definition(str_definition)
{
    ANALYZER;

    DebugInfo::add_object(this);

    // parse attributes
    QRegularExpression pattern(R"((?P<param>[^=]+)\s*=\s*["'](?P<value>[^"']+)["']?)");
    QRegularExpressionMatchIterator iterator = pattern.globalMatch(attributes.trimmed());
    while (iterator.hasNext())
    {
        QRegularExpressionMatch match = iterator.next();
        m_attributes[match.captured("param").trimmed()] = match.captured("value").trimmed();
    }
}

MarkupBlock::MarkupBlock(const QString &data, QObject *parent):
    QObject(parent),
    m_definition(data)
{
    ANALYZER;
    DebugInfo::add_object(this);
}

bool MarkupBlock::isValid() const
{
    ANALYZER;

    return !m_name.isEmpty() && !m_definition.isEmpty();
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

    return m_definition;
}

void MarkupBlock::appendChild(MarkupBlock *block)
{
    ANALYZER;

    m_blocks.append(block);
    block->setParent(this);
    connect(block, &MarkupBlock::destroyed, this, &MarkupBlock::blockDestroyed);

    #if !defined(QT_NO_DEBUG_OUTPUT)
    qDebug() << name() << "APPEND CHILD" << block->name();
    #endif
}

MarkupBlock *MarkupBlock::appendChild(const QString &name, const QString &attributes, const QString &str_definition)
{
    ANALYZER;

    auto block = new MarkupBlock(name, attributes, str_definition, this);
    appendChild(block);
    return block;
}

QList<MarkupBlock*> MarkupBlock::blocks() const
{
    ANALYZER;

    return m_blocks;
}

MarkupBlock *MarkupBlock::parentBlock() const
{
    ANALYZER;

    return qobject_cast<MarkupBlock*>(parent());
}

QList<MarkupBlock*> MarkupBlock::findBlocks(const QString &name, const QHash<QString, QVariant> &attributes)
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

    for (auto child : m_blocks)
        res << child->findBlocks(name, attributes);

    return res;
}

MarkupBlock *MarkupBlock::findBlock(const QString &name, const QHash<QString, QVariant> &attributes)
{
    ANALYZER;

    QList<MarkupBlock*> res = findBlocks(name, attributes);
    if (res.size() == 1)
        return res.at(0);

    return Q_NULLPTR;
}

TYPE MarkupBlock::type() const
{
    ANALYZER;

    if (m_name.toLower() == "!doctype")
        return TYPE::DocType;

    if (m_name.startsWith("?xml", Qt::CaseInsensitive))
        return TYPE::DocType;

    if (m_name.startsWith("!--", Qt::CaseInsensitive))
        return TYPE::Comment;

    if (!m_name.isEmpty())
        return TYPE::Element;

    return TYPE::Data;
}

bool MarkupBlock::isEndTag() const
{
    ANALYZER;

    return m_name.startsWith("/") or m_definition.endsWith("/>");
}

int MarkupBlock::index()
{
    MarkupBlock *parent = parentBlock();
    if (parent)
        return parent->blocks().indexOf(this);

    return 0;
}

void MarkupBlock::set_data(const QString &data)
{
    if (!data.trimmed().isEmpty())
    {
        auto block = new MarkupBlock(data.trimmed());
        appendChild(block);
    }
}

void MarkupBlock::blockDestroyed(QObject *block)
{
    QList<MarkupBlock*>::Iterator it = m_blocks.begin();
    while (it != m_blocks.end())
    {
        if (*it == block)
            it = m_blocks.erase(it);
        else
            it++;
    }
}
