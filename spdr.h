#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#ifndef SPDR_H //i have no idea what these do but they're there because it makes it work
#define SPDR_H
 void installfile(char chosen_file[40]){ //i've heard about const char and i dont see the point
    pid_t pid = fork(); //i actually have to use fork
    //this is why you read things
    //never thought the day would happen holy hell lois
    char code_for_system[500];
    if (pid < 0) {
        perror("Fork failed"); //checks if fork actually exploded and killed them all
        return;
    } 
    else if (pid == 0) {
        // baby
        
        //chdir(getenv("HOME")); //turns out this is the evil guy
        system("sudo rm -rf /root/spdr-repo"); //this is the safest net ever!
      //root is used because every other directory hates me
      //in theory it'd work in other directories but im not gonna
        printf("Installing stuff...\n");//, argv[2]
        if (system("git clone --depth=1 --filter=blob:none --sparse https://github.com/mohennaceur/spdr-repo.git /root/spdr-repo") != 0) {
            printf("git clone failed\n");
            exit(1);
        }
        //system("git clone --filter=blob:none --sparse https://github.com/OWNER/REPO.git && cd REPO git sparse-checkout set path/to/directory")
        strcpy(code_for_system, "git sparse-checkout set ");
        strcat(code_for_system, chosen_file); //git sparse-checkout set [name]
        chdir("/root/spdr-repo");
        if (system(code_for_system) != 0){
            printf("other part of installing git failed\n");
            exit(1);
        }
        printf("moving actual files...\n");
        //system("sudo -s"); //so i dont actually kill someone
        chdir(getenv("HOME")); //so i actually dont kill someone ++
        strcpy(code_for_system, "cd ~/spdr-repo/"); //cd ~/spdr-repo/
        //turns out cd ~/ when sudo'ed means you end up in root ): not good
        strcat(code_for_system, chosen_file); //cd ~/spdr-repo/[name]
        strcat(code_for_system, " && sudo cp -r -v /root/spdr-repo/");//cd ~/spdr-repo/[name] && && sudo cp -r -v ~/spdr-repo/
        strcat(code_for_system, chosen_file); //cd ~/spdr-repo/[name] && && sudo cp -r -v /opt-repo/[name]
        strcat(code_for_system,  " /opt/spdr/"); //cd /opt-repo/[name] && && sudo cp -r -v /opt-repo/[name] /opt/
        printf("%s", code_for_system); //opt is chosen because its optional and because nobody actually uses opt
        if (system(code_for_system) != 0){ //i could write a func for this but i wont because i hate you
            printf("getting into the repo and cloning the files failed\n");
            exit(1);
        }
        strcpy(code_for_system, "sudo chmod +x /opt/spdr/"); //chmod +x /opt/
        strcat(code_for_system, chosen_file); //chmod +x /opt/[name]
        strcat(code_for_system, "/");//chmod +x /opt/[name]/
        strcat(code_for_system, chosen_file); //chmod +x /opt/[name]/[name]
        if (system(code_for_system) != 0){
            printf("making the files executable failed\n");
            exit(1);
        }
        printf("\nmaking executable in /bin/\n");
        strcpy(code_for_system, "cd /bin && sudo touch "); //cd /bin && sudo touch
        strcat(code_for_system, chosen_file);//cd /bin && sudo touch [name]
        if (system(code_for_system) != 0){
            printf("touching the files o_o or entering bin failed\n");
            exit(1);
        }
        strcpy(code_for_system, "/bin/");
        strcat(code_for_system, chosen_file); 
        FILE *file = fopen(code_for_system, "w"); 
        fprintf(file, "#!/bin/sh\nexec /opt/spdr/%s/%s \"$@\"\n",chosen_file,chosen_file); //links to C file
        strcpy(code_for_system, "sudo chmod +x /bin/");
        strcat(code_for_system, chosen_file);
        if (system(code_for_system) != 0){
            printf("making bin file executable failed \n");
            exit(1);
        }
        printf("deleting github repo\n");
        chdir(getenv("HOME")); //escape from island
        system("sudo rm -rf /root/spdr-repo"); //i am not checking you bro :sob:
        printf("should be done!\n");
        exit(0);
        }else{
            //code for the parent
            int status;
            waitpid(pid, &status, 0); //wait for baby
            //printf("from parent, baby executed\n");    
        }
}
 void delete_file(char chosen_file[50]) {
    char code_for_system[70];
    strcpy(code_for_system, "cd /opt/spdr && sudo rm -rf ");
    strcat(code_for_system, chosen_file); //this is all pretty simple
   //so no comments needed
    printf("Deleting file...\n");
    if (system(code_for_system) != 0){
        printf("deleting opt file failed ):\n");
        exit(1);
    }
    strcpy(code_for_system, "sudo rm /bin/");
    strcat(code_for_system, chosen_file);
    if (system(code_for_system) != 0){
        printf("deleting bin file failed ):\n");
        exit(1);
    }
    printf("File deleted! Woohoo!\n");
}
 void check_for_error(char argument[60]){
    if (argument == NULL){
        printf("not enough input\n");
        printf("yadda yadda\n");
        printf("hate having to babyproof for people like you\n"); //< read it and weep... it
        exit(1);
    }
}
#endif
