#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "spdr-func.c"
//sudo ./spdr install textmite
int main(int argc, char *argv[]){ //very small int main woo hoo!
    check_for_error(argv[1]); //we check whether argv[1] exists because update in the future will just auto update without argv[2].
    if (geteuid() != 0) { //checks whether it has root access
        printf("i need root in order to function so please retype with sudo at the front\n");
        printf("eg; sudo spdr install neofetch\n"); //you know i could technically combine this into check error but i feel like thats overdoing it
        exit(1);
    }
    if (strcmp(argv[1], "install") == 0){ //temp
        check_for_error(argv[2]); //remember to put this infront of check_package size, dont wanna give null to a function
        //combined them both into one
        installfile(argv[2]);
        } else if (strcmp(argv[1], "remove") == 0){
        check_for_error(argv[2]);
        delete_file(argv[2]);
    } else if (strcmp(argv[1], "update") == 0) {
        if (argc > 1){ //check whether it wants everything or not
            check_for_error(argv[2]);
            update_one_thing(argv[2]);
        }else{
            printf("adding soon\n"); //soon... yeap.. soon..
        }
    } else {
        printf("Invalid command/argument\n"); //checks whether the argument is a phony! a big, fat phony!
        //hey! everyone! the user is a phony! a big stinking fat phony!
    }
    return 0;
}
