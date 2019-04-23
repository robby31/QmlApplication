#include "markupdocument.h"

MarkupDocument::MarkupDocument(QObject *parent):
    QObject(parent)
{
    ANALYZER;
}

void MarkupDocument::setContent(const QByteArray &data)
{
    ANALYZER;

    m_data = data;
    parse_data();
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

    QStringList blockNameWithNoParent;
    blockNameWithNoParent << "link" << "br" << "hr" << "img" << "input";

    MarkupBlock *lastBlock = Q_NULLPTR;

    // parse data
    QRegularExpression pattern(R"(<((?P<name>[/!]?\w+)(?P<attributes>\s[^>]*)?)>)");
    QRegularExpressionMatchIterator iterator = pattern.globalMatch(m_data);
    while (iterator.hasNext())
    {
        QRegularExpressionMatch match = iterator.next();
        qDebug() << match.capturedStart() << match.captured("name") << match.captured("attributes");

        MarkupBlock *block = new MarkupBlock(match.captured("name"), match.captured("attributes"), match.captured(0));

        if (!lastBlock)
        {
            m_blocks.append(block);
            block->setParent(this);
        }
        else
        {
            lastBlock->appendChild(block);

            if (block->name().startsWith("/"))
            {
                if (lastBlock->name() == block->name().right(block->name().size()-1))
                    lastBlock = lastBlock->parentBlock();
                else
                    qCritical() << "invalid tag" << match.capturedStart() << block->toString() << "parent" << lastBlock->toString();
            }
        }

        if (!match.captured("name").startsWith("!") && !match.captured("name").startsWith("/") && !match.captured(0).endsWith("/>"))
            lastBlock = block;

        // some block cannot have parents
        if (lastBlock && blockNameWithNoParent.contains(lastBlock->name()))
            lastBlock = lastBlock->parentBlock();
    }
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
        if (block->toString() == "<!doctype html>")
            return DOC_TYPE::HTML;
    }

    return DOC_TYPE::UNKNOWN;
}

bool MarkupDocument::isValid() const
{
    ANALYZER;

    return docType() != DOC_TYPE::UNKNOWN;
}
