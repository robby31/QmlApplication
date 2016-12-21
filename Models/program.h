#ifndef PROGRAM_H
#define PROGRAM_H

#include <QDebug>
#include "Models/listmodel.h"

class Program : public ListItem
{
    Q_OBJECT

public:
  enum Roles {
    IdRole = Qt::UserRole+1,
    NameRole,
    ImageRole,      
    DescriptionRole
  };

public:
    Program(QObject *parent = 0): ListItem(parent) {}
    explicit Program(const QString &name, QObject *parent = 0);
    
    virtual QVariant data(int role) const;
    virtual bool setData(const QVariant &value, const int &role);
    QHash<int, QByteArray> roleNames() const;

    QString name()          const {return m_name;}
    QString image()         const {return m_image;}
    QString description()   const {return m_description;}

    void setName(const QString &name)               {m_name = name;}
    void setImage(const QString &image)             {m_image = image;}
    void setDescription(const QString &description) {m_description = description;}

signals:
    
public slots:
    
private:
    QString m_name;
    QString m_image;
    QString m_description;
};

#endif // SONGITEM_H
