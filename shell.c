#include <wait.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include "login.h"
#include "find.h"

int LOGIN(char *shell) {
	printf("\n");

	pid_t PID;
	int my_pipe[2];
	struct login_info *my_login_info;

	if (pipe(my_pipe) == -1) {
		printf("Eroare la crearea pipeului\n");
	}

	switch (PID = fork()) {
		case -1:
			// Eroare

			printf("Eroare la crearea unui nou proces\n");
			break;

		case 0:
			// Proces copil

			close(my_pipe[0]); // Scriem in pipe

			// Incercam sa ne logam

			my_login_info = login();
			write(my_pipe[1], my_login_info->logged_as, strlen(my_login_info->logged_as));
			close(my_pipe[1]);
			exit(0);

		default:
			//Proces tata
			wait(NULL);
			close(my_pipe[1]); // Citim din pipe

			int aux = 0;
			char logged_as[256], chr;

			while (read(my_pipe[0], &chr, 1) == 1) {
				logged_as[aux++] = chr;
			}

			close(my_pipe[0]);
			logged_as[aux] = '\0';

			if(strlen(logged_as) != 0) {
				printf("V-ati logat cu succes\n");
				strcpy(shell, "");
				strcpy(shell, logged_as);

				int shell_len = strlen(shell);

				shell[shell_len] = '~';
				shell[shell_len + 1] = ':';
				shell[shell_len + 2] = '\0';

				return 1;
			}
			else {
				printf("Eroare. Incercati alta combinatie de username si parola.\n");
				return 0;
			}
	}
}

void CUSER() {
	printf("\n");

	pid_t PID;
	int my_pipe[2];

	if (pipe(my_pipe) == -1) {
		printf("Eroare la crearea pipeului\n");
	}

	switch (PID = fork()) {
		case -1:
			// Eroare

			printf("Eroare la crearea unui nou proces\n");
			break;

		case 0:
			// Proces copil

			close(my_pipe[0]); // Scriem in pipe

			// Incercam sa cream un nou user
			int c_user_result = create_user();
			write(my_pipe[1], &c_user_result, sizeof(int));
			close(my_pipe[1]);
			exit(0);

		default:
			// Proces tata

			wait(NULL);
			close(my_pipe[1]); // Citim din pipe

			int c_user = 0;
			read(my_pipe[0], &c_user, sizeof(int));

			if (c_user != 0) {
				printf("Eroare la crearea userului\n");
			}
			else {
				printf("User creat cu succes\n");
			}
		}

}

void MYFIND(char *path) {
	char *my_fifo = "my_fifo";

	if (mkfifo(my_fifo, 0777) < 0) {
		printf("FIFO error\n");
		exit(0);
	}


	pid_t PID = fork();

	if (PID == -1) {
		// Eroare

		printf("Eroare la crearea procesului\n");
		exit(0);
	}
	else if (PID == 0) {
		// Proces copil

		int child_fd = open(my_fifo, O_WRONLY);
		dup2(child_fd, 1);
		int file_found = find_file("./", path);

		close(child_fd);
		exit(0);
	}
	else {
		// Proces tata

		int parent_fd = open(my_fifo, O_RDONLY);
		char chr;

		while(read(parent_fd, &chr, 1) != 0) {
			printf("%c", chr);
		}

		remove(my_fifo);
	}
}

void MYSTAT(char *path) {
	pid_t PID;
	int sockp[2];

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockp) < 0) { 
        printf("Eroare: socketpair"); 
        exit(0); 
    }

    PID = fork();

    if (PID < 0) {
    	printf("Eroare la crearea procesului"); 
        exit(0); 
    }
    else if (PID == 0) {
    	// Proces copil

    	close(sockp[1]); 
        dup2(sockp[0], STDOUT_FILENO);
        show_file_attributes(path);
        close(sockp[0]);
    }
    else {
    	// Proces tata

    	char chr;
    	close(sockp[0]); 

        while(read(sockp[1], &chr, 1) != 0) {
			printf("%c", chr);
		}
 
        close(sockp[1]); 
    }

}
