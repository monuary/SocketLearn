#pragma once
#pragma comment(lib,"ws2_32.lib")	//linker

#include<iostream>
#include<WinSock2.h>
#include<Windows.h>
#include<vector>					//vector
#include<utility>					//pair
#include<thread>					//thread
#include<mutex>						//mutex
#include<string>					//getline
#include<assert.h>

class CLIENT	// structure is better
{
private:
	CLIENT() {}
public:
	SOCKET* GetSocketPtr() { return &Client; }
	SOCKADDR_IN* GetAddrPtr() { return &ClientAddr; }
	int* GetSizePtr() { return &iClientSize; }
	int* GetNumPtr() { return &iNum; }
public:
	void SetNum(int iDst) { iNum = { iDst }; }
private:
	SOCKET Client = {};
	SOCKADDR_IN ClientAddr = {};
	int iClientSize = { sizeof ClientAddr };
	int iNum = { -1 };
public:
	static CLIENT* Create() { return new CLIENT(); }
};

typedef std::pair<CLIENT, std::string>pii;
std::vector<pii>vecClient;

constexpr int	iPort = { 8080 };			// port address
constexpr int	iPacketSize = { 1024 };
std::mutex		Mutex;