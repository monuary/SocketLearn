#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")

#include<iostream>
#include<WinSock2.h>
#include<Windows.h>
#include<thread>					//thread
#include<assert.h>
#include<string>

constexpr int iPort = { 8080 };
constexpr int iPacketSize = { 1024 };