#ifndef MARKUPBLOCK_H
#define MARKUPBLOCK_H

#include <QDebug>
#include <QString>
#include <QRegularExpression>
#include "analyzer.h"

enum class TYPE { DocType, Comment, Element, Data };

class MarkupBlock : public QObject
{
    Q_OBJECT

public:
    explicit MarkupBlock(QObject *parent = Q_NULLPTR);
    explicit MarkupBlock(const QString &name, const QString &attributes, const QString &str_definition, QObject *parent = Q_NULLPTR);
    explicit MarkupBlock(const QString &data, QObject *parent = Q_NULLPTR);

    bool isValid() const;

    TYPE type() const;

    bool isEndTag() const;

    MarkupBlock *parentBlock() const;

    QString name() const;
    QHash<QString, QVariant> attributes() const;

    void set_data(const QString &data);

    QString toString() const;

    int index();
    void appendChild(MarkupBlock *block);
    MarkupBlock *appendChild(const QString &name, const QString &attributes, const QString &str_definition);
    QList<MarkupBlock*> blocks() const;
    QList<MarkupBlock*> findBlocks(const QString &name,  const QHash<QString, QVariant> &attributes = QHash<QString, QVariant>());

    /* Find unique block with tag <name> */
    MarkupBlock *findBlock(const QString &name,  const QHash<QString, QVariant> &attributes = QHash<QString, QVariant>());

private:
    QString m_name;
    QString m_definition;
    QHash<QString, QVariant> m_attributes;
    QList<MarkupBlock*> m_blocks;
};

#endif // MARKUPBLOCK_H
