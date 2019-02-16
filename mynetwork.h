#ifndef MYNETWORK_H
#define MYNETWORK_H

#include <QNetworkAccessManager>
#include <QThread>

class MyNetwork
{
public:
    MyNetwork(MyNetwork const&) = delete;
    void operator=(MyNetwork const&)  = delete;

    static QNetworkAccessManager &manager()
    {
        static QHash<QString, QNetworkAccessManager*> thread_manager;

        QString current_thread_pointer = QString().sprintf("%8p", static_cast<void*>(QThread::currentThread()));

        if (thread_manager.contains(current_thread_pointer))
            return *thread_manager[current_thread_pointer];

        auto manager = new QNetworkAccessManager();
        thread_manager[current_thread_pointer] = manager;
        return *manager;
    }

private:
    MyNetwork() = default;
};

#endif // MYNETWORK_H
