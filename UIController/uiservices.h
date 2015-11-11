#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "libqmlapplication_global.h"
#include <QObject>
#include <QTimer>

class QMLAPPLICATIONSHARED_EXPORT UiServices : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool popupVisible READ popupVisible WRITE setPopupVisible NOTIFY popupVisibleChanged)
    Q_PROPERTY(QString popupText READ popupText WRITE setPopupText NOTIFY popupTextChanged)
    Q_PROPERTY(PopupType popupType READ popupType WRITE setPopupType NOTIFY popupTypeChanged)

public:
    Q_ENUMS(PopupType)
    enum PopupType {POP_INFO, POP_WARNING, POP_ERROR};

    explicit UiServices(QObject *parent = 0);

private:
    bool popupVisible()   const {return m_popupVisible;}
    QString popupText()   const {return m_popupText;}
    PopupType popupType() const {return m_popupType;}

    void setPopupVisible(bool visible = true)       {m_popupVisible = visible;   emit popupVisibleChanged();}
    void setPopupText(const QString &text)          {m_popupText = text;         emit popupTextChanged();}
    void setPopupType(const PopupType &type)        {m_popupType = type;         emit popupTypeChanged();}

signals:
    void popupVisibleChanged();
    void popupTextChanged();
    void popupTypeChanged();

public slots:
    void pop(const QString &message, const PopupType &type = POP_INFO);
    void unPop();

private:
    QTimer m_timer;

    bool m_popupVisible;
    QString m_popupText;
    PopupType m_popupType;
};

#endif // APPCONTROLLER_H
