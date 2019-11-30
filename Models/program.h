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
    Program(QObject *parent = Q_NULLPTR): ListItem(parent) {}
    explicit Program(const QString &name, QObject *parent = Q_NULLPTR);
    
    QVariant data(int role) const Q_DECL_OVERRIDE;
    bool setData(const QVariant &value, const int &role) Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    QString name()          const {return m_name;}
    QString image()         const {return m_image;}
    QString description()   const {return m_description;}

    void setName(const QString &name)               {m_name = name;}
    void setImage(const QString &image)             {m_image = image;}
    void setDescription(const QString &description) {m_description = description;}
    
private:
    QString m_name;
    QString m_image;
    QString m_description;
};

#endif // SONGITEM_H
