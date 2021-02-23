#ifndef GOMOKU_SERVER_H
#define GOMOKU_SERVER_H
#define _CRT_SECURE_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")
#include <iostream>
using namespace std;
#include <winsock.h>
#include <vector>
#include "Util.h"
#include "Client.h"

static class Server {
private:
	static SOCKET serverSocket;
	static WSAData wsaData;
	static SOCKADDR_IN serverAddress;
	static int nextID;
	static vector<Client*> connections;
	static Util util;
public:
	//필요한 모든 함수가 다 정의
	static void start();
	static int clientCountInRoom(int roomID);
	static void playClient(int roomID);
	static void exitClient(int roomID);
	static void putClient(int roomID, int x, int y);
	static void fullClient(Client *client);
	static void enterClient(Client *client);
	static void ServerThread(Client *client);
};
#endif // !GOMOKU_SERVER_H
