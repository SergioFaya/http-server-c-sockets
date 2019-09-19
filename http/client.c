#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

struct sockaddr_in dirServ;
char mensajeEnv[100];
char mensajeRec[200];
int rec;
int s;


char* host = "127.0.0.1";
int puerto = 8080;

char* getRequest = "";
main(int argc, char *argv[])
{
    s = socket(AF_INET, SOCK_STREAM, 0);
    dirServ.sin_family = AF_INET;
    dirServ.sin_addr.s_addr = inet_addr(host);
    dirServ.sin_port = htons(puerto); /* servidor de echo */
    connect(s, &dirServ, sizeof(dirServ));
    strcpy(mensajeEnv, "hola amigo");
    write(s, mensajeEnv, strlen(mensajeEnv));
    int tot = strlen(mensajeEnv);
    int n = 0;
    while (n < tot)
    {
        rec = read(s, mensajeRec + n, tot - n);
        n += rec;
    }
    mensajeRec[tot] = '\0';
    printf("recibido :%s:\n", mensajeRec);

    close(s);
}