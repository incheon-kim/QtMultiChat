#include "room.h"
#include"mainserver.h"
#include"connectedclient.h"


Room::Room(MainServer* mainServer, ConnectedClient* client1, ConnectedClient* client2)
{
	this->mainServer = mainServer;
	c1 = client1;
	c2 = client2;

	c1->SetState(ConnectedClient::State::Join);
	c2->SetState(ConnectedClient::State::Join);

	c1->EnterRoom(this);
	c2->EnterRoom(this);

	// 각 클라이언트들에게 찾았다고 데이터 보냄
	Protocol_FindUserResult c1_result;
	c1_result.resultCode = ResultCode::Sucess;
	c1_result.userSession = c2->GetSession();
	c1->Send(Protocol_FindUserResult::ToJson(c1_result));

	Protocol_FindUserResult c2_result;
	c2_result.resultCode = ResultCode::Sucess;
	c2_result.userSession = c1->GetSession();
	c2->Send(Protocol_FindUserResult::ToJson(c2_result));
}


Room::~Room()
{
}

// 메시지 보냄
void Room::SendMessage(const wstring& message, const ConnectedClient* client)
{
	// 보낸 Client에게는 SendMessageResult를, 그 외는 GetMessage를 보내줌
	if(client == c1) {
		Protocol_SendMessageResult r;
		r.resultCode = ResultCode::Sucess;
		c1->Send(Protocol_SendMessageResult::ToJson(r));

		Protocol_GetMessage p;
		p.message = message;
		p.sendUserSession = c1->GetSession();
		c2->Send(Protocol_GetMessage::ToJson(p));
	} else if(client == c2) {
		Protocol_SendMessageResult r;
		r.resultCode = ResultCode::Sucess;
		c2->Send(Protocol_SendMessageResult::ToJson(r));

		Protocol_GetMessage p;
		p.message = message;
		p.sendUserSession = c2->GetSession();
		c1->Send(Protocol_GetMessage::ToJson(p));
	}
}

// 방을 나감
void Room::Exit(const ConnectedClient* client)
{
	c1->SetState(ConnectedClient::State::Idle);
	c2->SetState(ConnectedClient::State::Idle);
	c1->ExitRoom();
	c2->ExitRoom();

	// 상대방에게 방을 나갔다고 알림
	Protocol_UserLeaved p;
	if(client == c1) {
		p.leavedUserSession = c1->GetSession();
		c2->Send(Protocol_UserLeaved::ToJson(p));
	} else if(client == c2) {
		p.leavedUserSession = c2->GetSession();
		c1->Send(Protocol_UserLeaved::ToJson(p));
	}

	// MainServer에게 방을 지우라고 함
	mainServer->RemoveRoomFromList(this);
}
