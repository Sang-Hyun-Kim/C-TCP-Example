//Ŭ���̾�Ʈ �ڵ�
#include "pch.h"
#include <iostream>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <stdint.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")//�߿�
#pragma comment(lib, "pthreadVC2.lib")
using namespace std;


int main()
{
	//����2 ���̺귯�� �ʱ�ȭ
	WSADATA wsaData;//���� ����
	//���� ������  wsaData�� ä����
	;//�������Ĺ���̺귯�� �ʱ�ȭ�Լ�, ���ڴ� (����Ĺ����2.2,������)
	if (::WSAStartup(MAKEWORD(2, 2),&wsaData )!= 0)
	{
		cout<< "�����߻�" << endl;
		return 0;//�ʱ�ȭ ���н� ����
	}

	SOCKET clientSocket = ::socket(AF_INET,SOCK_STREAM ,0 );//(af:�ּ�ü��,����Ÿ��,���� ������������)
	//af: AF_INET: IPv4, AF_INET6: IPv6
	//���� Ÿ��: TCP: ������ ���� �� ��Ŷ������ �������ִ� TCP/IP�� SOCK_STREAM
	//��������: 0(�ڵ��Ҵ�)
	//return �ÿ��� descriptor��� ���� ��ȯ��
	//������ ������ �޾Ƽ� �ش� ���ҽ��� �Ҵ�� ��ȣ�� ����Ű�� �ִٰ� �����ϸ� ��
	if (clientSocket == INVALID_SOCKET)//����� �Ҵ�������Ѱ��� �����ϴ� ���
	{
		int32_t errCode = ::WSAGetLastError();//� ������ �߻��ߴ��� �޾ƿ��� �Լ�
		cout << "Socket ErrorCode: " << errCode << endl;
		return 0;
	}


	SOCKADDR_IN serverAddr; //ipv4��������ϴ� ����ü
	::memset(&serverAddr,0,sizeof(serverAddr));//c���� �����ϴ� ����ʱ�ȭ
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1");// 4����Ʈ ���Ͽ� Unsigned long(uchar 4�� �Ǵ� ushort 2��)
	//4����Ʈ ������ �־�����ϴµ�, 4���� ������ ��� ����(ip�ڸ�����)
	//�ٵ� inet_addr�Լ��� �����̶� deprecated��� ������� ��
	//�ش� ������ ��ü
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);//�߰� �ּҴ� ����ּҴϱ�
	// �ڱ� ��ǻ�� ���ٰ��̴� �ʿ信���� ����
	// ������ DNS�ּҸ� ���� ip���� �޾� �־��ִ� ����� �ְԵɰ��̴�.
	// �ϵ��ڵ����� �ּҸ� ��������...
	serverAddr.sin_port = ::htons(7777);
	// ��Ʈ ��ȣ: ip�ּҿ� �ѽ��� �Ǵ� ����, ���� �ʿ��� �����ִ� ��Ʈ
	// ����� �� ���� ���� ��Ʈ�� ������ ���� �����ϵ���, Ư�� ���� �ڸ� ���� ���
	// host to networkshort => htons
	// ���� �־����� �ʰ� �Լ��� ��ġ�� ����: Little-Endian �̽�
	// Little-Endian vs Big-Endian
	// ex) 0x12345678
	// low [0x78][0x56][0x34][0x12] high : Little = CPU, ���� ���۽� ���� ����
	// low [0x12][0x34][0x56][0x78] high : Big = ��Ʈ��ũ ��� ǥ��
	// �� �� �Լ��� host���� ��Ʈ��ũ ������� ��ȯ�����شٴ� �� 
	// ��Ŀ� ���� htonl �� �ٸ� �Լ��� ã�Ƽ� ����ض�

	// ������ �ּҿ� �����غ���
	
	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32_t errCode = ::WSAGetLastError();//� ������ �߻��ߴ��� �޾ƿ��� �Լ�
		cout << "Socket ErrorCode: " << errCode << endl;
		return 0;
	}
	// �ι�° �Ķ���ʹ� connet �Լ��� SOCKADDR* Ÿ�� ���ڸ� �ޱ⶧���� ����ȯ
	// ũ�⵵ ���� �־���

	// --------------------------------------------
	// ������� ���� ���ٸ� ���� ����, ������ �ۼ����� ��������
	cout << "Connected To Server" << endl;// ���� ���� �α� ����

	// ���� �� �ۼ��Ű� ������ ������ ���� while��
	while (true)
	{
		// TODO
		this_thread::sleep_for(1s);
	}
	// ---------------------------------------------
	
	// ���� ���� ��ȯ
	::closesocket(clientSocket);

	// ���� ����
	::WSACleanup();//�Ҵ� ����, WSASTARTUP�� ȣ��ȸ�ŭ �����������
	
}