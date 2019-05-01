#include "markupdocmodel.h"

MarkupDocModel::MarkupDocModel(QObject *parent):
    QAbstractItemModel(parent)
{
    connect(this, &MarkupDocModel::urlChanged, this, &MarkupDocModel::loadUrl);
}

QModelIndex MarkupDocModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid())
        return createIndex(row, column, const_cast<MarkupDocument*>(&m_doc));  // ROOT

    if (parent.internalPointer() == &m_doc && row >= 0 && row < m_doc.blocks().size())
        return createIndex(row, column, const_cast<MarkupBlock*>(m_doc.blocks().at(row)));

    auto block = static_cast<MarkupBlock*>(parent.internalPointer());
    if (block && row >=0 && row < block->blocks().size())
        return createIndex(row, column, const_cast<MarkupBlock*>(block->blocks().at(row)));

    qCritical() << "index not found" << row << column << "parent" << parent;
    return {};
}

QModelIndex MarkupDocModel::parent(const QModelIndex &index) const
{
    if (index.internalPointer() == &m_doc)
    {
        return {};
    }

    auto block = static_cast<MarkupBlock*>(index.internalPointer());
    if (block)
    {
        if (!block->parentBlock())
            return createIndex(0, 0, const_cast<MarkupDocument*>(&m_doc));  // ROOT

        auto parent_block = block->parentBlock();
        if (parent_block)
            return createIndex(parent_block->index(), 0, parent_block);
    }

    qCritical() << "parent not found" << index << index.internalPointer();
    return {};
}

int MarkupDocModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 1; // root item

    if (parent.internalPointer() == &m_doc)
        return m_doc.blocks().size();

    auto block = static_cast<MarkupBlock*>(parent.internalPointer());
    if (block)
        return block->blocks().size();

    qCritical() << "invalid rowCount" << parent;
    return 0;
}

int MarkupDocModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant MarkupDocModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.internalPointer())
    {
        if (index.internalPointer() == &m_doc)
            return m_url.url();

        auto block = static_cast<MarkupBlock*>(index.internalPointer());
        if (block)
            return block->toString();
    }

    qCritical() << "INVALID data" << index << index.parent() << role;
    return QVariant();
}

QUrl MarkupDocModel::url() const
{
    return m_url;
}

void MarkupDocModel::setUrl(const QUrl &url)
{
    m_url = url;
    emit urlChanged();
}

void MarkupDocModel::setContent(const QString &data)
{
    beginResetModel();
    m_doc.setContent(data);
    endResetModel();

    if (!m_doc.isValid())
        qCritical() << "invalid format" << m_url;
}

void MarkupDocModel::loadUrl()
{
    if (m_url.isLocalFile())
    {
        QFile file(m_url.toLocalFile());
        if (!file.open(QFile::ReadOnly))
        {
            qCritical() << "unable to load file" << m_url;
            return;
        }

        setContent(file.readAll());
    }
    else
    {
        QNetworkRequest request(m_url);
        QNetworkReply *reply = MyNetwork::manager().get(request);
        connect(reply, &QNetworkReply::finished, this, &MarkupDocModel::replyFinished);
    }
}

void MarkupDocModel::replyFinished()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply)
    {
        qCritical() << "invalid reply" << reply;
    }
    else
    {
        setContent(reply->readAll());
        reply->deleteLater();
    }
}

QString MarkupDocModel::xmlString() const
{
    return m_doc.toString();
}

void MarkupDocModel::setXmlString(const QString &data)
{
    setContent(data);
}

