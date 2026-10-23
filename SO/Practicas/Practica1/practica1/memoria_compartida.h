#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h>

struct memoria_compartida{

    char letra;


    pid_t pid_servidor_real;

    pid_t pid_cliente;

    pid_t pid_A_real;

    pid_t pid_B_real;

    pid_t pid_X_real;

    pid_t pid_Y_real;

    pid_t pid_Z_real;

    int senyal;

    


};