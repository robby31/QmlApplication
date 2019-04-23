#ifndef MARKUPDOCUMENT_H
#define MARKUPDOCUMENT_H

#include <QDebug>
#include <QRegularExpression>
#include "markupblock.h"

#include "analyzer.h"

enum class DOC_TYPE { UNKNOWN, HTML };

class MarkupDocument : public QObject
{
    Q_OBJECT

public:
    explicit MarkupDocument(QObject *parent = Q_NULLPTR);

    void setContent(const QByteArray &data);

    DOC_TYPE docType() const;

    bool isValid() const;

    MarkupBlock *firstBlock() const;
    QList<MarkupBlock*> blocks() const;
    QList<MarkupBlock*> findBlocks(const QString &name) const;

private:
    void parse_data();

private:
    QByteArray m_data;
    QList<MarkupBlock*> m_blocks;
};

#endif // MARKUPDOCUMENT_H
