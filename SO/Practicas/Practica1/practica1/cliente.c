#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "memoria_compartida.h"

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
        
        int shmid = shmget(CLAVE, sizeof(struct memoria_compartida), 0666);

        struct memoria_compartida *datos;

        datos = (struct memoria_compartida*) (shmat(shmid, NULL, 0));

        datos->letra = argv[1][0];
        datos->pid_cliente = getpid();
        kill(datos->pid_servidor_real, SIGUSR1);

        signal(SIGUSR1,manejador);

        pause();
        
        printf("Los PIDS creados por el servidor son: %d, %d, %d, %d y %d\n", datos->pid_A_real, datos->pid_B_real, datos->pid_X_real, datos->pid_Y_real, datos->pid_Z_real);
        
    }

}

void manejador(){

    printf("Recibo senyal");

}
