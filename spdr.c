#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "spdr.h"
//sudo ./spdr install neofetch < basic cmd
int main(int argc, char *argv[]){ //very small int main woo hoo!
    check_for_error(argv[1]); //checks whether you actually input anything you dingus
    if (strcmp(argv[1], "install") == 0){ //temp
        check_for_error(argv[2]); //checks whether you actually input anything againnn
        installfile(argv[2]); 
        } else if (strcmp(argv[1], "remove") == 0){
        check_for_error(argv[2]);
        delete_file(argv[2]);
    } else if (strcmp(argv[1], "update") == 0) {
        printf("working on it!!!\n"); //not made yet, will be shortly? or will it take a while? you're all part of my domain now so HA
    } else {
        printf("invalid command/argument\n"); //what do you think
    }
    return 0;
}
