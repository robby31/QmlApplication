#ifndef MARKUPBLOCK_H
#define MARKUPBLOCK_H

#include <QDebug>
#include <QString>
#include <QRegularExpression>
#include "analyzer.h"

enum class TYPE { DocType, Comment, Element };

class MarkupBlock : public QObject
{
    Q_OBJECT

public:
    explicit MarkupBlock(QObject *parent = Q_NULLPTR);
    explicit MarkupBlock(const QString &name, const QString &attributes, const QString &data, QObject *parent = Q_NULLPTR);

    bool isValid() const;

    TYPE type() const;

    bool isEndTag() const;

    MarkupBlock *parentBlock() const;

    QString name() const;
    QHash<QString, QVariant> attributes() const;

    QString toString() const;

    void appendChild(MarkupBlock *block);
    QList<MarkupBlock*> children() const;
    QList<MarkupBlock*> findBlocks(const QString &name, const QHash<QString, QVariant> attributes = QHash<QString, QVariant>());

private:
    QString m_name;
    QString m_data;
    QHash<QString, QVariant> m_attributes;
    QList<MarkupBlock*> m_blocks;
};

#endif // MARKUPBLOCK_H
