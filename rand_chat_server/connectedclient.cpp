#include "connectedclient.h"
#include"mainserver.h"

ConnectedClient::ConnectedClient()
{
}


ConnectedClient::~ConnectedClient()
{
}

// 초기화 함수
void ConnectedClient::Init(MainServer* mainServer, int fd, sockaddr_in6* addr, std::function<void(ConnectedClient*)> endConnectionCallback)
{
	this->mainSever = mainServer;
	clientFd = fd;
	clientAddr = addr;
	this->endConnectionCallback = endConnectionCallback;

	// 세션 id 생성 (uuid로 관리)
	boost::uuids::uuid u = boost::uuids::random_generator()();
	session = boost::uuids::to_string(u);

	state = State::Idle;
	room = NULL;
}

// 시작 함수
void ConnectedClient::Run()
{
	// 해당 클라이언트의 데이터 접속을 담당할 스레드 생성
	t = new thread(&ConnectedClient::Receive, this);

	// 마지막 세션 값 셋팅
	lastSessionTime = std::time(nullptr);

	// 클라이언트에게 Session값 보내줌
	Protocol_ConnectResult p;
	p.resultCode = ResultCode::Sucess;
	p.session = session;
	string j = p.ToJson(p);
	Send(j);
}
// 클라이언트로부터 데이터를 받는 함수
void ConnectedClient::Receive()
{
	char buffer[256];
	while(1) {
		// recv함수로 클라이언트로부터 오는 데이터 받음
		int receiveSize = (int)recv(clientFd, buffer, sizeof(buffer), 0);

		if(receiveSize <= 0) { // Connection 종료됨
			std::cout.flush();
			std::cout << "End connection. (" << session << ")" << endl;
			break;
		}

		// 받은 내용을 분석
		Process(buffer, receiveSize);
	}
	// 혹시 방에 들어가 있으면 나옴
	if(room != NULL) {
		room->Exit(this);
	}
	// 소켓 닫음
	close(clientFd);
	// 연결 종료 Callback함수 호출
	endConnectionCallback(this);
}

// 내용을 분석하는 함수
void ConnectedClient::Process(const char* buf, int bufSize)
{
	json j = json::parse(buf);

	string type = j["protocolType"];

	if(type == "Protocol_FindUser") { // 유저 찾기
		// 서칭큐에 넣어줌
		mainSever->AddClientInSearchingQueue(this);
	} else if(type == "Protocol_SendMessage") {
		// 들어간 Room에 메시지 날려줌
		Protocol_SendMessage p = Protocol_SendMessage::ToProtocol(buf);
		room->SendMessage(p.message, this);
	} else if(type == "Protocol_LeaveRoom") {
		// 들어간 Room에게 방을 나간다고 말해줌
		room->Exit(this);
	}
}

// 클라이언트에게 데이터를 보내는 함수
void ConnectedClient::Send(const char* data, int dataSize)
{
	send(clientFd, data, dataSize, 0);
	//delete data;
}
void ConnectedClient::Send(const string& string)
{
	Send(string.c_str(), (int)string.size() + 1);
}

// 세션이 만료되었는가?
bool ConnectedClient::isSessionExpired()
{
	/*
	time_t currentTime = std::time(nullptr);
	
	// 만약 마지막 세션갱신시간에서 5분(300초)가 지난 경우 세션이 만료됨
	if(currentTime - lastSessionTime >= 10)
		return true;
	else
		return false;
		*/
	return false;
}

// 방에 들어왔다는 것을 알리는 함수
void ConnectedClient::EnterRoom(Room* room)
{
	this->room = room;
}
// 방에 나갔다는 것을 알리는 함수
void ConnectedClient::ExitRoom()
{
	state = State::Idle;
	room = NULL;
}

// 현재 상태를 가져오는 함수
ConnectedClient::State ConnectedClient::GetState()
{
	return state;
}

// 현재 상태를 변경하는 함수
void ConnectedClient::SetState(State state)
{
	this->state = state;
}

// 세션을 가져오는 함수
string ConnectedClient::GetSession()
{
	return session;
}
