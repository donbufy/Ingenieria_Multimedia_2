//Jose Manuel Rico Lorenzo
//DNI 51235628T
//Correo UA: jmrl21@alu.ua.es

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

#define CLAVE 1234

void crear_arbol(struct memoria_compartida* datos, int cliente_fd, int shmid);

void manejador(int sig);

int main(){

    pid_t pid_servidor_real = getpid();
    printf("Soy el proceso servidor, mi PID es %d\n", pid_servidor_real);

    //Memoria compartida
    int shmid = shmget(CLAVE, sizeof(struct memoria_compartida), IPC_CREAT | 0666);
    if(shmid < 0){
    
        perror("shmget");
        exit(EXIT_FAILURE);
    
    }
    struct memoria_compartida *datos;
    datos = (struct memoria_compartida *) shmat(shmid, NULL, 0);
    if(datos == (void *) -1){

        perror("shmat");
        exit(EXIT_FAILURE);

    } 
    signal(SIGUSR1, manejador);
    signal(SIGUSR2, manejador);

    //SOCKETS
    int server_fd = socket(AF_INET, SOCK_STREAM, 0 );
    if(server_fd < 0){
    
        perror("Error al crear el socket");

        exit(EXIT_FAILURE);
    
    }
    
    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
    
    perror("Error en setsockopt\n");
    exit(EXIT_FAILURE);
    
    }
    

    struct sockaddr_in direccion = {0};

    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(8080);
    direccion.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&direccion, sizeof(direccion)) < 0){
    
        perror("Error en bind\n");
        exit(EXIT_FAILURE);

    }
    
    if (listen(server_fd, 5) < 0){
    
        perror("Error en listen\n");
        
        exit(EXIT_FAILURE);

    }
    
    while(1){

        printf("A la espera de la conexion con el cliente\n");

        struct sockaddr_in direccion_cliente = {0};
        socklen_t tam = sizeof(direccion_cliente);
        int cliente_fd = accept(server_fd, (struct sockaddr *)&direccion_cliente, &tam);
        
        if(cliente_fd < 0){
        
            perror("Fallo en accept\n");
            
            exit(EXIT_FAILURE);
        
        }

        char buffer;

        int byte_recibido = recv(cliente_fd, &buffer, sizeof(buffer), 0);
        
        if(byte_recibido < 1){
            
            perror("Error al recivir la letra\n");
            exit(EXIT_FAILURE);
            
        }
        //Inicializar la memoria a valores que no son basura del cliente anterior
        printf("Recibo del cliente el caracter %c\n",buffer);
        datos->letra = buffer;

        datos->pid_servidor_real = pid_servidor_real;

        datos->pid_A_real = 0;

        datos->pid_B_real= 0;

        datos->pid_X_real = 0;

        datos->pid_Y_real = 0;

        datos->pid_Z_real = 0;

        datos->senyal = 0;
        //cerrar servidor
        if(buffer == 'Q'){
            
            break;
            
        }


        crear_arbol(datos, cliente_fd, shmid);

        pause();
                        
        sleep(3);

        printf("Va a morir A con pid: %d\n", datos->pid_A_real);

        int r1 = kill(datos->pid_A_real, SIGKILL);
        printf("SIGKILL A devuelve: %d\n", r1);
                

        printf("Va a morir B con pid: %d\n", datos->pid_B_real);
        int r2 = kill(datos->pid_B_real, SIGKILL);
        printf("SIGKILL B devuelve: %d\n", r2);
        

        printf("Va a morir X con pid: %d\n", datos->pid_X_real);
        kill(datos->pid_X_real, SIGKILL);

        printf("Va a morir Y con pid: %d\n", datos->pid_Y_real);
        kill(datos->pid_Y_real, SIGKILL);

        while(wait(NULL) > 0){
            
                
            
        }

        close(cliente_fd);
        
    }
    //Cuando buffer == q
    close(server_fd);
    shmctl(shmid, IPC_RMID, NULL);

}

    //cuando llegue una señal, y los pids de la memoria compartida estén en 0 todos menos pid_servidor_real

void crear_arbol( struct memoria_compartida * datos, int cliente_fd, int shmid){

    pid_t pid_A = fork();
    
    if (pid_A == 0){
    
       

        datos->pid_A_real = getpid();

        signal(SIGUSR1, manejador);
        
        
        pause();

        printf("Soy el Proceso A con pid %d, he recibido la senyal\n", getpid());        

        char pid_str[20];

        snprintf(pid_str, sizeof(pid_str), "%d", (int)datos->pid_servidor_real);

        kill(datos->pid_Z_real,SIGUSR2);
        execlp("pstree", "pstree", "-p", pid_str, NULL);
        shmdt(datos);
        perror("Error ejecutando pstree");
        exit(1);

    }

    else{
        
        pid_t pid_B = fork();

        if(pid_B == 0){
            
            datos->pid_B_real = getpid();


            pid_t pid_X = fork();

                
            if(pid_X == 0){
                
                
                datos->pid_X_real = getpid();

                signal(SIGUSR1, manejador);

                pause();
                sleep(1);
                printf("Soy el Proceso X con pid %d, he recibido la senyal\n", getpid());
                kill(datos->pid_Z_real, SIGUSR2);

                shmdt(datos);
                execlp("ls", "ls", "-p", NULL);
                exit(0);                  
                
            }


            else{

                pid_t pid_Y  = fork();

                if(pid_Y == 0){

                    datos->pid_Y_real = getpid();

                    signal(SIGUSR1,manejador);
                    signal(SIGUSR2,manejador);
                    pause();

                    sleep(1);
                    printf("Soy el Proceso Y con pid %d, he recibido la senyal\n", getpid());
      
                    kill(datos->pid_Z_real,SIGUSR2);

                    shmdt(datos);

                    execlp("ls", "ls", "-p", NULL);
                    exit(0);                       
                        
                }

                else{

                    pid_t pid_Z = fork();    
                            
                    if(pid_Z == 0){

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

                        shmdt(datos);

                        exit(0);

                    }
                    
                    else{

                        signal(SIGUSR1,manejador);

                        

                        pause();

                        sleep(1);

                        char pid_str[20];
                        snprintf(pid_str, sizeof(pid_str), "%d", (int)datos->pid_servidor_real);


                        kill(datos->pid_Z_real, SIGUSR2);

                        shmdt(datos);
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