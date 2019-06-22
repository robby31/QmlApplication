#include "debuginfoitem.h"

DebugInfoItem::DebugInfoItem(QObject *parent):
    ListItem(parent)
{

}

QString DebugInfoItem::id() const
{
    return m_name;
}

 QHash<int, QByteArray> DebugInfoItem::roleNames() const
 {
      QHash<int, QByteArray> res;
      res[NameRole] = "name";
      res[CounterRole] = "counter";
      return res;
 }

 QVariant DebugInfoItem::data(int role) const
 {
     if (role == NameRole)
         return m_name;

     if (role == CounterRole)
        return m_counter;

     return QVariant();
 }

 bool DebugInfoItem::setData(const QVariant &value, const int &role)
 {
     if (role == NameRole)
     {
         m_name = value.toString();
         return true;
     }

     if (role == CounterRole)
     {
         m_counter = value.toInt();
         return true;
     }

     return false;
 }
