#include"Client.h"

static void recvData(SOCKET& s)
{
	char szBuffer[iPacketSize] = {};
	while (1)
	{
		ZeroMemory(szBuffer, sizeof szBuffer);
		int iRecv = { recv(s, szBuffer, sizeof szBuffer, 0) };
		if (!iRecv)
		{
			std::cout << "Server Disconnect" << std::endl;
			break;
		}
		else if (iRecv == SOCKET_ERROR)
		{
			int iError = { WSAGetLastError() };
			if (iError == WSAECONNRESET)
				std::cout << "Server Disconnect" << std::endl;
			else std::cout << "recv error : " << iError << std::endl;
			break;
		}
		std::cout << std::endl << "Server : " << szBuffer << std::endl << "Message : ";
	}
	closesocket(s);
}

int main()
{
	char szIP[iPacketSize] = {}, szName[iPacketSize] = {};


	std::cout << "Name : ";
	std::cin >> szName;
	std::cout << "IP : ";
	std::cin >> szIP;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
		assert(false && "WSAStartup Error");

	SOCKET server = { socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) };

	SOCKADDR_IN addr = { 0 };
	addr.sin_addr.s_addr = { inet_addr(szIP) };
	addr.sin_port = { iPort };
	addr.sin_family = { AF_INET };

	while (connect(server, (SOCKADDR*)&addr, sizeof addr));
	send(server, szName, sizeof szName, 0);	//send name

	std::thread(recvData, std::ref(server)).detach();

	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

	while (1)
	{
		ZeroMemory(szName, sizeof szName);
		std::cout << "Message : ";
		std::cin.getline(szName, iPacketSize);
		send(server, szName, sizeof szName, 0);
	}
}