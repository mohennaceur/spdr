
#ifndef SPDRLIB_H
#define SPDRLIB_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
void run_cmd(char *cmd_argument);
void check_for_error(void *argument);
void check_pkg_size(char *pkg);
FILE *open_file(char filename[], int file_type);
void check_for_404(char *package);
void verify_inst(char *chosen_file);
void check_for_prev_inst(char *package, int installing);
void installfile(char *chosen_file);
void delete_file(char *chosen_file);
void update_one_thing(char *argument);
void scan_dir();
void list_pack();
void pack_info(char *package);
void help_panel();
#endif // SPDRLIB_H