
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include "spdr-func.c" //boo hoo! i didn't use a .h file! it broke ):
//sudo ./spdr install textmite
//now that a lot of stuff is done, time to upload to github and work on rollbacking
int main(int argc, char *argv[]){ //very small int main woo hoo!
    check_for_error(argv[1]); //
    if (strcmp(argv[1], "list") == 0){
        list_pack();
        exit(EXIT_SUCCESS); //look lois im using exit_success
        //arent i pretty freakin sweet
        //we dont need to check for argv2 because it doesn't really matter
        //like its not used here.
    } else if (strcmp(argv[1], "info") == 0){
        check_for_error(argv[2]);
        pack_info(argv[2]);
        exit(EXIT_SUCCESS);
    } else if (strcmp(argv[1], "help") == 0){
        help_panel(); 
        exit(EXIT_SUCCESS);
    } //we put this section here because they dont require sudo
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
    } else if (strcmp(argv[1], "update") == 0){
        if (argv[2] != NULL){ //check whether it wants everything or not
            check_for_error(argv[2]);
            update_one_thing(argv[2]);
        } else{
            scan_dir(); //holy hell this is freakin sweet lois! it updates!
        }
    } else if (strcmp(argv[1], "search") == 0){
        check_for_error(argv[2]);
        check_for_404(argv[2]);
    } else {
        printf("Invalid command/argument\n"); //checks whether the argument is a phony! a big, fat phony!
        //hey! everyone! the user is a phony! a big stinking fat phony!
    }
    return 0;
}
