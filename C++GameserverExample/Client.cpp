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
	if (::WSAStartup(MAKEWORD(2, 2),&wsaData )!= 0)
	{
		cout<< "오류발생" << endl;
		return 0;//초기화 실패시 리턴
	}

	SOCKET clientSocket = ::socket(AF_INET,SOCK_STREAM ,0 );//(af:주소체계,소켓타입,소켓 프로토콜종류)
	//af: AF_INET: IPv4, AF_INET6: IPv6
	//소켓 타입: TCP: 데이터 검증 및 패킷유실을 검증해주는 TCP/IP는 SOCK_STREAM
	//프로토콜: 0(자동할당)
	//return 시에는 descriptor라는 것을 반환함
	//일정의 정수를 받아서 해당 리소스에 할당된 번호를 가르키고 있다고 생각하면 됨
	if (clientSocket == INVALID_SOCKET)//제대로 할당받지못한것을 검증하는 기능
	{
		int32_t errCode = ::WSAGetLastError();//어떤 에러가 발생했는지 받아오는 함수
		cout << "Socket ErrorCode: " << errCode << endl;
		return 0;
	}


	SOCKADDR_IN serverAddr; //ipv4에서사용하는 구조체
	::memset(&serverAddr,0,sizeof(serverAddr));//c에서 제공하는 멤버초기화
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1");// 4바이트 유니온 Unsigned long(uchar 4개 또는 ushort 2개)
	//4바이트 정수를 넣어줘야하는데, 4개의 정수를 끊어서 전달(ip자릿수들)
	//근데 inet_addr함수가 구식이라 deprecated라는 경고문구가 뜸
	//해당 문구로 대체
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);//중간 주소는 룩백주소니까
	// 자기 컴퓨터 접근값이니 필요에따라 수정
	// 언젠간 DNS주소를 통해 ip값을 받아 넣어주는 기능을 넣게될것이다.
	// 하드코딩으로 주소를 넣을리가...
	serverAddr.sin_port = ::htons(7777);
	// 포트 번호: ip주소와 한쌍이 되는 값임, 서버 쪽에서 열어주는 포트
	// 사용할 수 없는 고정 포트가 있으니 따로 공부하도록, 특히 낮은 자리 수의 경우
	// host to networkshort => htons
	// 직접 넣어주지 않고 함수를 거치는 이유: Little-Endian 이슈
	// Little-Endian vs Big-Endian
	// ex) 0x12345678
	// low [0x78][0x56][0x34][0x12] high : Little = CPU, 게임 제작시 자주 쓰임
	// low [0x12][0x34][0x56][0x78] high : Big = 네트워크 방식 표준
	// 즉 저 함수는 host에서 네트워크 방식으로 변환시켜준다는 뜻 
	// 방식에 따라 htonl 등 다른 함수를 찾아서 사용해라

	// 정해진 주소와 연결해보자
	
	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32_t errCode = ::WSAGetLastError();//어떤 에러가 발생했는지 받아오는 함수
		cout << "Socket ErrorCode: " << errCode << endl;
		return 0;
	}
	// 두번째 파라미터는 connet 함수가 SOCKADDR* 타입 인자를 받기때문에 형변환
	// 크기도 같이 넣어줌

	// --------------------------------------------
	// 여기까지 문제 없다면 연결 성공, 데이터 송수신이 가능해짐
	cout << "Connected To Server" << endl;// 연결 성공 로그 생성

	// 연결 후 송수신간 데이터 전송을 위한 while문
	while (true)
	{
		// TODO
		this_thread::sleep_for(1s);
	}
	// ---------------------------------------------
	
	// 윈속 소켓 반환
	::closesocket(clientSocket);

	// 윈속 종료
	::WSACleanup();//할당 해제, WSASTARTUP이 호출된만큼 정리해줘야함
	
}