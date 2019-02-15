#ifndef MYNETWORK_H
#define MYNETWORK_H

#include <QNetworkAccessManager>

class MyNetwork
{
public:
    MyNetwork(MyNetwork const&) = delete;
    void operator=(MyNetwork const&)  = delete;

    static QNetworkAccessManager &manager()
    {
        static QNetworkAccessManager manager;
        return manager;
    }

private:
    MyNetwork() = default;
};

#endif // MYNETWORK_H
