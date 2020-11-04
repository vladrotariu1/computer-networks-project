#include <wait.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include "shell.h"

int logged_in = 0;


int main(int argc, char const *argv[])
{
	char shell[256] = "shell~:";
	char command[256];
	struct login_info *my_login_info = NULL;

	printf("Welcome to VLAD shell!!\nTasteaza \'help\' pentru lista de comenzi\n\n");
	printf("%s", shell);

	chdir("../home");

	while (1) {
		if (fgets(command, 256, stdin)) {
			command[strlen(command) - 1] = '\0';
			if (strcmp(command, "login") == 0) {
				logged_in = LOGIN(shell);
				printf("\n%s", shell);
			}
			else if (strcmp(command, "cuser") == 0) {
				CUSER();
				printf("\n%s", shell);
			}
			else if (strcmp(command, "quit") == 0) {
				return 0;
			}
			else {
				char *token = strtok(command, " ");

				if (strcmp (token, "myfind") == 0) {

					if (logged_in == 0) {
	                	printf("Trebuie sa fii logat pentru a executa comanda\n");
	                	printf("%s", shell);
	                	continue;
	                }

	                char *path = strtok(NULL, " ");

	                if (strtok(NULL, " ") != NULL) {
	                	printf("Prea multe argumente\n");
	                }
	                else if (path == NULL) {
	                	printf("Introduceti numele fisierului\n");
	                }
	                else {
	                	MYFIND(path);
	    			}

					printf("\n%s", shell);
				}
				else if (strcmp (token, "mystat") == 0) {

					if (logged_in == 0) {
	                	printf("Trebuie sa fii logat pentru a executa comanda\n");
	                	printf("%s", shell);
	                	continue;
	                }

	    			char *path = strtok(NULL, " ");

	                if (strtok(NULL, " ") != NULL) {
	                	printf("Prea multe argumente\n");
	                }
	                else if (path == NULL) {
	                	printf("Introduceti numele fisierului\n");
	                }
	                else {
	                	MYSTAT(path);
	    			}

					printf("\n%s", shell);
				}
				else {
					printf("Comanda %s nu eista\n", command);
					printf("%s", shell);
				}
			}
		}
	}

	return 0;
}