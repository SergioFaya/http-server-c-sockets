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
	if (argc > 2)
	{
		// printf("Host: %s\n",argv[1]);
		host = argv[1];
		// printf("Port: %i\n",atoi(argv[2]));
		port = argv[2];
	}
	printf("Starting client\r\n");

	s = socket(AF_INET, SOCK_STREAM, 0);

	dirServ.sin_family = AF_INET;
	dirServ.sin_addr.s_addr = inet_addr(host);
	dirServ.sin_port = htons(port); /* servidor de echo */

	res = connect(s, &dirServ, sizeof(dirServ));
	printf("%i\n", res);

	if (res < 0)
	{
		perror("Error en connect");
		exit(0);
	}

	int writen = write(s, mensajeEnv, strlen(mensajeEnv));
	printf("%i\n", writen);
	if (writen < 0)
	{
		perror("Error en write");
		exit(0);
	}

	int n = 0;
	int len = read(s, mensajeRec, len);
	;
	// while (n < len)
	// {
	ioctl(s, FIONREAD, &len);
	if (len > 0)
	{
		len = read(s, mensajeRec, len);
	}
	// printf("%s\n", mensajeRec);
	// n += len;
	// }
	// mensajeRec[tot] = '\0';
	parseMessage(mensajeRec);
	printf("Fin\n");
	close(s);
}

void parseMessage(char mensajeRec[])
{
	char delimiter[] = "\n"; // separador head de body
	// char *ptr = strtok(mensajeRec, delimiter);
	// while (ptr != NULL)
	// {
	// 	printf("-------------\n%s\n", ptr);
	// 	ptr = strtok(NULL, delimiter);
	// }
	char *mensaje = mensajeRec;
	char storage;
	char *bufferHeader;
	int headerSize;
	char *bufferBody;
	int bodySize;
	int storeInHeader = 1;
	for (int i = 0; mensaje[i] != '\0'; ++i)
	{
		bodySize = i - headerSize;
		if (storeInHeader == 1)
		{
			bufferHeader += mensaje[i];
			printf("%c", mensaje[i]);
		}
		else
		{
			bufferBody += mensaje[i];
			printf("%c", mensaje[i]);
		}

		if (mensaje[i] == '\n' && mensaje[i + 1] == '\n')
		{
			printf("\n"); // print header

			printf("haz split %d\n", mensaje[i + 2]);

			storeInHeader = 0;
			headerSize = i;
		}
	}
	printf("\n"); // print header

	printf("header %i\n", headerSize);
	printf("body %i\n", bodySize);

	fptr = fopen("text.html", "w");
	if (fptr == NULL)
	{
		printf("Cannot open file \n");
		exit(EXIT_FAILURE);
	}

	// fprintf(fptr, "%c", bufferHeader);

	bufferHeader[headerSize] = '\0';
	// printf("%s\n", bufferHeader);
	fprintf(fptr, "%s", bufferHeader);
	bufferBody[bodySize] = '\0';
	printf("%s\n", bufferBody);
}
