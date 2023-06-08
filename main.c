#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp4.h"


int main() {
    T_Index *index = NULL;
    int choix = 0;
    int nbmots = 0;
    char str[1000]; // initialisation de la chaine de caractères qui contiendra les noms des éléments à ajouter
    T_Noeud * noeud = NULL;
    char* motmin = NULL;
    T_Position* position = NULL;
    while (choix != '7') {
        printf("\n========== MENU UTILISATEUR ==========");
        printf("\n1. Charger un fichier");
        printf("\n2. Caracteristiques de l'index");
        printf("\n3. Afficher index");
        printf("\n4. Rechercher un mot");
        printf("\n5. Afficher les occurrences d'un mot");
        printf("\n6. Construire le texte a partir de l'index");
        printf("\n7. Quitter");
        printf("\n======================================");
        printf("\n   Votre choix ? ");
        choix = getchar();
        viderBuffer(); // supprimer de l'entrée standard le retour charriot et les éventuels caractères supplémentaires tapés par l'utilisateur

        switch (choix) {
            case '1' :
                if (index == NULL) {
                    index = creerIndex();
                    printf("\nEntrez le chemin du fichier a charger:");
                    fgets(str, 1000,
                          stdin); // récupère au maximum les 999 caractères écris dans la console et les stocke dans la variable str
                    str[strcspn(str, "\n")] = '\0'; // remplace le premier saut de ligne par le caractère de fin
                    nbmots = indexerFichier(index, str);
                    if (nbmots==-1){
                        printf("\nVeuillez reessayer");
                        libererIndex(index);
                        index = NULL;
                        break;
                    }
                    printf("\n%d mot(s) lu(s)", nbmots);
                }
                else{
                    printf("\nUn fichier a deja ete charge. Voulez vous le desindexer et en charger un autre (Y/n):");
                    choix = getchar();
                    viderBuffer();
                    if (choix != 'n') {
                        printf("\nEntrez le chemin du fichier a charger:");
                        fgets(str, 1000,
                              stdin); // récupère au maximum les 999 caractères écris dans la console et les stocke dans la variable str
                        str[strcspn(str, "\n")] = '\0'; // remplace le premier saut de ligne par le caractère de fin
                        libererIndex(index);
                        index = NULL;
                        index = creerIndex();
                        nbmots = indexerFichier(index, str);
                        if (nbmots==-1){
                            printf("\nL' ancien fichier a ete desindexer mais le nouveau fichier n'a pas ete trouve. Veuillez reessayer");
                            libererIndex(index);
                            index = NULL;
                            break;
                        }
                        printf("\n%d mot(s) lu(s)", nbmots);
                    }
                }
                break;
            case '2' :
                if (index == NULL) {
                    printf("\nAucun fichier n'a ete indexe !");
                    break;
                }
                printf("\nVoici les caracteristiques de l'index :\n\t- Nombre de mots total : %d\n\t- Nombre de mots distincts : %d", index->nbMotsTotal, index->nbMotsDistincts);
                break;
            case '3' :
                if (index == NULL) {
                    printf("\nAucun fichier n'a ete indexe !");
                    break;
                }
                afficherIndex(*index);
                break;
            case '4' :
                if (index == NULL) {
                    printf("\nAucun fichier n'a ete indexe !");
                    break;
                }
                printf("\nEntrez le mot a rechercher:");
                fgets(str, 1000,
                      stdin); // récupère au maximum les 999 caractères écris dans la console et les stocke dans la variable str
                str[strcspn(str, "\n")] = '\0'; // remplace le premier saut de ligne par le caractère de fin
                if ((noeud = rechercherMot(*index, str)) == NULL){
                    printf("\nCe mot n'est pas dans le texte");
                    break;
                }
                motmin = malloc(strlen(noeud->mot)+1);
                strcpy(motmin,noeud->mot);
                ignorerCasse(motmin);
                motmin[0] = (char)(motmin[0]-32);
                printf("%s\n",motmin); //On affiche le mot
                free(motmin);
                motmin = NULL; // on réinitialise le pointeur associé au motmin après avoir libéré la mémoire attribuée
                position = noeud->ListePositions; //On définit la position initiale
                while (position != NULL){ //Affichage des positions de chaque apparition du mot
                    printf("|-- (l:%d, o:%d, p:%d)\n",position->numeroLigne,position->ordre,position->numeroPhrase);
                    position = position->suivant;
                }
                break;
            case '5' :
                if (index == NULL) {
                    printf("\nAucun fichier n'a ete indexe !");
                    break;
                }
                printf("\nEntrez le mot pour lequel il faut afficher les phrases ou il apparait:");
                fgets(str, 1000,
                      stdin); // récupère au maximum les 999 caractères écris dans la console et les stocke dans la variable str
                str[strcspn(str, "\n")] = '\0'; // remplace le premier saut de ligne par le caractère de fin
                afficherOccurencesMot(*index,str);
                break;
            case '6' :
                if (index == NULL) {
                    printf("\nAucun fichier n'a ete indexe !");
                    break;
                }
                printf("\nEntrez le chemin du fichier dans lequel sera enregistre le texte:");
                fgets(str, 1000,
                      stdin); // récupère au maximum les 999 caractères écris dans la console et les stocke dans la variable str
                str[strcspn(str, "\n")] = '\0'; // remplace le premier saut de ligne par le caractère de fin
                construireTexte(*index,str);
                break;
            case '7' :
                printf("\n========== PROGRAMME TERMINE ==========\n");
                if (index != NULL) {
                    libererIndex(index);
                }
                break;
            default :
                printf("\n\nERREUR : votre choix n'est pas valide ! ");

        }
        printf("\n");
    }
    return 0;
}
