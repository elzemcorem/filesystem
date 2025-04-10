#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/func.h"

int main() {
    if (load_disk("vdisk.img") != 0) {
        return 1;
    }

    int choix;
    char nom[32];
    char contenu[512];
    
    while (1) {
        printf("1. Lister les fichiers\n");
        printf("2. Créer un fichier\n");
        printf("3. Écrire un fichier\n");
        printf("4. Lire un fichier\n");
        printf("5. Quitter\n");
        printf("Choisissez une option: ");
        scanf("%d", &choix);
        getchar(); // Consomme le '\n' après le scanf

        switch (choix) {
            case 1:
                list_files();
                break;
            case 2:
                printf("Nom du fichier: ");
                fgets(nom, 32, stdin);
                nom[strcspn(nom, "\n")] = '\0'; // Enlever le '\n'
                create_file(nom);
                break;
            case 3:
                printf("Nom du fichier à écrire: ");
                fgets(nom, 32, stdin);
                nom[strcspn(nom, "\n")] = '\0';
                printf("Contenu à écrire: ");
                fgets(contenu, 512, stdin);
                write_file(nom, contenu, strlen(contenu));
                break;
            case 4:
                printf("Nom du fichier à lire: ");
                fgets(nom, 32, stdin);
                nom[strcspn(nom, "\n")] = '\0';
                read_file(nom, contenu, sizeof(contenu));
                printf("Contenu du fichier: %s\n", contenu);
                break;
            case 5:
                return 0;
            default:
                printf("Option invalide\n");
        }
    }

    return 0;
}

