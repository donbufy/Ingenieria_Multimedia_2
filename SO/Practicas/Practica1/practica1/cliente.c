#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "memoria_compartida.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bits/signum-arch.h>

#define CLAVE 1234

void manejador();

int main(int argc, char *argv[]){

    if(argc != 2){

        printf("Por favor escriba solo un argumento");

    }

    else{
        
        char c[4] = {'A','B','X', 'Y'};
        int senal = 0;
        for(int i = 0; i<4; i++){
        
            if (c[i] == argv[1][0]){
            
                senal = 1;
            
            }
        
        }
        if (senal == 1){
        
        pid_t pid_cliente = getpid();

        printf("Soy el proceso cliente, mi PID es %d\n", pid_cliente);
        
        printf("Recibo como argumento el caracter %c", argv[1][0]);

        }

        else{
        
            printf("Escriba como argumento A, B, X o Y.\n");
        
        }

        int cliente_fd = socket(AF_INET, SOCK_STREAM, 0);

        if(cliente_fd < 0){
        
            perror("Error en socket\n");
            exit(EXIT_FAILURE);
        
        }

        struct sockaddr_in direccion_cliente = {0};

        direccion_cliente.sin_family = AF_INET;
        direccion_cliente.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &direccion_cliente.sin_addr);



        if(connect(cliente_fd, (struct sockaddr *)&direccion_cliente, sizeof(direccion_cliente)) < 0){
        
            perror("Error en connect\n");

            exit(EXIT_FAILURE);
        
        }

        char buffer [100] = {0};

        char letrita = argv[1][0];

        if(send(cliente_fd, &letrita, sizeof(letrita), 0) < 0){
        
            perror("Error al enviar letra");
            exit(EXIT_FAILURE);
        
        }

        
        



        

        struct memoria_compartida datos = {0};

        

        
        if(recv(cliente_fd, &datos, sizeof(datos), 0) < 0){
        
            perror("Error al recivir los PIDS\n");
            exit(EXIT_FAILURE);
        
        }
        
        
        printf("Los PIDS creados por el servidor son: %d, %d, %d, %d y %d\n", datos.pid_A_real, datos.pid_B_real, datos.pid_X_real, datos.pid_Y_real, datos.pid_Z_real);
        
    }

}
