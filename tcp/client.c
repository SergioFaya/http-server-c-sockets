#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

struct sockaddr_in dirServ;

char mensajeRec[100];
int rec;
int s;
int res;

char *mensajeEnv = "GET /index.html HTTP/1.0\r\nAccept: text/html\r\n";
FILE *fptr;

char *host = "127.0.0.1";
int port = 8080;
main(int argc, char *argv[])
{
	if(argc > 2){
		// printf("Host: %s\n",argv[1]);
		host = argv[1];
		// printf("Port: %i\n",atoi(argv[2]));
		host = argv[2];
	}
	printf("Starting client\r\n");

	s = socket(AF_INET, SOCK_STREAM, 0);

	dirServ.sin_family = AF_INET;
	dirServ.sin_addr.s_addr = inet_addr(host);
	dirServ.sin_port = htons(port); /* servidor de echo */

	res = connect(s, &dirServ, sizeof(dirServ));
	write(s, mensajeEnv, strlen(mensajeEnv));
	int n = 0;
	int len = read(s, mensajeRec, len);;

	// while (n < len)
	// {
		ioctl(s, FIONREAD, &len);
		if (len > 0)
		{
			len = read(s, mensajeRec, len);
		}
		printf("%s\n", mensajeRec);
		n += len;
	// }
	// mensajeRec[tot] = '\0';
	// manageResponse(mensajeRec);
	printf("Fin\n");
	close(s);
}

void manageResponse(char mensajeRec[])
{
	parseMessage(mensajeRec);
	printResponse(mensajeRec);
}

void parseMessage(char mensajeRec[])
{
	char delimiter[] = "\r\n";
	char *ptr = strtok(mensajeRec, delimiter);
	while (ptr != NULL)
	{
		printf("\n%s", ptr);
		ptr = strtok(NULL, delimiter);
	}
}

void printResponse(char mensajeRec[])
{
	printf("recibido :\n%s:\n", mensajeRec);
}

void assertConnection(int resStatus)
{
	if (resStatus < 0)
	{
		perror("Connection canceled");
		exit(0);
	}
}
