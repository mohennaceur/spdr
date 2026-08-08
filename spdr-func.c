#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> //man i hate easyjet. yk they dont give you charging on the flight? nor power banks? ridiculous how am i
//supposed to charge my stuff
//):<
#ifndef SPDR_H
#define SPDR_H
int command_sequence = 1; //people infront of me on the plane are chowing down on some food
//luckily for me tho, i've got starbursts and they dont. HA!
//void write_cmd(char commands[200]){
//    if (system(commands) != 0){ //this really ought to be replaced with exec. but eh
//        printf("sequence %d failed. check in the files to see what (most likely permissions or wifi)\n", command_sequence);
//        printf("the command that was supposed to be ran is : %s\n", commands);
//        //as said before, i will never, never NEVER use fprintf. whats the point
//        exit(1);
//    }
//    command_sequence++; //this could kill a man
//}
void run_cmd(char argument[300]){
    pid_t pid = fork();
    if (pid < 0){
        printf("fork failed\n");
    } else if (pid == 0){
     char contain_arg[300];
     strcpy(contain_arg, argument);
     size_t len = strlen(argument); //this being contain_arg or arg doesnt matter
     if (len > 0 && contain_arg[len - 1] == '\n') {
         contain_arg[len - 1] = '\0';
     }
     //printf("You entered: %s\n", input);
     char *array_of_arg[300];
     int i = 0; //you know what my biggest pet peeve is? having to define what i's value is. int i should auto set to 0.
     char *token = strtok(contain_arg, " "); //this is a pointer which is cut up by spaces. this works.
     while (token != NULL && i < 299) {
        array_of_arg[i++] = token; //actually genius line
        token = strtok(NULL, " \t\n"); //turns it into array

     }
     array_of_arg[i] = NULL; // must kil arracy

     //char cmd[300];

    //snprintf(cmd, sizeof(cmd), "sudo rm /bin/%s", chosen_file);
    //
    //run_cmd(cmd, chosen_file);
    //use this!!!! snprintf is actually useful!! woo! party!!
     execvp(array_of_arg[0], array_of_arg);
     printf("if you're seeing this, execvp didnt kill itself.\n");
     exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0); //wait for baby
    }
}

void check_for_error(char argument[100]){ //the reason we use check-for-error is because its much more versatile
    if (argument == NULL){
        printf("not enough input\n");
        printf("yadda yadda\n");
        printf("hate having to babyproof for people like you\n");
        exit(1);
    }
    if (strlen(argument) > 69){ //checks argument size
        printf("your 'package' (it most likely doesn't exist) is too big dude\n");
        exit(1);
    }
}
void installfile(char chosen_file[100]){
    printf("installfile runs\n");
    char cmd[300];

    char code_for_system[500];
        // b
        //chdir(getenv("HOME")); //turns out this is the evil guy
        system("sudo rm -rf /root/spdr-repo"); //this is the safest net ever!
        printf("Installing stuff...\n");//, argv[2]
        if (system("git clone --depth=1 --filter=blob:none --sparse https://github.com/mohennaceur/spdr-repo.git /root/spdr-repo") != 0) {
            printf("git clone failed\n");
            exit(1);
        } //this will NOT be converted to an write_cmd because the cmd isn't a str
        //system("git clone --filter=blob:none --sparse https://github.com/OWNER/REPO.git && cd REPO git sparse-checkout set path/to/directory")
        chdir("/root/spdr-repo");
        snprintf(cmd, sizeof(cmd), "git sparse-checkout set %s", chosen_file);
        run_cmd(cmd);
        printf("moving actual files...\n");
        chdir("/root/spdr-repo");
        strcpy(code_for_system, "cd ~/spdr-repo/"); //cd ~/spdr-repo/
        //turns out cd ~/ when sudo'ed means you end up in root ): not good
        printf("%s", code_for_system);
        snprintf(cmd, sizeof(cmd), "sudo cp -r -v /root/spdr-repo/%s/ /opt/spdr", chosen_file);
        run_cmd(cmd);
        snprintf(cmd, sizeof(cmd), "chmod +x /opt/spdr/%s/%s", chosen_file, chosen_file);
        run_cmd(cmd);
        printf("\nmaking executable in /bin/\n");
        chdir("/bin/");
        snprintf(cmd, sizeof(cmd), "sudo touch %s", chosen_file);
        run_cmd(cmd); //sequence 4
        strcpy(code_for_system, "/bin/");
        strcat(code_for_system, chosen_file); 
        FILE *file = fopen(code_for_system, "w"); 
        if (file == NULL){ 
            printf("cannot open file!");
        }
        fprintf(file, "#!/bin/sh\nexec /opt/spdr/%s/%s \"$@\"\n",chosen_file,chosen_file);
        //i will always sing my praises for fprintf and all its cousins
        //except people who use fprintf for terminal text. you're weird
        fclose(file);
        snprintf(cmd, sizeof(cmd), "sudo chmod +x /bin/%s", chosen_file);
        run_cmd(cmd);
        printf("deleting github repo\n");
        chdir(getenv("HOME")); //escape from island
        system("sudo rm -rf /root/spdr-repo"); //i am not checking you bro :sob:
        printf("should be done!\n");
        exit(0);
}
 void delete_file(char chosen_file[50]) {
    char cmd[300];
    char code_for_system[70];
    printf("Deleting file...\n");
    chdir("opt/spdr");
    snprintf(cmd, sizeof(cmd), "sudo rm -rf %s", chosen_file);
    run_cmd(cmd);
    chdir("/bin/");
    snprintf(cmd, sizeof(cmd), "sudo rm /bin/%s", chosen_file);
    run_cmd(cmd);
    printf("File deleted! Woohoo!\n");
}

void update_one_thing(char argument[60]){
    char code_for_system[70]; //dunno about this being in every func
    delete_file(argument);
    //installfile(argument); //this is so much easier its insane
}
#endif //did you guys ever read what if? such a good book
////what if every rain drop was replaced with a full grown man
