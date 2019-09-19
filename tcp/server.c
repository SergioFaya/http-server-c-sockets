#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

struct sockaddr_in dirServ;
struct sockaddr_in dirCli;
char mensajeEnv[2048];
char mensajeRec[2048];
int s;
int n;
int sconn;
char *buffer;


int sendResponse()
{
	while (n > 0)
	{
		int valid = parseMessage(mensajeRec);
		if (valid == 0) {
			write (sconn,buffer,n);
			/* free the memory we used for the buffer */
			free(buffer);
		}
		n = read(sconn, mensajeRec, sizeof(mensajeRec));
	}
}

void writeBadRequest()
{
	// 400 Bad Request
}

// returns 1 if valid, 0 otherwise
int parseMessage(char mensajeRec[])
{
	char delimiter[] = "\n\r";
	char *line = strtok(mensajeRec, delimiter);
	int count  = 0;
	FILE *infile;
	long numbytes;
	/* open an existing file for reading */
	char* filename;
	while (line != NULL)
	{
		printf("En line'%s'\n", line);
		if (count == 0)
		{
			char *request = strtok(line, " "); // metodo
			if (strcmp(request, "GET") != 0)
			{
				printf("En metodo '%s' no está implementado\n", request);
				// TODO: enviar 400 not implemented
				exit(0);
			}
			request = strtok(NULL, " "); // resource address
			// TODO: check if file exists and return
			
			filename = request;
			filename++;
			printf("Filename: %s \n", filename);
			infile = fopen(filename, "r");

			/* quit if the file does not exist */
			if (infile == NULL)
			{
				// todo: error de recurso no encontrado
				perror("Error leyendo el fichero\n");
				exit(0);
			}
			/* Get the number of bytes */
			fseek(infile, 0L, SEEK_END);
			numbytes = ftell(infile);

			/* reset the file position indicator to 
the beginning of the file */
			fseek(infile, 0L, SEEK_SET);

			/* grab sufficient memory for the 
buffer to hold the text */
			buffer = (char *)calloc(numbytes, sizeof(char));

			/* memory error */
			if (buffer == NULL)
			{
				printf("File empty\n");
				exit(0);
			}
			/* copy all the text into the buffer */
			fread(buffer, sizeof(char), numbytes, infile);
			fclose(infile);
			/* confirm we have read the file by outputing it to the console */
			printf("The file called test.dat contains this text\n\n%s", buffer);

			request = strtok(NULL, " "); // version
		}
		else if (count > 0)
		{
			// validate header
			printf("%s",line);
		}
		line = strtok(NULL, delimiter);
		printf("%d\n", count);

		count++;
	}
	return 0;
}

main(int argc, char *argv[])
{
	printf("Starting server\r\n");
	s = socket(AF_INET, SOCK_STREAM, 0);
	dirServ.sin_family = AF_INET;
	dirServ.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip
	dirServ.sin_port = htons(8080);					  // puerto
	bind(s, &dirServ, sizeof(dirServ));
	listen(s, 5); // escucha 5 conexiones a la vez
	while (1)
	{
		int tam = sizeof(dirCli);
		bzero(&dirCli, tam);
		sconn = accept(s, &dirCli, &tam);
		n = read(sconn, mensajeRec, sizeof(mensajeRec));
		n = sendResponse(n, sconn);
		close(sconn);
	}
}
