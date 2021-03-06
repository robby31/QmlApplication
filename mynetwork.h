#ifndef MYNETWORK_H
#define MYNETWORK_H

#include <QNetworkAccessManager>
#include <QThread>

class MyNetwork
{
public:
    MyNetwork(MyNetwork const&) = delete;
    ~MyNetwork() = delete;
    MyNetwork& operator =(MyNetwork const&) = delete;
    MyNetwork(MyNetwork&&) = delete;
    MyNetwork& operator=(MyNetwork&&) = delete;

    static QNetworkAccessManager &manager()
    {
        static QHash<QString, QNetworkAccessManager*> thread_manager;

        QString current_thread_pointer = QString::asprintf("%8p", static_cast<void*>(QThread::currentThread()));

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
