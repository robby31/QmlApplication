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

    void setContent(const QString &data);
    void clear();

    void setSingletonTag(const QStringList &tagList);

    DOC_TYPE docType() const;

    bool isValid() const;

    QString toString() const;

    MarkupBlock *appendChild(const QString &name, const QString &attributes, const QString &str_definition);
    MarkupBlock *appendChild(const QString &data);

    MarkupBlock *firstBlock() const;
    QList<MarkupBlock*> blocks() const;
    QList<MarkupBlock*> findBlocks(const QString &name) const;

private:
    void parse_data();

private slots:
    void blockDestroyed(QObject *block);

private:
    QString m_data;
    QList<MarkupBlock*> m_blocks;
    QStringList m_singletonTag;
};

#endif // MARKUPDOCUMENT_H
