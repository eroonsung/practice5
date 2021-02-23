#include "Server.h"

//변수선언
SOCKET Server::serverSocket;
WSAData Server::wsaData;
SOCKADDR_IN Server::serverAddress;
int Server::nextID;
vector<Client*> Server::connections;
Util Server::util;

//접속 확인 메시지
void Server::enterClient(Client* client) {
	char* sent = new char[256];
	ZeroMemory(sent, 256);
	sprintf(sent, "%s", "[Enter]");
	send(client->getClientSocket(), sent, 256, 0);
}

//현재 방이 가득 찼을 때
void Server::fullClient(Client* client) {
	char* sent = new char[256];
	ZeroMemory(sent, 256);
	sprintf(sent, "%s", "[Full]");
	send(client->getClientSocket(), sent, 256, 0);
}

// 차례대로 방에 접속을 하고 있는 클라이언트의 명단을 확인한 이후
//먼저 확인되는 사용자한테는 "[Play]Black", 두번째로 확인되는 사용자에게는 "[Play]White"
//랜덤으로 정해주는 방법도 있음
void Server::playClient(int roomID) {
	char* sent = new char[256];
	bool black = true;
	for (int i = 0; i < connections.size(); i++) {
		if (connections[i]->getRoomID() == roomID)
		{
			ZeroMemory(sent, 256);
			if (black)
			{
				sprintf(sent, "%s", "[Play]Black");
				black = false;
			}
			else
			{
				sprintf(sent, "%s", "[Play]White");
			}
			send(connections[i]->getClientSocket(), sent, 256, 0);
		}
	}
}

// 상대방 사용자가 나갔을 때 그 방에 남아있는 사용자를 찾아서 exit 명령어를 보내줌
void Server::exitClient(int roomID) {
	char* sent = new char[256];
	for (int i = 0; i < connections.size(); i++) {
		if (connections[i]->getRoomID() == roomID) {
			ZeroMemory(sent, 256);
			sprintf(sent, "%s", "[Exit]");
			send(connections[i]->getClientSocket(), sent, 256, 0);
		}
	}
}

//특정한 방에서 돌이 놓였을 때 그 방에 접속한 사용자에게 어느 위치에 돌이 놓였다는 것을 전송
void Server::putClient(int roomID, int x, int y) {
	char* sent = new char[256];
	for (int i = 0; i < connections.size(); i++) {
		if (connections[i]->getRoomID() == roomID) {
			ZeroMemory(sent, 256);
			string data = "[Put]" + to_string(x) + "," + to_string(y);
			sprintf(sent, "%s", data.c_str());
			send(connections[i]->getClientSocket(), sent, 256, 0);
		}
	}
}

// 서버의 입장에서 현재 특정한 방에 몇명의 클라이언트가 접속해있는지 확인 => Full/ Enter 처리
int Server::clientCountInRoom(int roomID) {
	int count = 0;
	for (int i = 0; i < connections.size(); i++) {
		if (connections[i]->getRoomID() == roomID)
		{
			count++;
		}
	}
	return count;
}

//서버가 가동되면 반복적으로 클라이언트의 연결을 기다림
void Server::start() {
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	serverSocket = socket(AF_INET, SOCK_STREAM, NULL);

	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(9876);
	serverAddress.sin_family = AF_INET;

	cout << "[C++ 오목 게임 서버 가동]" << endl;
	bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	listen(serverSocket, 32);

	int addressLength = sizeof(serverAddress);
	while (true)
	{
		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, NULL);
		if (clientSocket = accept(serverSocket, (SOCKADDR*)&serverAddress, &addressLength))
		{
			Client* client = new Client(nextID, clientSocket);
			cout << "[ 새로운 사용자 접속 ]" << endl;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ServerThread, (LPVOID)client, NULL, NULL);
			connections.push_back(client);
			nextID++;
		}
		Sleep(100);
	}
}


//연결이 실행되는 경우 serverThread 실행
//클라이언트가 접속할 때마다 각 클라이언트를 위한 Thread를 하나씩 만들어서 클라이언트에 할당
void Server:: ServerThread (Client *client){
	char* sent = new char[256];
	char* received = new char[256];
	int size = 0;
	while (true) {
		ZeroMemory(received, 256);
		// 특정한 클라이언트가 접속하면 클라이언트로부터 메시지를 기다림
		if ((size = recv(client->getClientSocket(), received, 256, NULL)) > 0){
			string receivedString = string(received);
			vector<string> tokens = util.getTokens(receivedString, ']');
			if (receivedString.find("[Enter]") != -1) {
				string roomID = tokens[1];
					int roomInt = atoi(roomID.c_str());
					int clientCount = clientCountInRoom(roomInt);
					/* 2명 이상이 동일한 방에 들어가 있는 경우*/
					if(clientCount >= 2){
						fullClient(client);
					}
					/*접속 성공*/
					client->setRoomID(roomInt);
					cout << "클라이언트 [" << client->getClientID() << "]: " << client->getRoomID() << "번 방으로 접속" << endl;
					/* 방에 성공적으로 접속했다고 메시지 전송*/	
					enterClient(client);
					/*상대방이 이미 방에 들어가 있는 경우 게임 시작*/
					if(clientCount == 1) {
						playClient(roomInt);
					}
			}
			else if (receivedString.find("[Put]") != -1) {
				/*메시지를 보낸 클라이언트 정보 받기*/
				string data = tokens[1];
				vector<string > dataTokens = util.getTokens(data, ',');
				int roomID = atoi(dataTokens[0].c_str());
				int x = atoi(dataTokens[1].c_str());
				int y = atoi(dataTokens[2].c_str());
				/*사용자가 놓은 돌의 위치를 전송*/
				// 그 방에 접속한 사람들에 한해서만 돌을 놓았다는 메시지를 전송할 수 있음
				putClient(client->getRoomID(), x, y);
			}
			else if (receivedString.find ("[Play]") != -1) {
				string roomID = tokens[1];
				int roomInt = atoi(roomID.c_str());
				/*사용자가 접속한 방의 번호에 시작 알림 전송*/
				playClient(client->getRoomID());
			}
		}
		else {
			cout << "클라이언트[" << client->getClientID() << "]의 연결이 끊어졌습니다" << endl;
			/*게임에서 나간 플레이어를 찾기*/
			for(int i = 0; i < connections.size (); i ++) {
				if(connections[i]->getClientID () == client->getClientID ()) {
					/*다른 사용자와 게임 중이던 사람이 나간 경우*/
					if(connections[i]-> getRoomID () != -1 &&
						clientCountInRoom(connections [i]-> getRoomID ()) == 2) {
						/*남아있는 사람에게 메시지 전송*/
						exitClient(connections[i]-> getRoomID());
					}
				connections.erase(connections.begin () +i);
				break;
			}
		}
		delete client;
		break;
		}
	}
}