#ifndef ROOM_H
#define ROOM_H



#pragma once
#include "protocol.h"

class MainServer;
class ConnectedClient;

using namespace std;

class Room
{
public:
	Room(MainServer* mainServer, ConnectedClient* client1, ConnectedClient* client2);
	~Room();

	// 메시지 보냄
	void SendMessage(const wstring& message, const ConnectedClient* client);

	// 방을 나감
	void Exit(const ConnectedClient* client);

private:
	MainServer* mainServer;

	ConnectedClient* c1;
	ConnectedClient* c2;
};

#endif // ROOM_H
