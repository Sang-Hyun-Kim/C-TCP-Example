//클라이언트 코드
#include "pch.h"
#include <iostream>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <stdint.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")//중요
#pragma comment(lib, "pthreadVC2.lib")
using namespace std;


int main()
{
	//윈속2 라이브러리 초기화
	WSADATA wsaData;//설정 변수
	//관련 정보가  wsaData에 채워짐
	;//윈도우소캣라이브러리 초기화함수, 인자는 (윈소캣버전2.2,설정값)
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "오류발생" << endl;
		return 0;//초기화 실패시 리턴
	}

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);//(af:주소체계,소켓타입,소켓 프로토콜종류)
	//af: AF_INET: IPv4, AF_INET6: IPv6
	//소켓 타입: TCP: 데이터 검증 및 패킷유실을 검증해주는 TCP/IP는 SOCK_STREAM
	//프로토콜: 0(자동할당)
	//return 시에는 descriptor라는 것을 반환함
	//일정의 정수를 받아서 해당 리소스에 할당된 번호를 가르키고 있다고 생각하면 됨
	if (listenSocket == INVALID_SOCKET)//제대로 할당받지못한것을 검증하는 기능
	{
		int32_t errCode = ::WSAGetLastError();//어떤 에러가 발생했는지 받아오는 함수
		cout << "Socket ErrorCode: " << errCode << endl;
		return 0;
	}


	// 서버의 주소를 생성
	// 나의 주소는?
	SOCKADDR_IN serverAddr; //ipv4에서사용하는 구조체
	::memset(&serverAddr, 0, sizeof(serverAddr));//c에서 제공하는 멤버초기화
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);// 알아서 주소 연결
	// 하드 코딩시 네트워크의 수만큼 따로해줘야함
	serverAddr.sin_port = ::htons(7777);

	// 클라이언트와 서버의 소켓과 직접 연결시켜주기 BIND()

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32_t errCode = ::WSAGetLastError();//어떤 에러가 발생했는지 받아오는 함수
		cout << "BIND ErrorCode: " << errCode << endl;
		return 0;
	}
	// 통신 진행 LISTEN()

	//대기할 것, 최대 대기 한도, 나중엔 정책을 직접 지정해줄 것
	// 지금은 간단하게 10, 나중가면 굉장히 큰 숫자를 넣게 될 것

	// 연결 후 요청받기
	if (::listen(listenSocket, 10) == SOCKET_ERROR)
	{
		int32_t errCode = ::WSAGetLastError();//어떤 에러가 발생했는지 받아오는 함수
		cout << "Listen ErrorCode: " << errCode << endl;
		return 0;
	}
	// ----------------------------------------------

	while (true)
	{
		SOCKADDR_IN clientAddr;
		::memset(&clientAddr, 0, sizeof(clientAddr));
		// 서버가 받게될 사용자 ip, 나중에 쓰여질 내용들이니 기억
		int32_t addrLen = sizeof(clientAddr);
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		//	 ::acept()의 두,세번째 파라미터는 오는 클라이언트의 주소가 필요할 경우 사용
		//	필요없다면 NULL로 밀어버려도 가능
		// clientSocekt은 나중에 연결된 클라이언트랑 통신하고 싶으면
		// 사용되는 소켓임
		// accept가 끝날때 클라이언트와 연결이 되어 패킷을 주고받을 수 있는 소켓이 만들어진다
		if (clientSocket == INVALID_SOCKET)
		{
			int32_t errCode = ::WSAGetLastError();//어떤 에러가 발생했는지 받아오는 함수
			cout << "Accept ErrorCode: " << errCode << endl;
			return 0;
		}
		char ipaddress[16];
		::inet_ntop(AF_INET, &clientAddr.sin_addr, ipaddress, sizeof(clientAddr));
		//inet_ntop(): ipadress를 볼수있는 숫자로 바꿔줌
		cout << "Client Connected! IP= " << ipaddress << endl;
	}



	// ----------------------------------------------
	// BIND->Listen->Accept
	// 윈속 종료
	::WSACleanup();//할당 해제, WSASTARTUP이 호출된만큼 정리해줘야함
	//할당해제로그
	cout << "Connect END" << endl;
}