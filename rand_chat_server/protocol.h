#ifndef PROTOCOL_H
#define PROTOCOL_H

#endif // PROTOCOL_H

#pragma once
#include<string>
#include<bitset>
#include<iostream>
#include"json.hpp"
// 메시지들 정의

using json = nlohmann::json;

enum ResultCode
{
	Sucess = 1,
	Fail = 2,
	FailToParseJson = 98
};

// 1. 연결
struct Protocol_Connect
{
	static std::string ToJson(const Protocol_Connect& protocol);

	static Protocol_Connect ToProtocol(const std::string& data);
};
// 2. 연결 결과
struct Protocol_ConnectResult
{
	ResultCode resultCode;
	std::string session;

	static std::string ToJson(const Protocol_ConnectResult& protocol);

	static Protocol_ConnectResult ToProtocol(const std::string& data);
};

// 3. 유저를 찾음
struct Protocol_FindUser
{
	std::string session;

	static std::string ToJson(const Protocol_FindUser& protocol);

	static Protocol_FindUser ToProtocol(const std::string& data);
};
// 4. 유저를 찾은 결과
struct Protocol_FindUserResult
{
	ResultCode resultCode;
	std::string userSession;

	static std::string ToJson(const Protocol_FindUserResult& protocol);

	static Protocol_FindUserResult ToProtocol(const std::string& data);
};

// 5. 메시지를 보냄
struct Protocol_SendMessage
{
	std::string session;
	std::wstring message;

	static std::string ToJson(const Protocol_SendMessage& protocol);

	static Protocol_SendMessage ToProtocol(const std::string& data);
};
// 6. 메시지를 보낸 결과
struct Protocol_SendMessageResult
{
	ResultCode resultCode;

	static std::string ToJson(const Protocol_SendMessageResult& protocol);

	static Protocol_SendMessageResult ToProtocol(const std::string& data);
};

// 7. 메시지를 받음
struct Protocol_GetMessage
{
	std::string sendUserSession;
	std::wstring message;

	static std::string ToJson(const Protocol_GetMessage& protocol);

	static Protocol_GetMessage ToProtocol(const std::string& data);
};

// 8. 방을 나감
struct Protocol_LeaveRoom
{
	std::string session;

	static std::string ToJson(const Protocol_LeaveRoom& protocol);

	static Protocol_LeaveRoom ToProtocol(const std::string& data);
};
// 9. 상대방이 방을 나감
struct Protocol_UserLeaved
{
	std::string leavedUserSession;

	static std::string ToJson(const Protocol_UserLeaved& protocol);

	static Protocol_UserLeaved ToProtocol(const std::string& data);
};
