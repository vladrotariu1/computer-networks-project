#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/dir.h>
#include<dirent.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

struct login_info {
	int logged_in;
	char *logged_as;
};

int user_exists(char *U_NAME);
int create_user();
char *get_password(int line_number);
struct login_info *login();