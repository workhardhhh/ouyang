#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include<pthread.h>

#define SEND_BUF_SIZE 100
#define RECV_BUF_SIZE 100
#define SERVER_PORT 7801 
#define LISTEN_PORT 7800
#define RECEIVE_BUF_SIZE 1024

void* receive(void* sendSocket);

int main(int argc, char* argv[])
{
	//����socket
	int sock_fd = 0;
	//����socket
	int listen_sock = 0;
	//���巢�����ݵ��ڴ�
	char sendbuf[SEND_BUF_SIZE] = { 0 };
	//������������г���
	int sendlen = 0;
	//��������ļ������ڴ�
	char file[RECEIVE_BUF_SIZE] = { 0 };
	//����������ݵ��ڴ�
	char buf[RECEIVE_BUF_SIZE] = { 0 };

	//�����������socketͨѶ��ַ
	struct sockaddr_in ser_addr;

	//���÷�������socket����
	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	inet_aton("127.0.0.1", (struct in_addr*)&ser_addr.sin_addr);
	ser_addr.sin_port = htons(SERVER_PORT);

	//����socket
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0)
	{
		printf("create socket failed\n");
		exit(1);
	}
	else
	{
		printf("welcome to talk\n");
	}

	//������߳�
	pthread_t tid;

	int ret = pthread_create(&tid, NULL, receive, (void*)&sock_fd);
	if (ret != 0)
	{
		printf("thread create error\n");
		close(sock_fd);
		exit(1);
	}


	while (1)
	{
		//��������

		scanf("%s", sendbuf);
		sendlen = strlen(sendbuf) + 1;

		int iRealSend = sendto(sock_fd, sendbuf, sendlen, 0,
			(struct sockaddr*)(&ser_addr), sizeof(struct sockaddr_in));
		printf("send data is :%s,send length:%d\n", sendbuf, iRealSend);
		sendlen -= iRealSend;

	}

	//�ر�socket
	close(sock_fd);
}

void* receive(void* sendSocket)
{
	int sock = *(int*)sendSocket;
	//������Socket��ַ
	struct sockaddr_in hostaddr;
	//�ͻ��˵�Socket��ַ
	struct sockaddr_in clientaddr;

	memset((void*)&hostaddr, 0, sizeof(hostaddr));
	memset((void*)&clientaddr, 0, sizeof(clientaddr));
	//���ñ�����socket��ַ
	hostaddr.sin_family = AF_INET;
	hostaddr.sin_port = htons(LISTEN_PORT);
	hostaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//�������ݵ��ڴ�
	char recvbuf[RECV_BUF_SIZE] = { 0 };

	//��Socket
	if (bind(sock, (struct sockaddr*)&hostaddr, sizeof(hostaddr)) < 0)
	{
		printf("bind socket failed\n");
		close(sock);
		exit(1);
	}


	while (1)
	{	
		//��ȡ����
		int iRealLength = recv(sock, recvbuf, RECV_BUF_SIZE, 0);

		if (iRealLength < 0)
		{
			printf("receive data error\n");
			close(sock);
			exit(1);
		}
		printf("server:%s\n", recvbuf);
	}



}


