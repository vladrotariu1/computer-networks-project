#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int get_chmod(int permission){
    /*
        Functia returneaza drepturile de acces ale unui fisier
    */

    int user = 0;
    int group = 0;
    int other = 0;
    int binary[3] = {0};
    int pow = 1;

    permission -= 32768;


    // Other
    for (int i = 0; i < 3; i++) {
        binary[i] = permission % 2;
        permission /= 2;
    }

    for (int i = 0; i < 3; i++) {
        other += pow * binary[i];
        pow *= 2;
    }

    pow = 1;


    // Group
    for (int i = 0; i < 3; i++) {
        binary[i] = permission % 2;
        permission /= 2;
    }

    for (int i = 0; i < 3; i++) {
        group += pow * binary[i];
        pow *= 2;
    }

    pow = 1;


    // User
    for (int i = 0; i < 3; i++) {
        binary[i] = permission % 2;
        permission /= 2;
    }

    for (int i = 0; i < 3; i++) {
        user += pow * binary[i];
        pow *= 2;
    }

    pow = 1;

    return user * 100 + group * 10 + other;
}

void show_file_attributes (char *path) {
    /*
        Functia afiseaza detaliile unui fisier in aceasta ordine: 
            
            -data crearii
            -data modificarii
            -dimensiunea fisierului
            -drepturile de access asupra fisierului
    */

    struct stat st;

    if (stat(path, &st) != 0) {
        printf("Fisierul nu exista\n");
    }
    else {
        printf("- id-ul deviceului ce contine fisierul: %li\n", st.st_dev);
        printf("- uid-ul ownerului                    : %i\n", st.st_uid);
        printf("- gid-ul ownerului                    : %i\n", st.st_gid);
        printf("- data crearii                        : %s", asctime(gmtime(&st.st_ctime)));
        printf("- data modificarii                    : %s", asctime(gmtime(&st.st_mtime)));
        printf("- dimensiunea                         : %li BYTES\n", st.st_size);
        printf("- drepturi de acces asupra fisierului : %i\n\n", get_chmod(st.st_mode));
    }
}

void show_file_details (char *path) {
    /*
        Functia afiseaza detaliile unui fisier in aceasta ordine: 
            
            -data crearii
            -data modificarii
            -dimensiunea fisierului
            -drepturile de access asupra fisierului
    */

    struct stat st;

    if (stat(path, &st) != 0) {
        printf("Eroare la accesarea datelor fisierului %s\n", path);
    }
    else {
        printf("- data crearii                       : %s", asctime(gmtime(&st.st_ctime)));
        printf("- data modificarii                   : %s", asctime(gmtime(&st.st_mtime)));
        printf("- dimensiunea                        : %li BYTES\n", st.st_size);
        printf("- drepturi de acces asupra fisierului: %i\n\n", get_chmod(st.st_mode));
    }
}

char *new_path (char *folder_path, char *subfolder_name) {
    /*
        Functia creaza un nou path de tipul: folder_path/subfolder_name/
    */

    char *path = malloc(256);
    strcpy(path, folder_path);
    strcat(path, subfolder_name);
    strcat(path, "/");

    path[strlen(folder_path) + strlen(subfolder_name) + 1] = '\0';

    return path;
}

int find_file (char *path, char *file_name) {
    /*
        Functia cauta un fisier in arborele de directoare
    */

    DIR *directory;
    directory = opendir(path);

    struct dirent *dirp;
    int file_found = 0;

    // Se citesc elementele directorului
    while ((dirp = readdir(directory)) != NULL) {

        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) {
            continue;
        }

        // d_type = 4 inseamna director
        if (dirp->d_type == 4) {

            char *subfolder_path = new_path(path, dirp->d_name);

            // Se incepe cautarea recursiva in subfolderul folderului curent
            if (find_file(subfolder_path, file_name) != 0) {
                file_found = 1;
            }
        }
        // d_type != 4 inseamna fisier
        else {
            // daca numele fisierului este 'file_name' se afiseaza pathul si informatii

            if (strcmp(file_name, dirp->d_name) == 0) {
                char *file_path = new_path(path, dirp->d_name);
                file_path[strlen(file_path) - 1] = '\0';

                printf("%s\n", file_path);
                show_file_details(file_path);
            	closedir(directory);
                return 1;
            }
        }
    }
    closedir(directory);

    return file_found;
}