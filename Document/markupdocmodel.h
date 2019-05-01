#ifndef MARKUPDOCMODEL_H
#define MARKUPDOCMODEL_H

#include <QAbstractItemModel>
#include <QUrl>
#include <QFile>
#include "mynetwork.h"
#include <QNetworkReply>

#include "Document/markupdocument.h"

class MarkupDocModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString xmlString READ xmlString WRITE setXmlString NOTIFY xmlStringChanged)

public:
    explicit MarkupDocModel(QObject *parent = Q_NULLPTR);


    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QUrl url() const;
    void setUrl(const QUrl &url);

    QString xmlString() const;
    void setXmlString(const QString &data);

private:
    void setContent(const QString &data);

signals:
    void urlChanged();
    void xmlStringChanged();

private slots:
    void loadUrl();
    void replyFinished();

private:
    QUrl m_url;
    MarkupDocument m_doc;
};

#endif // MARKUPDOCMODEL_H
