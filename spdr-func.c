#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h> //man i hate easyjet. yk they dont give you charging on the flight? nor power banks? ridiculous how am i
//supposed to charge my stuff
//):<
#ifndef SPDR_H
#define SPDR_H
//people infront of me on the plane are chowing down on some food
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
    pid_t pid = fork(); //fork so it doesn't suicide
    if (pid < 0){
        printf("fork failed\n");
    } else if (pid == 0){
     char contain_arg[300];
     strcpy(contain_arg, argument);
     size_t len = strlen(argument); //this being contain_arg or arg doesnt matter
     if (len > 0 && contain_arg[len - 1] == '\n') { //if it has a /n it replaces it with a null indicator
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
    char cmd[300];
        // b
        //chdir(getenv("HOME")); //turns out this is the evil guy
        //we did it! we destroyed the repo! no file deletion needed
        printf("Installing stuff...\n");//, argv[2]
        chdir("/tmp/spdr-repo/");
        //welcome to run_cmd hell
        snprintf(cmd, sizeof(cmd), "curl -L -o %s.tar.gz https://github.com/mohennaceur/spdr-repo/raw/refs/heads/main/%s/%s.tar.gz", chosen_file, chosen_file, chosen_file);
        run_cmd(cmd);
        snprintf(cmd, sizeof(cmd), "tar -xf %s.tar.gz -C /opt/spdr", chosen_file);
        run_cmd(cmd);
        printf("moving actual files...\n");
        snprintf(cmd, sizeof(cmd), "chmod +x /opt/spdr/%s/%s", chosen_file, chosen_file);
        run_cmd(cmd);
        printf("\nmaking executable in /bin/\n");
        chdir("/bin/");
        snprintf(cmd, sizeof(cmd), "sudo touch %s", chosen_file);
        run_cmd(cmd); //sequence 4
        snprintf(cmd, sizeof(cmd), "/bin/%s", chosen_file);
        FILE *file = fopen(cmd, "w"); 
        if (file == NULL){ 
            printf("cannot open file!\n");
            exit(1);
        }
        fprintf(file, "#!/bin/sh\nexec /opt/spdr/%s/%s \"$@\"\n#this was made by Mohammed Ennaceur's spdr tool! Thanks for using me!\n#although if you're here most likely something went wrong.\n#sorry?",chosen_file,chosen_file);
        //i will always sing my praises for fprintf and all its cousins
        //except people who use fprintf for terminal text. you're weird
        fclose(file);
        snprintf(cmd, sizeof(cmd), "sudo chmod +x /bin/%s", chosen_file);
        run_cmd(cmd);
        //leaving runcmd hell
        printf("should be done!\n");
        exit(0);
}
 void delete_file(char chosen_file[50]) {
    char cmd[300];
    char code_for_system[70];
    printf("Deleting file...\n");
    chdir("/opt/spdr");
    snprintf(cmd, sizeof(cmd), "sudo rm -rf %s", chosen_file);
    run_cmd(cmd);
    chdir("/bin/");
    snprintf(cmd, sizeof(cmd), "sudo rm /bin/%s", chosen_file);
    run_cmd(cmd);
    printf("File deleted! Woohoo!\n");
}

void update_one_thing(char argument[60]){
    char cmd[200];
    snprintf(cmd, sizeof(cmd), "/opt/spdr/%s/info", argument); //why the hell does it need /opt/ and not opt
    FILE *file = fopen(cmd, "r");
    if (file == NULL){
        printf("We couldn't open the file in /opt/spdr/%s/info ):\n", argument);
        exit(1);
    }
    char fil_inf_str[101]; //always leave one for the null indicator!
    fgets(fil_inf_str, 100, file);
    int update_ver = atoi(fil_inf_str); //converts str to int
    fclose(file); 
    chdir("/tmp/");
    snprintf(cmd, sizeof(cmd), "sudo curl -L -o info https://github.com/mohennaceur/spdr-repo/raw/refs/heads/main/%s/info", argument);
    run_cmd(cmd);
    FILE *file2 = fopen("/tmp/info", "r");
    if(file2 == NULL){
        printf("The /tmp/info file couldn't have been opened. Sorry dude\n");
        exit(1);
    }
    char git_file_str[101];
    fgets(git_file_str, 100, file2);
    int git_update_ver = atoi(git_file_str); //converts str to int
    fclose(file2);
    if(update_ver < git_update_ver){
        printf("Outdated package detected! Replacing it with smoking hot new one...\n");
        delete_file(argument);
        installfile(argument);
    } else {
        printf("this package (%s) isn't outdated! isn't that awesome!\n", argument);
    }
    system("sudo rm /tmp/info");
}
void scan_dir(){
    chdir("/opt/spdr");
    char *path = "/opt/spdr/";  // directory to inspect

    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (entry->d_name[0] == '.' &&
            (entry->d_name[1] == '\0' || //pipelines are freakin sweet lois
             (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) {
            continue;
        }

        if (entry->d_type == DT_DIR) {//thois was mostly stolen from stack overflow
            //if you couldn't notice
            //lol
            printf("checking out %s\n", entry->d_name);
            update_one_thing(entry->d_name);
        }
    }
}
#endif //did you guys ever read what if? such a good book
////what if every rain drop was replaced with a full grown man
