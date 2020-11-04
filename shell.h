#include <wait.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

int LOGIN(char *shell);
void CUSER();
void MYFIND(char *path);
void MYSTAT(char *path);