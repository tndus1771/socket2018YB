#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "Hi, I'm server\n";
 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char rcvBuffer[100];
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
 
	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}
 
	if(listen(s_socket, 5) == -1) {
		printf("listen Fail\n");
		return -1;
	}
 	
	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		printf("Client is connected\n");
		while(1){
			char * ptrtmp;
			char * sptr;
			char string[256];
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			else if(!strcmp(rcvBuffer, "안녕하세요."))
			{
				n = strlen("안녕하세요, 만나서 반가워요\n");
				write(c_socket, "안녕하세요, 만나서 반가워요\n", n);
			}
			else if(!strcmp(rcvBuffer, "이름이 머야?"))
			{
				n = strlen("내 이름은 심동근이야\n");
				write(c_socket, "내 이름은 심동근이야\n", n);
			}
			else if(!strcmp(rcvBuffer, "몇 살이야?"))
			{
				n = strlen("난 삼겹살이야\n");
				write(c_socket, "난 삼겹살이야\n", n);
			}
			else
			{
				sptr = strtok(rcvBuffer, " ");
				if(!strncasecmp(sptr, "strlen", 6))
				{
					sptr = strtok(NULL, " ");
					sprintf(string, "%d\n", strlen(sptr));
					n = strlen(string);
					write(c_socket, string, n);
				}
				else if(!strncasecmp(sptr, "strcmp", 6))
				{
					sptr = strtok(NULL, " ");
					ptrtmp = strtok(NULL, " ");
					sprintf(string, "%d\n", strcmp(sptr, ptrtmp));
					n = strlen(string);
					write(c_socket, string, n);
				}
				else if(!strncasecmp(sptr, "readfile", 8))
				{
					FILE * fp;
					sptr = strtok(NULL, " ");
					fp = fopen(sptr, "r");
					if(fp != NULL) {
						char string2[100];
						while(fgets(string2, 100, (FILE *)fp))
						{
							n = strlen(string2);
							write(c_socket, string2, n);
						}
						fclose(fp);
					}
					else
					{	
						n = strlen("해당 파일을 여는데 문제가 있습니다.\n");
						write(c_socket, "해당 파일을 여는데 문제가 있습니다.\n", n);
					}
				}
				else if(!strncasecmp(sptr, "exec", 4))
				{
					char * command;
					sptr = strtok(NULL, "\0");
					int ret = system(sptr);
					if(!ret)
					{

						n = strlen("command is executed.\n");
						write(c_socket, "command is executed.\n", n);
					}
					else
					{
						n = strlen("command failed.\n");
						write(c_socket, "command failed.\n", n);
					}
				}
				else
				{
					n = strlen(buffer);
					write(c_socket, buffer, n);
				}
			}
		}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}
