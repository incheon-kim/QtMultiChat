#include"protocol.h"

// 1. 연결
std::string Protocol_Connect::ToJson(const Protocol_Connect& protocol)
{
    json j;
    j["protocolType"] = "Protocol_Connect";

    j["data"] = "";

    return j.dump();
}

Protocol_Connect Protocol_Connect::ToProtocol(const std::string& data)
{
    Protocol_Connect p = {};

    json j = json::parse(data);
    if(j["protocolType"] != "Protocol_Connect") {
        return p;
    }

    return p;
}
// 2. 연결 결과
std::string Protocol_ConnectResult::ToJson(const Protocol_ConnectResult& protocol)
{
    json j;
    j["protocolType"] = "Protocol_ConnectResult";

    j["data"]["resultCode"] = static_cast<int>(protocol.resultCode);
    j["data"]["session"] = protocol.session;

    return j.dump();
}

Protocol_ConnectResult Protocol_ConnectResult::ToProtocol(const std::string& data)
{
    Protocol_ConnectResult p = {};

    json j = json::parse(data);
    if(j["protocolType"] != "Protocol_ConnectResult") {
        return p;
    }

    int c = j["data"]["resultCode"];
    p.resultCode = static_cast<ResultCode>(c);
    p.session = j["data"]["session"];

    return p;
}

// 3. 유저를 찾음
std::string Protocol_FindUser::ToJson(const Protocol_FindUser& protocol)
{
    json j;
    j["protocolType"] = "Protocol_FindUser";

    j["data"]["session"] = protocol.session;

    return j.dump();
}

Protocol_FindUser Protocol_FindUser::ToProtocol(const std::string& data)
{
    Protocol_FindUser p = {};

    json j = json::parse(data);
    if(j["protocolType"] != "Protocol_FindUser") {
        return p;
    }

    p.session = j["data"]["session"];

    return p;
}
// 4. 유저를 찾은 결과
std::string Protocol_FindUserResult::ToJson(const Protocol_FindUserResult& protocol)
{
    json j;
    j["protocolType"] = "Protocol_FindUserResult";

    j["data"]["resultCode"] = static_cast<int>(protocol.resultCode);
    j["data"]["userSession"] = protocol.userSession;

    return j.dump();
}

Protocol_FindUserResult Protocol_FindUserResult::ToProtocol(const std::string& data)
{
    Protocol_FindUserResult p = {};

    json j = json::parse(data);
    if(j["protocolType"] != "Protocol_FindUserResult") {
        return p;
    }

    int c = j["data"]["resultCode"];
    p.resultCode = static_cast<ResultCode>(c);
    p.userSession = j["data"]["userSession"];

    return p;
}

// 5. 메시지를 보냄

std::string Protocol_SendMessage::ToJson(const Protocol_SendMessage& protocol)
{
    json j;
    j["protocolType"] = "Protocol_SendMessage";

    j["data"]["session"] = protocol.session;
    j["data"]["message"] = protocol.message;

    return j.dump();
}

Protocol_SendMessage Protocol_SendMessage::ToProtocol(const std::string& data)
{
    Protocol_SendMessage p = {};

    json j = json::parse(data);
    if(j["protocolType"] != "Protocol_SendMessage") {
        return p;
    }

    p.session = j["data"]["session"];
    std::wstring t = j["data"]["message"];
    p.message = t;

    return p;
}
// 6. 메시지를 보낸 결과
std::string Protocol_SendMessageResult::ToJson(const Protocol_SendMessageResult& protocol)
{
    json j;
    j["protocolType"] = "Protocol_SendMessageResult";

    j["data"]["resultCode"] = static_cast<int>(protocol.resultCode);

    return j.dump();
}

Protocol_SendMessageResult Protocol_SendMessageResult::ToProtocol(const std::string& data)
{
    Protocol_SendMessageResult p = {};

    json j = json::parse(data);
    if(j["protocolType"] != "Protocol_SendMessageResult") {
        return p;
    }

    int c = j["data"]["resultCode"];
    p.resultCode = static_cast<ResultCode>(c);

    return p;
}

// 7. 메시지를 받음
std::string Protocol_GetMessage::ToJson(const Protocol_GetMessage& protocol)
{
    json j;
    j["protocolType"] = "Protocol_GetMessage";

    j["data"]["sendUserSession"] = protocol.sendUserSession;
    j["data"]["message"] = protocol.message;

    return j.dump();
}

Protocol_GetMessage Protocol_GetMessage::ToProtocol(const std::string& data)
{
    Protocol_GetMessage p = {};

    json j = json::parse(data);
    if(j["protocolType"] != "Protocol_GetMessage") {
        return p;
    }

    p.sendUserSession = j["data"]["sendUserSession"];
    std::wstring t = j["data"]["message"];
    p.message = t;

    return p;
}

// 8. 방을 나감
std::string Protocol_LeaveRoom::ToJson(const Protocol_LeaveRoom& protocol)
{
    json j;
    j["protocolType"] = "Protocol_LeaveRoom";

    j["data"]["session"] = protocol.session;

    return j.dump();
}

Protocol_LeaveRoom Protocol_LeaveRoom::ToProtocol(const std::string& data)
{
    Protocol_LeaveRoom p = {};

    json j = json::parse(data);
    if(j["protocolType"] != "Protocol_LeaveRoom") {
        return p;
    }

    p.session = j["data"]["session"];

    return p;
}
// 9. 상대방이 방을 나감
std::string Protocol_UserLeaved::ToJson(const Protocol_UserLeaved& protocol)
{
    json j;
    j["protocolType"] = "Protocol_UserLeaved";

    j["data"]["leavedUserSession"] = protocol.leavedUserSession;

    return j.dump();
}

Protocol_UserLeaved Protocol_UserLeaved::ToProtocol(const std::string& data)
{
    Protocol_UserLeaved p = {};

    json j = json::parse(data);
    if(j["protocolType"] != "Protocol_UserLeaved") {
        return p;
    }

    p.leavedUserSession = j["data"]["leavedUserSession"];

    return p;
}
