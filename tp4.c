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

Pile* creerPile(T_Noeud* noeud, int N){
    Pile* new = malloc(sizeof(Pile));
    if (new!=NULL) {
        new->suivant = NULL;
        new->N = N;
        new->noeud = noeud;
    }
    return new;
}

Mot* creerMot(int ordre,int ligne,char* mot){
    Mot* new = malloc(sizeof(Mot));
    if (new!=NULL){
        new->suivant = NULL;
        new->ordre = ordre;
        new->numeroLigne = ligne;
        new->nom = malloc(strlen(mot)+1);
        strcpy(new->nom, mot);
    }
    return new;
}

Phrase* creerPhrase(int n){
    Phrase* new = malloc(sizeof(Phrase));
    if (new!=NULL){
        new->suivant = NULL;
        new->listeMot = NULL;
        new->numero = n;
    }
    return new;
}
void empiler(Pile** pile,T_Noeud* noeud, int N){
    Pile *pileInter = creerPile(noeud,N);
    pileInter->suivant = *pile;
    *pile = pileInter;
}

Pile *depiler(Pile** pile){
    Pile *pileInter = creerPile((*pile)->noeud,(*pile)->N);
    *pile = (*pile)->suivant;
    return pileInter;
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

Phrase* ajouterPhrase(Phrase** phrase, int n){
    // ajoute une nouvelle phrase de numéro n dans la liste s'il elle n'existe pas déjà et renvoie un pointeur dessus
    // si la phrase existe, la fonction permet de la rechercher
    if (*phrase == NULL){
        *phrase = creerPhrase(n);
        return *phrase;
    }
    if (n < (*phrase)->numero){
        Phrase* inte = creerPhrase(n);
        inte->suivant = *phrase;
        *phrase = inte;
        return inte;
    }
    if (n == (*phrase)->numero){
        return *phrase;
    }
    Phrase* actuelle = *phrase;
    while (actuelle->suivant != NULL){
        if(n < actuelle->suivant->numero){
            Phrase* inte = creerPhrase(n);
            inte->suivant = actuelle->suivant;
            actuelle->suivant = inte;
            return inte;
        }
        else if (n == actuelle->suivant->numero){
            return actuelle->suivant;
        }
        actuelle = actuelle->suivant;
    }
    actuelle->suivant = creerPhrase(n);
    return actuelle->suivant;
}

void ajouterMot(Phrase** phrase, int numeroLigne, int ordre, char* nom, int numeroPhrase){
    //ajoute le mot doté des caractéristiques en paramètre dans sa phrase correspondante
    Phrase* pInter = ajouterPhrase(phrase, numeroPhrase);
    Mot* mInter = pInter->listeMot;
    if (mInter == NULL){
        pInter->listeMot = creerMot(ordre, numeroLigne, nom);
        return;
    }
    if (numeroLigne < mInter->numeroLigne){
        Mot* inte = creerMot(ordre, numeroLigne, nom);
        inte->suivant = mInter;
        pInter->listeMot = inte;
        return;
    }
    else if ((numeroLigne == mInter->numeroLigne)&&(ordre <= mInter->ordre)){
        if (ordre == mInter->ordre){
            // le mot existe déjà donc pour éviter les doublons on ne le rajoute pas
            return;
        }
        Mot* inte = creerMot(ordre, numeroLigne, nom);
        inte->suivant = mInter;
        pInter->listeMot = inte;
        return;
    }
    while (mInter->suivant != NULL){
        if(numeroLigne < mInter->suivant->numeroLigne){
            Mot* inte = creerMot(ordre, numeroLigne, nom);
            inte->suivant = mInter->suivant;
            mInter->suivant = inte;
            return;
        }
        else if ((numeroLigne == mInter->suivant->numeroLigne)&&(ordre <= mInter->suivant->ordre)){
            if (ordre == mInter->suivant->ordre){
                // le mot existe déjà donc pour éviter les doublons on ne le rajoute pas
                return;
            }
            Mot* inte = creerMot(ordre, numeroLigne, nom);
            inte->suivant = mInter->suivant;
            mInter->suivant = inte;
            return;
        }
        mInter = mInter->suivant;
    }
    mInter->suivant = creerMot(ordre, numeroLigne, nom);
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
        char *motparc = NULL;
        strcpy(motmin,mot);
        ignorerCasse(motmin); //Copie en minuscule du mot entré en paramètre pour ne pas le perdre
        if (parc == NULL){
            index->racine = creerNoeud(motmin,ligne, ordre, phrase);
            index->nbMotsDistincts++;
            index->nbMotsTotal++;
            return 1;
        }
        while(parc != NULL) {
            motparc = malloc(strlen(parc->mot) + 1);
            strcpy(motparc, parc->mot);
            ignorerCasse(motparc);
            if (strcmp(motmin, motparc) < 0) { // On va à gauche
                if (parc->filsGauche == NULL) { //Si le suivant est nul
                    parc->filsGauche = creerNoeud(motmin, ligne, ordre, phrase);
                    index->nbMotsDistincts += 1;
                    index->nbMotsTotal += 1;
                    free(motparc);
                    free(motmin);
                    return 1;
                }
                parc = parc->filsGauche;
            } else if (strcmp(motmin, motparc) > 0) { //On va à droite
                if (parc->filsDroite == NULL) { //Si le suivant est nul on rajoute le mot dans l'ABR
                    parc->filsDroite = creerNoeud(motmin, ligne, ordre, phrase);
                    index->nbMotsDistincts += 1;
                    index->nbMotsTotal += 1;
                    free(motparc);
                    free(motmin);
                    return 1;
                }
                parc = parc->filsDroite;
            } else if (!strcmp(motmin, motparc)) { //Si le mot est déjà dans le ABR, on rajoute juste une occurence
                free(motparc);
                free(motmin);
                if (!ajouterPosition(parc->ListePositions, ligne, ordre, phrase)) {
                    return 0;
                }
                parc->nbOccurences += 1;
                index->nbMotsTotal += 1;
                return 1;
            }
            free(motparc);
            motparc = NULL;
        }
        free(motmin);
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
                    motPrecedent = malloc(strlen(motActuel) + 1); //On alloque la taille du mot
                    strcpy(motPrecedent, motActuel); //On copie le mot dans motPrecedent pour ne pas le perdre
                    free(motActuel);
                    motActuel = malloc(espaceMemoire);//Longueur du motPrecedent + 1, pour mettre la dernière lettre en plus
                    strcpy(motActuel, motPrecedent); //On remet le mot dans la variable actuelle
                    motActuel[espaceMemoire - 2] = caractere; //On met le nouveau caractère à la place de \0
                    motActuel[espaceMemoire - 1] = '\0'; //On replace \0 dans le texte
                    free(motPrecedent); //On libère motPrecedent pour pouvoir rajouter des lettres après
                    motPrecedent = NULL;
                }
            }
            else{
                // cas où le caractère lu n'est pas une lettre on index alors le mot est fini
                // on ne fait rien si aucun mot avant
                espaceMemoire = 1;
                if (caractere == '\n'){
                    // cas où saut de ligne : on réinitialise l'ordre et on l'ajoute au nombre total de mot
                    n+=ordre;
                    ordre = 0;
                    ligne++;
                }
                if (motActuel != NULL){ //Le mot est écrit, et on est tombés sur un caractère spécial, on l'enregistre
                    if (!ajouterOccurence(index,motActuel,ligne,ordre,phrase)){ //On ajoute le mot à l'index et on vérifie si ça a marché
                        // ajout du mot dans l'index et test si l'ajout a échoué
                        printf("\nL'ajout du mot '%s' a echoue\n", motActuel); //Message d'erreur
                    }
                    free(motActuel); //On libère la mémoire pour créer un nouveau mot après
                    motActuel=NULL;
                }
                if (caractere == '.'){
                    // cas où fin de phrase, on incrémente ainsi la phrase
                    phrase++; //On compte le passage à la phrase suivante
                }
            }
        }
        if (ordre != 0){
            n+=ordre;
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
    fclose (file);
    return n;
}//Retourne -1 si il n'arrive pas à indexer les mots




void afficherIndex(T_Index index){
    // on souhaite afficher l'index avec chacun des mots doté de leur première lettre en majuscule
    // et selon un parcourt infixe itératif de l'arbre (ordre alphabétique)
    T_Noeud *x = index.racine;
    int N = 1;
    Pile* pile = creerPile(NULL,0);
    Pile* depile = NULL;
    char* motmin  = NULL;
    T_Position *pos = NULL;
    char lettreActuelle = 0;
    while ((pile->N != 0)||((N == 1) && (x != NULL))){
        if ((N == 1)&&(x!=NULL)){
            empiler(&pile, x, 1);
            x = x->filsGauche;
        }
        else{
            depile = depiler(&pile);
            x = depile->noeud;
            N = depile->N;
            free(depile);
            depile = NULL;
            if (N==1){
                //on fait en sorte que seulement la 1ere lettre des mots soit en majuscule
                motmin = malloc(strlen(x->mot)+1);
                strcpy(motmin,x->mot);
                ignorerCasse(motmin);
                motmin[0] = (char)(motmin[0]-32);
                if (motmin[0] != lettreActuelle){
                    lettreActuelle = motmin[0];
                    printf("\n%c\n", lettreActuelle);
                }
                printf("|-- %s\n",motmin); //On affiche le mot
                free(motmin);
                motmin = NULL; // on réinitialise le pointeur associé au motmin après avoir libéré la mémoire attribuée
                pos = x->ListePositions; //On définit la position initiale
                while (pos != NULL){ //Affichage des positions de chaque apparition du mot
                    printf("|---- (l:%d, o:%d, p:%d)\n",pos->numeroLigne,pos->ordre,pos->numeroPhrase);
                    pos = pos->suivant;
                }
                printf("|\n");
                empiler(&pile,x,2);
                x = x->filsDroite;
            }
        }
    }
    while (pile !=NULL){
        // on libère la mémoire des potentiels éléments restant dans la pile
        depile = pile;
        pile = pile->suivant;
        free(depile);
    }
}

T_Noeud* rechercherMot(T_Index index, char *mot) {
    // recherche si un mot est dans l'index en ignorant la casse
    T_Noeud *x = index.racine;
    char *motmin1 = NULL;
    char *motmin2 = NULL;
    motmin1 = malloc(strlen(mot)+1);
    strcpy(motmin1,mot);
    ignorerCasse(motmin1);
    while (x!=NULL){
        motmin2 = malloc(strlen(x->mot)+1);
        strcpy(motmin2,x->mot);
        ignorerCasse(motmin2);
        if(strcmp(motmin1, motmin2)<0){
            // on va à gauche
            x = x->filsGauche;
        }
        else if(strcmp(motmin1, motmin2)>0){
            // on va à droite
            x = x->filsDroite;
        }
        else{
            //mot trouvé
            free(motmin1);
            free(motmin2);
            return x;
        }
        free(motmin2);
        motmin2 = NULL;
    }
    free(motmin1);
    return NULL;
}

void afficherOccurencesMot(T_Index index, char *mot){
    //on fait en sorte que seulement la 1ere lettre du mot soit en majuscule
    char *motmin = malloc(strlen(mot)+1);
    T_Noeud* n = rechercherMot(index, mot);
    if (n==NULL){
        printf("\nCe mot n'est pas dans le texte.");
        return;
    }
    strcpy(motmin,mot);
    ignorerCasse(motmin);
    motmin[0] = (char)(motmin[0]-32);
    printf("\nMot = \"%s\"\nOcurrences = %d", motmin, n->nbOccurences);
    free(motmin);
    //on parcourt ensuite itérativement l'arbre avec la méthode prefixe en ajoutant tous les mots de la même
    //phrase dans une structure associée
    T_Noeud * r = index.racine;
    Pile * pile = creerPile(NULL,0); // on réutilise la structure similaire à une pile (LIFO) utilisé
    // précédemment pour parcourir efficacement l'arbre
    Pile * depile = NULL;
    T_Position* pos = n->ListePositions;
    T_Position* pos2 = NULL;
    Phrase* phrase = NULL;
    Mot* motInte = NULL;
    if(r != NULL){
        empiler(&pile, r, 1);
    }
    while(pile->N != 0){
        depile = depiler(&pile);
        while(pos != NULL){
            pos2 = depile->noeud->ListePositions;
            while (pos2 != NULL){
                if (pos->numeroPhrase == pos2->numeroPhrase){
                    ajouterMot(&phrase, pos2->numeroLigne, pos2->ordre, depile->noeud->mot, pos2->numeroPhrase);
                }
                pos2 = pos2->suivant;
            }
            pos = pos->suivant;
        }
        if(depile->noeud->filsDroite != NULL){
            empiler(&pile,depile->noeud->filsDroite, 1);
        }
        if(depile->noeud->filsGauche != NULL){
            empiler(&pile,depile->noeud->filsGauche, 1);
        }
        pos = n->ListePositions;
        free(depile);
        depile = NULL;
    }
    free(pile);
    //après avoir établi la liste chainée contenant les phrases dans lesquels apparait le mot, on les affiche
    int debut;
    int numeroPhrase = 0;
    pos2 = pos;
    while ((phrase!=NULL) && (pos!=NULL)){
        numeroPhrase = pos->numeroPhrase; // on stocke le numéro de phrase actuel de l'occurrence du mot recherché
        printf("\n| Ligne %d", pos->numeroLigne);
        pos2 = pos;
        while ((pos->suivant != NULL) && (numeroPhrase == pos->suivant->numeroPhrase)){
            // cette boucle est nécessaire pour que l'affichage corresponde bien lorsqu'on a plusieurs fois le mot
            // recherché dans une phrase. (pour la ligne)
            printf("-%d",pos->suivant->numeroLigne);
            pos = pos->suivant;
        }
        pos = pos2;
        printf(", mot %d", pos->ordre);
        // on refait la boucle pour l'ordre (on aurait pu créer 2 chaines de caractères simultanément dans 1 seule boucle sinon)
        while ((pos->suivant != NULL) && (numeroPhrase == pos->suivant->numeroPhrase)){
            // cette boucle est nécessaire pour que l'affichage corresponde bien lorsqu'on a plusieurs fois le mot
            // recherché dans une phrase. (pour l'ordre)
            printf("-%d",pos->suivant->ordre);
            pos = pos->suivant;
        }
        pos = pos->suivant;
        printf(" :");
        motInte = phrase->listeMot;
        debut = 1;
        while (motInte!=NULL){
            if(debut) {
                debut = 0;
                motInte->nom[0] = (char)(motInte->nom[0]-32);
                printf(" %s", motInte->nom);
                motInte->nom[0] = (char)(motInte->nom[0]+32);
            }
            else {
                printf(" %s", motInte->nom);
            }
            motInte = motInte->suivant;
        }
        printf(".");
        phrase = phrase->suivant;
    }
    while (phrase != NULL) {
        Mot* motCourant = phrase->listeMot;
        while (motCourant != NULL) {
            Mot* motSuivant = motCourant->suivant;
            free(motCourant->nom);
            free(motCourant);
            motCourant = motSuivant;
        }

        Phrase* phraseSuivante = phrase->suivant;
        free(phrase);
        phrase = phraseSuivante;
    }
}

void construireTexte(T_Index index, char *filename){
    // similaire à afficherOccurencesMot, elle parcourt l'arbre efficacement (prefixe), puis ajoute tous les mots
    // dans les structures phrases et mot, elle réécrit ensuite dans l'ordre ces mots dans le fichier texte
    FILE *file = fopen(filename, "w");
    if (file == NULL){ //Fichier n'existe pas ou erreur à l'ouverture.
        printf("Le fichier '%s' n'a pas pu s'ouvrir.",filename);
        fclose(file);
        return;
    }
    Pile * pile = creerPile(NULL,0); // on réutilise la structure similaire à une pile (LIFO) utilisé
    // précédemment pour parcourir efficacement l'arbre
    Pile * depile = NULL;
    T_Position* pos = NULL;
    Phrase* phrase = NULL;
    Mot* motInte = NULL;
    T_Noeud * r = index.racine;
    if(r != NULL){
        empiler(&pile, r, 1);
    }
    while(pile->N != 0){
        // rangement des occurrences de tous les mots dans l'ordre à travers les structures Phrase et Mot
        depile = depiler(&pile);
        pos = depile->noeud->ListePositions;
        while(pos != NULL){
            ajouterMot(&phrase, pos->numeroLigne, pos->ordre, depile->noeud->mot, pos->numeroPhrase);

            pos = pos->suivant;
        }
        if(depile->noeud->filsDroite != NULL){
            empiler(&pile,depile->noeud->filsDroite, 1);
        }
        if(depile->noeud->filsGauche != NULL){
            empiler(&pile,depile->noeud->filsGauche, 1);
        }
        free(depile);
        depile = NULL;
    }
    int debut;
    int ancienneLigne = 1;
    int anciennePhrase = 0;
    while (phrase!=NULL){
        debut = 1;
        motInte = phrase->listeMot;
        if (anciennePhrase != phrase->numero) {
            if (anciennePhrase !=0) {
                for (int i = 0; i <= (phrase->numero - anciennePhrase - 1); i++) {
                    fputc('.', file);
                }
                fputc(' ', file);
            }
        }
        else
            fputs(". ", file);
        while (motInte!=NULL){
            if (ancienneLigne != motInte->numeroLigne)
                for (int i = 0; i <= (motInte->numeroLigne - ancienneLigne - 1); i++){
                    fputc('\n', file);
                }
            if(debut) {
                debut = 0;
                motInte->nom[0] = (char)(motInte->nom[0]-32);
                fputs(motInte->nom, file);
                motInte->nom[0] = (char)(motInte->nom[0]+32);
            }
            else {
                if (ancienneLigne == motInte->numeroLigne)
                    fputc(' ', file);
                fputs(motInte->nom, file);
            }
            ancienneLigne = motInte->numeroLigne;
            motInte = motInte->suivant;
        }
        anciennePhrase = phrase->numero;
        phrase = phrase->suivant;
        if (phrase == NULL){
            fputc('.', file);
        }
    }
    printf("\nConstruction du texte effectuee !\n");
    fclose (file);
    while (phrase != NULL) {
        Mot* motCourant = phrase->listeMot;
        while (motCourant != NULL) {
            Mot* motSuivant = motCourant->suivant;
            free(motCourant->nom);
            free(motCourant);
            motCourant = motSuivant;
        }

        Phrase* phraseSuivante = phrase->suivant;
        free(phrase);
        phrase = phraseSuivante;
    }
}

// Vider le buffer (utile quand on utlise des getchar() )
void viderBuffer() {
    int c = 0;
    while (c != '\n' && c != EOF) { // feof(stdin) renvoie une valeur non nulle si le buffer est vide
        c = getchar();
    }
}

//fonctions libératrices
void libererIndex(T_Index* index) {
    if(index == NULL){
        return;
    }
    libererNoeud(index->racine);
    free(index);
}

void libererNoeud(T_Noeud* noeud) {
    if (noeud == NULL) {
        return;
    }

    libererNoeud(noeud->filsGauche);
    libererNoeud(noeud->filsDroite);
    libererPosition(noeud->ListePositions);
    free(noeud->mot);
    free(noeud);
}

void libererPosition(T_Position* position) {
    while (position != NULL) {
        T_Position* positionSuivante = position->suivant;
        free(position);
        position = positionSuivante;
    }
}