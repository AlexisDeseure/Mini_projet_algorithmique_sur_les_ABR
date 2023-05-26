#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp4.h"
T_Position* creerPosition(int ligne, int ordre, int phrase){
    T_Position *new = malloc(sizeof(T_Position));
    if (new!=NULL) {
        //allocation réussi
        new->suivant = NULL;
        new->numeroLigne = ligne;
        new->ordre = ordre;
        new->numeroPhrase = phrase;
    }
    return new;
}


T_Noeud * creerNoeud(char* mot, int ligne, int ordre, int phrase){
    T_Noeud *new = malloc(sizeof(T_Noeud));
    T_Position *P = creerPosition(ligne, ordre, phrase);
    if (new!=NULL) {
        //allocation réussi
        new->mot = malloc(strlen(mot) + 1);
        strcpy(new->mot,mot);
        new->ListePositions = P;
        new->nbOccurences = 1;
        //un noeud ne peut exister sans occurence
        new->filsDroite = NULL;
        new->filsGauche = NULL;
    }
    return new;
}//Crée un noeud, et rajoute une position en son début
//On ne se sert plus d'une position en entrée, on met toutes les données du noeud, cela permet à faire moins de lignes de code quand on veut créer un nouveau noeud, car on doit pas créer la position à chaque fois

T_Index* creerIndex() {
    T_Index * new= malloc(sizeof(T_Index));
    if (new!=NULL){
        //allocation réussi
        new->nbMotsDistincts = 0;
        new->nbMotsTotal = 0;
        new->racine = NULL;
    }
    return new;
}

void ignorerCasse(char* mot){
    if (mot == NULL || strcmp(mot, "\0") == 0){
        return;
    }
    int index = 0;
    while (mot[index]!='\0'){
        if (mot[index]>=65 && mot[index]<=90){
            mot[index]+=32;
        }
        index++;
    }
}

T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase){
    T_Position *pos_int = listeP;
    T_Position *P = creerPosition(ligne,ordre,phrase);
    if(listeP==NULL){
        return P;
    }
    else{
        while(pos_int->suivant != NULL && (pos_int->suivant->numeroLigne<=P->numeroLigne) && ((pos_int->suivant->numeroLigne != P->numeroLigne)||(pos_int->suivant->ordre<P->ordre))){
            pos_int=pos_int->suivant;
        }
        P->suivant=pos_int->suivant;
        pos_int->suivant=P;
    }
    return listeP;
}

int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase){
    if (index == NULL){
        return 0;
    }
    else{
        T_Noeud *parc = index->racine;
        char *motmin = malloc(strlen(mot)+1);
        strcpy(motmin,mot);
        ignorerCasse(motmin); //Copie en minuscule du mot entré en paramètre pour ne pas le perdre
        if (parc == NULL){
            index->racine = creerNoeud(mot,ligne, ordre, phrase);
            index->nbMotsDistincts++;
            index->nbMotsTotal++;
            return 1;
        }
        while(parc != NULL){
            char *motparc = malloc(strlen(parc->mot)+1);
            strcpy(motparc,parc->mot);
            ignorerCasse(motparc);
            if(strcmp(motmin,motparc)<0){ // On va à gauche
                if(parc->filsGauche == NULL){ //Si le suivant est nul
                    parc->filsGauche = creerNoeud(mot,ligne,ordre,phrase);
                    index->nbMotsDistincts +=1;
                    index->nbMotsTotal +=1;
                    return 1;
                }
                parc = parc->filsGauche;
                free(motparc);
            }
            else if(strcmp(motmin,motparc)>0){ //On va à droite
                if(parc->filsDroite == NULL){ //Si le suivant est nul on rajoute le mot dans l'ABR
                    parc->filsDroite = creerNoeud(mot,ligne,ordre,phrase);
                    index->nbMotsDistincts +=1;
                    index->nbMotsTotal +=1;
                    return 1;
                }
                parc = parc->filsDroite;
                free(motparc);
            }
            else if(!strcmp(motmin,motparc)){ //Si le mot est déjà dans le ABR, on rajoute juste une occurence
                if (!ajouterPosition(parc->ListePositions, ligne, ordre, phrase)){
                    return 0;
                }
                parc->nbOccurences +=1;
                index->nbMotsTotal +=1;
                return 1;
            }
        }
        return 0;
    }
}//Permet d'ajouter un mot dans l'ABR. Si le mot existe déjà, on ajoute sa position dans la liste de position du mot, sinon, on l'ajoute dans le ABR.

int indexerFichier(T_Index *index, char *filename){
    int n = 0; //Nombre de mots dans le texte
    FILE *file = fopen(filename, "r");
    if (file == NULL){ //Fichier n'existe pas ou erreur à l'ouverture.
        printf("Le fichier '%s' n'a pas pu s'ouvrir.",filename);
        fclose(file);
        return -1;
    }
    else{
        // ouverture réussie : traitement du fichier
        int ligne = 1;
        int ordre = 0;
        int phrase = 1;
        char caractere = 0; //contiendra le caractère lu
        char* motActuel = NULL;
        char* motPrecedent = NULL;
        int espaceMemoire = 1; //sert à stocker le nombre d'espace à allouer pour les chaines de caractères des mots
        while((caractere = (char)fgetc(file))!= EOF){
            //parcourt du fichier carctère par caractère
            if (((caractere<='z') && (caractere>='a'))||((caractere<='Z') && (caractere>='A'))) {
                if (espaceMemoire == 1) {
                    ordre++;
                }
                espaceMemoire++;
                if (motActuel == NULL) {
                    motActuel = malloc(espaceMemoire);
                    motActuel[espaceMemoire - 2] = caractere;
                    motActuel[espaceMemoire - 1] = '\0';
                } else {
                    motPrecedent = malloc(strlen(motActuel) + 1);
                    strcpy(motPrecedent, motActuel);
                    free(motActuel);
                    motActuel = malloc(espaceMemoire);
                    strcpy(motActuel, motPrecedent);
                    motActuel[espaceMemoire - 2] = caractere;
                    motActuel[espaceMemoire - 1] = '\0';
                    free(motPrecedent);
                    motPrecedent = NULL;
                }
            }
            else{
                // cas où le caractère lu n'est pas une lettre on index alors le mot
                // qui s'est terminé ou on ne fait rien si aucun mot avant
                espaceMemoire = 1;
                if (caractere == '.'){
                    // cas où fin de phrase, on incrémente ainsi la phrase
                    phrase++;
                }
                if (caractere == '\n'){
                    // cas où saut de ligne : on réinitialise l'ordre et on l'ajoute au nombre total de mot
                    n+=ordre;
                    ordre = 0;
                    ligne++;
                }
                if (motActuel != NULL){
                    if (!ajouterOccurence(index,motActuel,ligne,ordre,phrase)){
                        // ajout du mot dans l'index et test si l'ajout a échoué
                        printf("\nL'ajout du mot '%s' a echoue\n", motActuel);
                    }
                    free(motActuel);
                    motActuel=NULL;
                }
            }
        }
        if (ordre != 0){
            n+=ordre;
        }
    }
    fclose (file);
    return n;
}//Retourne -1 si il n'arrive pas à indexer les mots

void afficherIndex(T_Index index){ // Il reste que à faire l'affichage formatté
    T_Noeud *n = index.racine;
    T_Index parcourir = index;
    if (n != NULL){
        parcourir.racine = n->filsGauche;
        afficherIndex(parcourir); //Récursivité, on affiche les sous-arbres gauche

        printf("|--%s\n",n->mot); //On affiche le mot
        T_Position *pos = n->ListePositions; //On définit la position initiale
        while (pos != NULL){ //Affichage des positions de chaque apparition du mot
            printf("|----(l:%d, o:%d, p:%d)\n",pos->numeroLigne,pos->ordre,pos->numeroPhrase);
            pos = pos->suivant;
        }

        parcourir.racine = n->filsDroite;
        afficherIndex(parcourir); //Récursivité, on affiche les sous-arbres droits
    }
}