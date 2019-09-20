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

char *mensajeEnv = "GET /index.html HTTP/1.0\r\nAccept: text/html\r\n\n";

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

	if (res < 0)
	{
		perror("Error en connect");
		exit(EXIT_FAILURE);
	}

	int writen = write(s, mensajeEnv, strlen(mensajeEnv));
	if (writen < 0)
	{
		perror("Error en write");
		exit(EXIT_FAILURE);
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
	FILE *fp;

	fp = fopen("response.txt", "w+");
	if (fp == NULL)
	{
		perror("Cannot open file \n");
		exit(EXIT_FAILURE);
	}

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

			printf("------------------\n");

			storeInHeader = 0;
			headerSize = i;
		}
	}
	printf("\n\n"); // print header

	printf("caracteres usuario header %i\n", headerSize);
	printf("caracteres de body %i\n", bodySize);

	fputs(mensaje,fp);
// char final[10000];
// 	printf("aquí\n");

// bufferBody[bodySize] = NULL;
// 	printf("aquí\n");

// strcpy(final, bufferBody);
// 	printf("aquí\n");

	// bufferBody[bodySize] = '\0';
	// fputs(bufferBody, fp);// tira violacion de segmento, no puedo imprimir


	// fprintf(fptr, "%c", bufferHeader);

	// bufferHeader[headerSize-1] = '\0';
	// printf("%s\n", bufferHeader);
	// bufferBody[bodySize-1] = '\0';
	// fprintf("%s\n", bufferBody);

	fclose(fp);
}
