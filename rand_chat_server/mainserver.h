#ifndef MAINSERVER_H
#define MAINSERVER_H



#include <list>
#include <thread>
#include <queue>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "connectedclient.h"
#include "room.h"

using namespace std; 

class MainServer
{
public:
    MainServer();
    ~MainServer();
    
    void Init(unsigned int port);
    bool Run();
    
        // serachingQueue에다가 클라이언트를 넣는 함수
        void AddClientInSearchingQueue(ConnectedClient* client);
    
        // Room을 list에서 지우는 함수
        void RemoveRoomFromList(Room* room);
    
    private:
        // 세션을 체크하는 함수
        void CheckSession();
    
        // 방을 만들어야하는지 체크하는 함수
        void CheckMakingRoom();
    
        // Client의 접속이 종료되었음을 알리는 Callback 함수
        void EndConnection_Callback(ConnectedClient* client);
    
        // Client를 list에서 지우는 함수
        void RemoveClientFromList(ConnectedClient* client);
    
        list<shared_ptr<ConnectedClient>> connectedClients;
        list<unique_ptr<Room>> rooms;
        thread* sessionCheckThread;
        thread* checkMakingRoomThread;
    
        unsigned int port;
        int serverFd, clientFd;
        sockaddr_in6 serverAddr;
    
        char buffer[256];
    
    queue<ConnectedClient*> searchingQueue;
};

#endif // MAINSERVER_H
