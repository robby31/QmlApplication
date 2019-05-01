#include "markupdocument.h"

MarkupDocument::MarkupDocument(QObject *parent):
    QObject(parent)
{
    ANALYZER;
}

void MarkupDocument::setContent(const QString &data)
{
    ANALYZER;

    clear();
    m_data = data;
    parse_data();
}

void MarkupDocument::clear()
{
    qDeleteAll(m_blocks);
    m_blocks.clear();
    m_data.clear();
}

MarkupBlock *MarkupDocument::firstBlock() const
{
    ANALYZER;

    if (!m_blocks.isEmpty())
        return m_blocks.at(0);

    return Q_NULLPTR;
}

void MarkupDocument::parse_data()
{
    ANALYZER;

    QStringList singletonTag;
    singletonTag << "area" << "base" << "br" << "col" << "command" << "embed" << "hr" << "img" << "input" << "keygen" << "link" << "meta" << "param" << "source" << "track" << "wbr";

    MarkupBlock *lastBlock = Q_NULLPTR;

    // parse data
    QRegularExpression pattern(R"((<(?P<name>!--|[/!?]?[\w:]+)(?P<attributes>\s[^>]*)?>))");
    if (!pattern.isValid())
        qCritical() << pattern.errorString();

    QRegularExpressionMatchIterator iterator = pattern.globalMatch(m_data);
    int capturedIndex = -1;
    while (iterator.hasNext())
    {
        QRegularExpressionMatch match = iterator.next();
        qDebug() << match.capturedStart() << match.capturedEnd() << match.captured("name") << match.captured("attributes") << match.captured(0) << m_data.mid(match.capturedStart(), match.capturedLength());

        if (capturedIndex != -1 && capturedIndex != match.capturedStart())
        {
            if (capturedIndex >= match.capturedStart())
            {
                qCritical() << "invalid block containing data" << capturedIndex << match.capturedStart() << m_data.mid(capturedIndex, match.capturedStart()-capturedIndex);
            }
            else
            {
                qDebug() << "DATA" << capturedIndex << match.capturedStart()-capturedIndex << m_data.mid(capturedIndex, match.capturedStart()-capturedIndex);

                if (lastBlock)
                    lastBlock->set_data(m_data.mid(capturedIndex, match.capturedStart()-capturedIndex));
                else
                    appendChild(m_data.mid(capturedIndex, match.capturedStart()-capturedIndex));
            }
        }

        MarkupBlock *block = Q_NULLPTR;

        if (!lastBlock)
        {
            block = appendChild(match.captured("name"), match.captured("attributes"), match.captured(0));
        }
        else
        {
            block = lastBlock->appendChild(match.captured("name"), match.captured("attributes"), match.captured(0));

            if (block && block->name().startsWith("/"))
            {
                if (lastBlock->name() == block->name().right(block->name().size()-1))
                    lastBlock = lastBlock->parentBlock();
                else
                    qCritical() << "invalid tag" << match.capturedStart() << block->toString() << "parent" << lastBlock->toString();
            }
        }

        if (block && !block->isEndTag() && block->type() == TYPE::Element)
            lastBlock = block;

        // some block cannot have parents
        if (lastBlock && singletonTag.contains(lastBlock->name()))
            lastBlock = lastBlock->parentBlock();

        capturedIndex = match.capturedEnd();
    }
}

MarkupBlock *MarkupDocument::appendChild(const QString &name, const QString &attributes, const QString &str_definition)
{
    ANALYZER;

    auto block = new MarkupBlock(name, attributes, str_definition);
    block->setParent(this);
    m_blocks.append(block);
    return block;
}

MarkupBlock *MarkupDocument::appendChild(const QString &data)
{
    ANALYZER;

    MarkupBlock *block = Q_NULLPTR;

    if (!data.trimmed().isEmpty())
    {
        block = new MarkupBlock(data.trimmed());
        block->setParent(this);
        m_blocks.append(block);
    }

    return block;
}

QList<MarkupBlock*> MarkupDocument::blocks() const
{
    ANALYZER;

    return m_blocks;
}

QList<MarkupBlock*> MarkupDocument::findBlocks(const QString &name) const
{
    ANALYZER;

    QList<MarkupBlock*> res;

    for (auto block : m_blocks)
    {
        if (block->name() == "html")
            res << block->findBlocks(name);
    }

    return res;
}

DOC_TYPE MarkupDocument::docType() const
{
    ANALYZER;

    MarkupBlock *block = firstBlock();
    if (block && block->type() == TYPE::DocType)
    {
        if (block->toString().toLower() == "<!doctype html>")
            return DOC_TYPE::HTML;

        if (block->toString().toLower().startsWith("<?xml"))
            return DOC_TYPE::XML;
    }

    return DOC_TYPE::UNKNOWN;
}

bool MarkupDocument::isValid() const
{
    ANALYZER;

    return docType() != DOC_TYPE::UNKNOWN;
}

QString MarkupDocument::toString() const
{
    return m_data;
}
