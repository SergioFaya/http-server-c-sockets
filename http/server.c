// Server side C program to demonstrate HTTP Server programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

char request[30000] = {0};

char *host = "127.0.0.1";
int port = 8080;
int valid; 
int main(int argc, char const *argv[])
{
    if (argc > 2)
    {
        // printf("Host: %s\n",argv[1]);
        host = argv[1];
        // printf("Port: %i\n",atoi(argv[2]));
        port = atoi(argv[2]);
    }
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Only this line has been changed. Everything is same.
    char *ok_response = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nContent-Length: 149\r\n\n<!DOCTYPE html><html lang='es'><head><meta charset='UTF-8'><title>Sergio Faya</title></head><body><h1>Servidor HTTP de Sergio Faya</h1></body></html>";
    char *notFound_response = "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\nConnection: close\r\n\n<!DOCTYPE html><html lang='es'><body><h1>NOT FOUND</h1></body></html>";
    char *notImplemented_response = "HTTP/1.0 501 Not Implemented\r\nContent-Type: text/html\r\nConnection: close\r\n\n<!DOCTYPE html><html lang='es'><body><h1>NOT IMPLEMENTED</h1></body></html>";


    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) // abrimos puerto
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) // escuchamos el puerto
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        printf("\nServidor iniciado\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) // esperamos a recibir peticion
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        valread = read(new_socket, request, 30000); // leemos la http request
        printf("%s\n", request);
        // validateRequest(request);
        // printf("Valid: %s\n", valid);
        // if (valid == 0) // valido
        // {
        //     // TODO: mover el write dentro si es valido
            write(new_socket, ok_response, strlen(ok_response));
        // }
        // else if (valid == 1)
        // {
        //     write(new_socket, notImplemented_response, strlen(notImplemented_response));

        // }
        // else if (valid == 2)
        // {
        //     write(new_socket, notFound_response, strlen(notFound_response));

        // }
        printf("Respuesta enviada");
        close(new_socket);
    }
    return 0;
}

int validateRequest(char mensajeRec[])
{
    char delimiter[] = "\n\r";
    char *line = strtok(mensajeRec, delimiter);
    int lineCount = 0;
    while (line != NULL)
    {
        if (lineCount == 0)
        {
            char *request = strtok(line, " "); // metodo
            if (strcmp(request, "GET") != 0)
            {
                printf("En metodo '%s' no está implementado\n", request);
                // exit(EXIT_FAILURE);
                valid = 1;
                return;
            }
            request = strtok(NULL, " "); // resource address
            // TODO: check if file exists and return
            char *filename = request;
            filename++;
            printf("Filename:%s: \n", filename);
            if (strcmp(filename, "index.html") != 0)
            {
                printf("Recurso no encontrado '%s' \n", filename);
                // exit(EXIT_FAILURE);
                valid = 2;
                return;
            }
        }
        else
        {
            // procesar headers
        }
    }
    valid = 0;
    return;
}