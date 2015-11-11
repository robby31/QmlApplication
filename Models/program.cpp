#include "program.h"
#include <QDebug>
Program::Program(const QString &name, QObject *parent):
    ListItem(parent),
    m_name(name),
    m_image(""),
    m_description("")
{
}

QHash<int, QByteArray> Program::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole]             = "id";
    roles[NameRole]           = "name";
    roles[ImageRole]          = "image";
    roles[DescriptionRole]    = "description";
    return roles;
}

QVariant Program::data(int role) const
{
    switch(role) {
    case IdRole:
        return QVariant::fromValue(id());
    case NameRole:
        return QVariant::fromValue(name());
    case ImageRole:
        return QVariant::fromValue(image());
    case DescriptionRole:
        return QVariant::fromValue(description());
    default:
        return QVariant();
    }
}

bool Program::setData(const QVariant &value, const int &role)
{
    switch (role)
    {
    case NameRole:
        setName(value.toString()); break;
    case ImageRole:
        setImage(value.toString()); break;
    case DescriptionRole:
        setDescription(value.toString()); break;
    default:
        return false;
    }
    emit itemChanged();
    return true;
}
