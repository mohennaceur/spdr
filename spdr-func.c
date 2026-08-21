
 //man i hate easyjet. yk they dont give you charging on the flight? nor power banks? ridiculous how am i
//supposed to charge my stuff
//):<
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#ifndef SPDR_H
#define SPDR_H
void run_cmd(char cmd_argument[300]){
    pid_t pid = fork(); //fork so it works
    if (pid < 0){
        perror("Fork()");
        exit(EXIT_FAILURE); //exit( ]: )
    } else if (pid == 0){
    char contain_arg[300];
    strcpy(contain_arg, cmd_argument);
    size_t len = strlen(cmd_argument); //this being contain_arg or arg doesnt matter
    if (len > 0 && contain_arg[len - 1] == '\n') { //if it has a /n it replaces it with a null indicator
        contain_arg[len - 1] = '\0';
    }
    char *array_of_arg[300];
    int i = 0; //you know what my biggest pet peeve is? having to define what i's value is. int i should auto set to 0.
    char *token = strtok(contain_arg, " "); //this is a pointer which is cut up by spaces. this works.
    while (token != NULL && i < 299) {
       array_of_arg[i++] = token; //actually genius line
       token = strtok(NULL, " \t\n"); //turns it into array
    }
    array_of_arg[i] = NULL; // must kil arracy
    execvp(array_of_arg[0], array_of_arg);
    perror("execvp");
    exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0); //wait for baby
    }
}

void check_for_error(char *argument){ //the reason we use check-for-error is because its much more versatile
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
void check_for_404(char package[200]){
    char cmd[200];
    //this is called checking for 404 because curl gets the data from websites
    //try to curl a 404 page and you're gonna get much more data than an intended file
    //the info files are around 10 bytes and these are 300k+
    //and no, you cannot check whether the curl fails because github always sends you to a 404 screen
    //which means it will always return a success
    chdir("/tmp/"); //wrapping them all in / ever since the incident.
    //also can chdir fail? i doubt it when it has sudo powers
    snprintf(cmd, sizeof(cmd), "sudo curl -L -o info https://github.com/mohennaceur/spdr-repo/raw/refs/heads/main/%s/info", package);
    run_cmd(cmd);
    FILE *file = fopen("/tmp/info", "r");
    if (file == NULL){
        perror("file");
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file); //thank you stack overflow
    if (size > 5000) { //if its bigger than that then its def not a normal file
        //its 404!!!
        printf("The install failed ): this means the package doesn't exist.\n"); //oh my god they killed the package!
        snprintf(cmd, sizeof(cmd), "sudo rm /tmp/info"); //hacky? yes. working? i think
        run_cmd(cmd);
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);
    snprintf(cmd, sizeof(cmd), "sudo rm /tmp/info"); //hacky? yes. working? i think
    run_cmd(cmd);
    printf("this package (%s) exists!", package);
}
void installfile(char chosen_file[100]){
    char *path = "/opt/spdr/";  // directory to inspect
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("creating /opt/spdr/...\n");
        system("sudo mkdir /opt/spdr/"); //babyproofing go go go!!
    }
    char cmd[300];
    check_for_404(chosen_file); //babyproof? this is eggproof
    //we did it! we destroyed the repo! no file deletion needed
    printf("Installing stuff...\n");//, argv[2]
    chdir("/tmp/spdr-repo/");
    //welcome to run_cmd hell
    snprintf(cmd, sizeof(cmd), "curl -L -o %s.tar.gz https://github.com/mohennaceur/spdr-repo/raw/refs/heads/main/%s/%s.tar.gz", chosen_file, chosen_file, chosen_file);
    run_cmd(cmd); //verbose is pretty (:
    snprintf(cmd, sizeof(cmd), "tar -xf %s.tar.gz -C /opt/spdr", chosen_file);
    run_cmd(cmd);
    printf("making it executable...\n");
    snprintf(cmd, sizeof(cmd), "chmod +x /opt/spdr/%s/%s", chosen_file, chosen_file);
    run_cmd(cmd);
    printf("\nmaking executable in /bin/\n");
    chdir("/bin/");
    snprintf(cmd, sizeof(cmd), "sudo touch %s", chosen_file);
    run_cmd(cmd); //sequence 4
    snprintf(cmd, sizeof(cmd), "/bin/%s", chosen_file);
    FILE *file = fopen(cmd, "w"); 
    if (file == NULL){ 
        perror("file opening");
        exit(EXIT_FAILURE);
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
    printf("Deleting file...\n");
    chdir("/opt/spdr/"); //wrap EVERYTHING in slashes incase of emergency
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
        exit(EXIT_FAILURE);
    }
    char fil_inf_str[101]; //always leave one for the null indicator!
    fgets(fil_inf_str, 100, file);
    int update_ver = atoi(fil_inf_str); //converts str to int
    fclose(file); 
    chdir("/tmp/");
    snprintf(cmd, sizeof(cmd), "sudo curl -L -o info https://github.com/mohennaceur/spdr-repo/raw/refs/heads/main/%s/info", argument);
    run_cmd(cmd);
    FILE *file2 = fopen("/tmp/info", "r");
    if (file2 == NULL) {
        printf("The /tmp/info file couldn't have been opened. Sorry dude\n");
        exit(EXIT_FAILURE);
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
    system("sudo rm /tmp/info"); //having this here isn't awful or evil. it functions quickly.
    //there is no benefit nor disbenefit to using execvl instead of this
}
void scan_dir(){
    chdir("/opt/spdr"); //this safety net is pretty freakin sweet
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
    closedir(dir);
}
void list_pack() {
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
            printf("you have %s\n", entry->d_name);
        }
    }
    closedir(dir);
}
void pack_info(char package[200]){
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "/opt/spdr/%s/info", package); //why the hell does it need /opt/ and not opt
    FILE *file = fopen(cmd, "r");
    if (file == NULL){
        printf("We couldn't open the file in /opt/spdr/%s/info ):\n", package);
        printf("this means that either the package doesn't exist OR it misdownloaded\n");
        printf("cant really tell\n");
        exit(EXIT_FAILURE);
    }
    char fil_inf_str[101]; //always leave one for the null indicator!
    fgets(fil_inf_str, 100, file);
    fclose(file); 
    printf("this package exists, can be ran by typing the word %s in the terminal\n", package);
    printf("the version number is %s. spdr worked! be happy!\n", fil_inf_str);
}

void help_panel(){
    printf("hello! thanks for choosing spdr!\n");
    printf("i mean it is a proof of concept package manager and has only 2 actually useful packages\n");
    printf("but you know what they say, one mans learning project is another mans package manager\n");
    printf("the commands are:\ninfo - check info about a installed package\nhelp - you just ran it\ninstall - install a package\nsearch - check whether a package exists\nlist - check what packages you have\nremove - removes a package\nupdate - updates. you can specify a package after typing it. \nagain, thank you for choosing spdr\n");
} //longest line ever
#endif //did you guys ever read what if? such a good book
////what if every rain drop was replaced with a full grown man
