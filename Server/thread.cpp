#include "thread.h"

Thread::Thread()
{

}


void Thread::run(int room_Number){
    qDebug()<< "스레드 시작!!";
    qDebug() << "방 번호: " << room_Number;
}
