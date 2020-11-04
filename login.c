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

int user_exists(char *U_NAME) {
	/*
		Functia testeaza daca userul exista in fisierul 'users'.
		Daca exista va returna numarul liniei pe care acesta se afla
	*/

	FILE *user_file = fopen("../root/users", "r");
	char *users;
	size_t size = 0;
	int line_number = 0;

	//	Se citeste fisierul linie cu linie pana cand se ajunge la ultima linie
	//	sau pana cand este gasit userul in fisier
	while (getline(&users, &size, user_file) != -1) {
		users[strlen(users) - 1] = '\0';
		line_number++;

		if (strcmp(U_NAME, users) == 0) {
			return line_number;
		}
	}

	fclose(user_file);

	return 0;
}

int create_user() {
	/* 
		Functia creaza un nou user in fisierul 'users' si 
		o parola aferenta in fisierul 'passwd'
	*/

	char U_NAME[256];
	char PASSWD[256];

	FILE *user_file = fopen("../root/users", "a");
	FILE *passwd_file = fopen("../root/passwd", "a");

	printf("Introduceti usernameul: ");
	scanf("%s", U_NAME);

	//Fortam userul sa creeze un user care nu se afla in fisierul 'users'
	while (user_exists(U_NAME) > 0) {
		printf("Userul exista. Alegeti altul.\n");
		printf("Enter username: ");
		scanf("%s", U_NAME);
	}

	fprintf(user_file, "%s\n", U_NAME);
	fclose(user_file);

	printf("Introduceti parola: ");
	scanf("%s", PASSWD);
	fprintf(passwd_file, "%s\n", PASSWD);
	fclose(passwd_file);

	return 0;
}

char *get_password(int line_number) {
	/* 
		Functia returneaza parola de pe linia cu numarul dat ca parametru
	*/

	FILE *passwd_file = fopen("../root/passwd", "r");
	char *passwd;
	size_t size = 0;

	for (int i = 0; i < line_number; i++) {
		getline(&passwd, &size, passwd_file);
	}

	passwd[strlen(passwd) - 1] = '\0';
	return passwd;
}

struct login_info *login() {
	/* 
		Functia se ocupa de sistemul de logare 
	*/

	char U_NAME[256];
	char PASSWD[256];
	struct login_info *my_login_info = malloc(sizeof(struct login_info));
	my_login_info->logged_in = 1;
	my_login_info->logged_as = malloc(256);

	FILE *user_file = fopen("../root/users", "a");
	FILE *passwd_file = fopen("../root/passwd", "a");

	printf("Introduceti usernameul: ");
	scanf("%s", U_NAME);

	printf("Introduceti parola: ");
	scanf("%s", PASSWD);

	int line_number = user_exists(U_NAME);

	if (line_number == 0) {
		return NULL;
	}
	else {
		if (strcmp(PASSWD, get_password(line_number)) == 0) {
			my_login_info->logged_in = 1;
			strcpy(my_login_info->logged_as, U_NAME);

			return my_login_info;
		}
		else {
			return NULL;
		}
	}
}