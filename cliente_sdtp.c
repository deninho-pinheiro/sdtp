#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "sdtp.h"

int main(int argc, char *argv[])
{
    char buffer[MAXSDTP];

    // descritor do socket do servidor
    int meusocket;

    // recebe o tamanho da estrutura sockaddr_in
    int sockettamanho;

    // numero de bytes recebidos pelo socket
    int numbytes;

    // informacoes do servidor
    struct sockaddr_in destinatario;

    if (argc != 4) 
    {
		printf("Erro: uso correto: ./cliente_sdtp ipservidor porta msg\n");
		return 1;
	}

    // tamanho da estrutura de endereco do socket usado
    sockettamanho = sizeof(struct sockaddr_in);

    // criando o socket
    meusocket = socket(AF_INET, SOCK_DGRAM, 0);

    destinatario.sin_family = AF_INET;

    // ip do servidor - 127.0.0.1 se estiver rodando na sua mesma maquina
    destinatario.sin_addr.s_addr = inet_addr(argv[1]);

    // porta do servidor
    destinatario.sin_port = htons(atoi(argv[2]));
    
    // zerando o resto da estrutura
    memset(&(destinatario.sin_zero), '\0', sizeof(destinatario.sin_zero));
    
    numbytes = sendto(meusocket, argv[3], strlen(argv[3]), 0,
			 (struct sockaddr *)&destinatario, sockettamanho);

    printf("Cliente: enviou %d bytes para %s:%s\n", 
            numbytes, argv[1], argv[2]);

    // exemplo de utilizacao da funcao de timeout no recvfrom
    // definindo um timeout de 10 segundos
    numbytes = recvtimeout(meusocket, buffer, MAXSDTP, 10000,
            (struct sockaddr *)&destinatario, &sockettamanho);

    // verificacao do retorno da funcao
    if (numbytes == -1)
    {
        // error occurred
        perror("recvtimeout");
    }
    else if (numbytes == -2)
    {
        // timeout occurred
    }
    else 
    {
        // got some data in buf
        printf("Cliente: recebeu %d bytes da mensagem \"%s\"\n", 
            numbytes, buffer);
    }

	close(meusocket);

    return 0;
}
