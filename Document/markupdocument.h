#ifndef MARKUPDOCUMENT_H
#define MARKUPDOCUMENT_H

#include <QDebug>
#include <QRegularExpression>
#include "markupblock.h"

#include "analyzer.h"

enum class DOC_TYPE { UNKNOWN, HTML, XML };

class MarkupDocument : public QObject
{
    Q_OBJECT

public:
    explicit MarkupDocument(QObject *parent = Q_NULLPTR);

    void setContent(const QByteArray &data);
    void clear();

    DOC_TYPE docType() const;

    bool isValid() const;

    MarkupBlock *appendChild(const QString &name, const QString &attributes, const QString &str_definition);
    MarkupBlock *appendChild(const QString &data);

    MarkupBlock *firstBlock() const;
    QList<MarkupBlock*> blocks() const;
    QList<MarkupBlock*> findBlocks(const QString &name) const;

private:
    void parse_data();

private:
    QString m_data;
    QList<MarkupBlock*> m_blocks;
};

#endif // MARKUPDOCUMENT_H
