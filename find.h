#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int get_chmod(int permission);
void show_file_attributes (char *path);
void show_file_details (char *path);
char *new_path (char *folder_path, char *subfolder_name);
int find_file (char *path, char *file_name);
