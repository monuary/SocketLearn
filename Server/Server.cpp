#include"Server.h"

static void recvData(SOCKET s, int iNum)
{
	char szBuffer[iPacketSize] = {};
	int iRecv = { recv(s, szBuffer, sizeof szBuffer, 0) };
	if (iRecv <= 0)
		return;
	vecClient[iNum].second = { szBuffer };

	while (1)
	{
		ZeroMemory(szBuffer, sizeof szBuffer);
		iRecv = { recv(s, szBuffer, sizeof szBuffer, 0) };	//receive name
		if (!iRecv)
		{
			std::cout << vecClient[iNum].second << "disconnect" << std::endl;
			break;
		}
		else if (iRecv == SOCKET_ERROR)
		{
			int iError = { WSAGetLastError() };
			if (iError == WSAECONNRESET)
				std::cout << vecClient[iNum].second << "disconnect" << std::endl;
			else std::cout << "recv error : " << iError << std::endl;
			break;
		}
		std::cout << vecClient[iNum].second << " : " << szBuffer << std::endl;
	}
	closesocket(s);
}

static void ACCEPT(SOCKET &server) //accept Client
{ 
	int iCnt = { 0 };
	while (1) 
	{
		vecClient.push_back(pii(*CLIENT::Create(), ""));
		*vecClient[iCnt].first.GetSocketPtr() = { accept(server,(SOCKADDR*)vecClient[iCnt].first.GetAddrPtr(),vecClient[iCnt].first.GetSizePtr()) };
		vecClient[iCnt].first.SetNum(iCnt);
		std::thread(recvData, *vecClient[iCnt].first.GetSocketPtr(), iCnt).detach();
		++iCnt;
	}
}

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) 
		assert(false && "WSAStartup Error");

	SOCKET server = { socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) };

	SOCKADDR_IN addr = {};
	addr.sin_addr.s_addr = { htonl(INADDR_ANY) };
	addr.sin_port = { iPort };
	addr.sin_family = { AF_INET };

	bind(server, (SOCKADDR*)&addr, sizeof addr);
	listen(server, SOMAXCONN);

	std::thread(ACCEPT, std::ref(server)).detach();
	
	char szName[iPacketSize] = {}, szMessage[iPacketSize] = {};

	while (1)	//message to a Client
	{
		ZeroMemory(szName, sizeof szName);
		ZeroMemory(szMessage, sizeof szMessage);
		std::cin >> szName >> szMessage;
		for (size_t i = {}; i < vecClient.size(); ++i)
			if (!strcmp(vecClient[i].second.c_str(), szName))
				send(*vecClient[i].first.GetSocketPtr(), szMessage, sizeof szMessage, 0);
	}
}