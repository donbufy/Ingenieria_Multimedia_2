#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <signal.h>
#include "memoria_compartida.h"
#include <netinet/in.h>
#include <sys/wait.h>

#define CLAVE 1234

void crear_arbol(struct memoria_compartida* datos, int cliente_fd);

void manejador(int sig);
void manejador_c(int sig);
int main(){


    

    pid_t pid_servidor_real = getpid();

    printf("Soy el proceso servidor, mi PID es %d\n", pid_servidor_real);

    int shmid = shmget(CLAVE, sizeof(struct memoria_compartida), IPC_CREAT | 0666);

    struct memoria_compartida *datos;

    datos = (struct memoria_compartida *) shmat(shmid, NULL, 0);

   

    
    signal(SIGUSR1, manejador);
    signal(SIGUSR2, manejador);


    int server_fd = socket(AF_INET, SOCK_STREAM, 0 );

    if(server_fd < 0){
    
        perror("Error al crear el socket");

        exit(EXIT_FAILURE);
    
    }
    else{
    
        printf("socket creado correctamente\n");
        
    }

    
    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
    
    perror("Error en setsockopt\n");
    exit(EXIT_FAILURE);
    
    }
    else{
    
        printf("SO_REUSEADDR activo\n");
    
    }

    struct sockaddr_in direccion = {0};

    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(8080);
    direccion.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&direccion, sizeof(direccion)) < 0){
    
        perror("Error en bind\n");
        exit(EXIT_FAILURE);

    }

    else{
    
        printf("Bind Salio correctamente\n");
    
    }
    
    if (listen(server_fd, 5) < 0){
    
        perror("Error en listen\n");
        
        exit(EXIT_FAILURE);

    }

    datos->terminar = 0;

    while(1){

    signal(SIGINT, manejador_c);

    printf("A la espera de la conexion con el cliente\n");

    struct sockaddr_in direccion_cliente = {0};

    socklen_t tam = sizeof(direccion_cliente);

    if(datos->terminar == 0){

        int cliente_fd = accept(server_fd, (struct sockaddr *)&direccion_cliente, &tam);

    }

    else{
    
        int cliente_fd = -1;
        close(server_fd);
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    
    }
    
    if(cliente_fd < 0){
    
        perror("Fallo en accept\n");
        
        exit(EXIT_FAILURE);
    
    }

    char buffer;
    printf("A la espera del parametro\n");

    int byte_recibido = recv(cliente_fd, &buffer, sizeof(buffer), 0);

        if(byte_recibido < 1){
        
            perror("Error al recivir la letra\n");
            exit(EXIT_FAILURE);
        
        }

        datos->letra = buffer;

        datos->pid_servidor_real = pid_servidor_real;

        datos->pid_A_real = 0;

        datos->pid_B_real= 0;

        datos->pid_X_real = 0;

        datos->pid_Y_real = 0;

        datos->pid_Z_real = 0;

        datos->senyal = 0;
        
        datos->terminar


        crear_arbol(datos, cliente_fd);

        pause();
                    
        sleep(3);


        printf("Va a morir A con pid: %d\n", datos->pid_A_real);

        int r1 = kill(datos->pid_A_real, SIGKILL);
        printf("SIGKILL A devuelve: %d\n", r1);
        perror("KILL A");
        



        printf("Va a morir B con pid: %d\n", datos->pid_B_real);
        int r2 = kill(datos->pid_B_real, SIGKILL);
        printf("SIGKILL B devuelve: %d\n", r2);
        perror("KILL B");




        printf("Va a morir X con pid: %d\n", datos->pid_X_real);
        kill(datos->pid_X_real, SIGKILL);




        printf("Va a morir Y con pid: %d\n", datos->pid_Y_real);
        kill(datos->pid_Y_real, SIGKILL);

        while(wait(NULL) > 0){
        
            
        
        }

        close(cliente_fd);

        



    }

}

    





    //cuando llegue una señal, y los pids de la memoria compartida estén en 0 todos menos pid_servidor_real

void crear_arbol( struct memoria_compartida * datos, int cliente_fd ){

    printf("Soy el servidor y creo A\n");

    pid_t pid_A = fork();
    
    if (pid_A == 0){
    
        printf("Soy el proceso A y mi pid es %d\n", getpid());

        datos->pid_A_real = getpid();

        signal(SIGUSR1, manejador);
        
        
        pause();

        

            char pid_str[20];

            snprintf(pid_str, sizeof(pid_str), "%d", (int)datos->pid_servidor_real);

            kill(datos->pid_Z_real,SIGUSR2);
            execlp("pstree", "pstree", "-p", pid_str, NULL);
            perror("Error ejecutando pstree");

            exit(1);

    }

    else{
        printf("Soy el servidor y creo B\n");
        pid_t pid_B = fork();

        if(pid_B == 0){
            
            printf("Soy el proceso B con pid %d\n", getpid());

            datos->pid_B_real = getpid();

            printf("Soy el proceso B y creo X\n");

            pid_t pid_X = fork();

                
            if(pid_X == 0){
                
                printf("Soy el proceso X con pid %d\n", getpid());
                
                datos->pid_X_real = getpid();

                signal(SIGUSR1, manejador);

                pause();
                sleep(1);
                
                kill(datos->pid_Z_real, SIGUSR2);
                execlp("ls", "ls", "-p", NULL);
                exit(0);
                

                
                    
                
                    
                
            }


            else{
                    
                printf("Soy el proceso B y creo Y\n");

                pid_t pid_Y  = fork();

                if(pid_Y == 0){
                        
                    printf("Soy el proceso Y con pid %d\n", getpid());

                    datos->pid_Y_real = getpid();

                    signal(SIGUSR1,manejador);
                    signal(SIGUSR2,manejador);
                    pause();

                    sleep(1);
                    
                    
                    kill(datos->pid_Z_real,SIGUSR2);

                    execlp("ls", "ls", "-p", NULL);
                    exit(0);
                    
                    
                        
                        
                }

                else{
                    printf("Soy el proceso B y creo Z\n");

                    pid_t pid_Z = fork();    
                            
                    if(pid_Z == 0){
                            
                        printf("Soy el proceso Z con pid %d\n", getpid());

                        datos->pid_Z_real = getpid();

                        
                        sleep(15);

                        switch(datos->letra){
    
                            case('A'):
                            
                            kill(datos->pid_A_real, SIGUSR1);
                            
                            
                            break;

                            case('B'):

                            kill(datos->pid_B_real, SIGUSR1);

                            break;

                            case('X'):
                            
                            kill(datos-> pid_X_real, SIGUSR1);
                            break;

                            case('Y'):

                            kill(datos-> pid_Y_real, SIGUSR1);

                            break;

                            default:

                            break;
                        }

                        

                        signal(SIGUSR2, manejador);

                        pause();
                        sleep(4);

                        kill(datos->pid_servidor_real, SIGUSR2);
                        
                        if(send(cliente_fd, datos, sizeof(*datos), 0) < 0){
                        
                            perror("Fallo al enviar los pids\n");
                            exit(EXIT_FAILURE);
                                                    
                        }
                        exit(0);

                    }
                    
                    else{

                        signal(SIGUSR1,manejador);

                        

                        pause();

                        sleep(1);

                        

                            char pid_str[20];
                            snprintf(pid_str, sizeof(pid_str), "%d", (int)datos->pid_servidor_real);


                            kill(datos->pid_Z_real, SIGUSR2);
                            execlp("pstree", "pstree", "-p", pid_str, NULL);

                            perror("Error");

                            exit(1);

                        }
                    
                    }
                
                }
                
            }
        
        }
        

    }



void manejador(int sig){

    

}

void manejador_c(int sig){

    int shmid = shmget(CLAVE, sizeof(struct memoria_compartida), 0666);

    struct memoria_compartida * datos;

    datos = (struct memoria_compartida *) shmat(shmid, NULL, 0);

    



    
    shmdt(shmid);

    shmctl(shmid, IPC_RMID, NULL);

}