#ifndef CONNECTEDCLIENT_H
#define CONNECTEDCLIENT_H



#include"pch_connectedclient.h"
#include"room.h"

using namespace std;

class MainServer;

class ConnectedClient
{
public:
    enum State {Idle, Searching, Join, Expired};
    ConnectedClient();
    ~ConnectedClient();

    // 초기화 함수
    void Init(MainServer* mainServer, int fd, sockaddr_in6* addr, std::function<void(ConnectedClient*)> endConnectionCallback);

    // 시작 함수
    void Run();

    // 클라이언트에게 데이터를 보내는 함수
    void Send(const char* data, int dataSize);
    void Send(const string& data);

    // 세션이 만료되었는지 확인하는 함수
    bool isSessionExpired();

    // 방에 들어왔다는 것을 알리는 함수
    void EnterRoom(Room* room);

    // 방에 나갔다는 것을 알리는 함수
    void ExitRoom();

    // 현재 상태를 가져오는 함수
    State GetState();

    // 현재 상태를 변경하는 함수
    void SetState(State state);

    // 세션을 가져오는 함수
    string GetSession();

private:

    MainServer* mainSever;
    Room* room;

    // 클라이언트로부터 데이터를 받는 함수
    void Receive();
    // 내용을 분석하는 함수
    void Process(const char* buf, int bufSize);

    //void(MainServer::*endConnectionCallback)(ConnectedClient*);
    std::function<void(ConnectedClient*)> endConnectionCallback;

    string session;
    time_t lastSessionTime;

    int clientFd;
    sockaddr_in6* clientAddr;

    thread* t;

    State state;
};

#endif // CONNECTEDCLIENT_H
