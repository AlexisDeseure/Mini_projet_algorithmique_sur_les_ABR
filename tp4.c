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

T_Noeud * creerNoeud(char* mot, T_Position* debut){
    T_Noeud *new = malloc(sizeof(T_Noeud));
    if (new!=NULL) {
        //allocation réussi
        new->mot = malloc(strlen(mot) + 1);
        strcpy(new->mot,mot);
        new->ListePositions = debut;
        new->nbOccurences = 1;
        //un noeud ne peut exister sans occurence
        new->filsDroite = NULL;
        new->filsGauche = NULL;
    }
    return new;
}

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
        while(pos_int->suivant != NULL && (pos_int->suivant->numeroLigne<P->numeroLigne || pos_int->suivant->ordre<P->ordre) ){
            pos_int=pos_int->suivant;
        }
        P->suivant=pos_int->suivant;
        pos_int->suivant=P;
    }
    return listeP;
}

int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase){
    T_Noeud *parc = index->racine;
    char *motparc;
    char *motmin = mot;
    T_Position *P;
    ignorerCasse(motmin); //Copie en minuscule du mot entré en paramètre pour ne pas le perdre
    if (index == NULL){
        P = creerPosition(ligne,ordre,phrase);
        index->racine = creerNoeud(mot,P);
        index->nbMotsDistincts=1;
        index->nbMotsTotal=1;
        return 1;
    }
    else{
        while(parc->filsDroite!=NULL || parc->filsGauche!=NULL){
            int len = strlen(parc->mot);
            char *motparc = malloc(len*sizeof(char));
            strcpy(motparc,parc->mot);
            ignorerCasse(motparc);
            if(strcmp(motmin,motparc)<0){
                parc = parc->filsGauche;
                free(motparc);
            }
            else if(strcmp(motmin,motparc)>0){
                parc = parc->filsDroite;
                free(motparc);
            }
            else if(!strcmp(motmin,motparc)){
                ajouterPosition(parc->ListePositions, ligne, ordre, phrase);
                parc->nbOccurences +=1;
                return 1;
            }
        }
        return 0;
    }
}
