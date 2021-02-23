#ifndef  GOMOKU_CLIENT_H
#define GOMOKU_CLIENT_H
	//헤더 파일이 단 한번만 불러와질 수 있도록 정의
#include <winsock.h>
class Client {
private:
	int clientID;
	int roomID;
	SOCKET clientSocket;
public:
	Client(int clientID, SOCKET clientSocket);
	int getClientID();
	int getRoomID();
	void setRoomID(int roomID);
	SOCKET getClientSocket();
};
#endif // ! GOMOKU_CLIENT_H
