#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QObject>
#include <QDebug>

class Thread : public QThread
{
public:
    Thread();
    void run(int n);
private:

};

#endif // THREAD_H
