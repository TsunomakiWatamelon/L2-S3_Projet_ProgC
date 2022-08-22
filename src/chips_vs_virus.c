#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <MLV/MLV_all.h>
#include "structure.h"
#define E 1
#define D 2
#define S 3
#define M 4
#define B 5

#define A 1
#define R 2
#define P 3
#define X 4
#define F 5


#define priceChips1 100
#define priceChips2 150
#define priceChips3 300
#define priceChips4 50
#define priceChips5 125

#define max_pos 30
#define TAILLE_X 1000
#define TAILLE_Y 600

Game* allouePartie(int argent){
    /*allouePartie, renvoie de la mémoire allouée pour un pointeur de type game lui associe aussi un certain montant d'argent pris en parametre
    *
    * int allouePartie(
    *   int argent
    * );
    */
    Game *new = (Game *)malloc(sizeof(Game));
    if (new == NULL){
        fprintf(stderr, "allouePartie: echec d'allocation \n");
        exit(EXIT_FAILURE);
    }
    new->virus = NULL;
    new->chips = NULL;
    new->turn = 1;
    new->money = argent;
    return new;
}

Chips* alloueChips(int type, int ligne, int position, int prix){
    /* alloueChips, renvoie de la mémoire allouée pour un pointeur de type Chips, et lui associe les données pris en paramètre
    *
    * int alloueChips(
    *   int argent
    *   int type
    *   int position
    *   int prix
    * );
    */
    Chips* new = (Chips *)malloc(sizeof(Chips));
    if (new == NULL){
        fprintf(stderr, "alloueChips: echec d'allocation \n");
        exit(EXIT_FAILURE);
    }
    new->type = type;
    switch(type){
        case A:
            new->life = 3;
            break;
        case R:
            new->life = 4;
            break;
        case P:
            new->life = 4;
            break;
        case X:
            new->life = 4;
            break;
        case F:
            new->life = 7;
            break;
    }
    new->line = ligne;
    new->position = position;
    new->price = prix;
    new->next = NULL;
    return new;
}

Virus* alloueVirus(int type, int ligne, int position, int tour){
    /* alloueVirus, renvoie de la mémoire allouée pour un pointeur de type Virus, et lui associe les données pris en paramètre
    *
    * int alloueVirus(
    *   int type
    *   int ligne
    *   int position
    *   int tour
    * );
    */
    Virus *new = (Virus *)malloc(sizeof(Virus));
    if(new == NULL){
        fprintf(stderr, "alloueVirus: echec d'allocation \n");
        exit(EXIT_FAILURE);
    }
    new->type = type;
    switch(type){
        case E:
            new->life = 3;
            new->speed = 2;
            break;
        case D:
            new->life = 6;
            new->speed = 1;
            break;
        case S:
            new->life = 4;
            new->speed = 4;
            break;
        case M:
            new->life = 4;
            new->speed = 2;
            break;
        case B:
            new->life = 4;
            new->speed = 2;
            break;
    }
    new->line = ligne;
    new->position = position;
    new->turn = tour;
    new->next = NULL;
    new->next_line = NULL;
    new->prev_line = NULL;
    return new;
}

int insereChrono(Virus** virus, Virus* newVirus){
    /* insereChrono, ajoute à la fin de la liste virus le nouveau élement newVirus (ajout chronologique suivant les vagues)
    *
    * int insereChrono(
    *   Virus** virus
    *   Virus* newVirus
    * );
    */
    if (*(virus) == NULL){
        *(virus) = newVirus;
    }
    else{
        Virus* current = *(virus);
        while (current->next != NULL){ // boucle pour arriver au dernier chronologique
            current = current->next;
        }
        current->next = newVirus;
    }
    return 1;
}

int insereLigne(Virus* newVirus, Virus* linevirus[7], int ligne){
    /* insereLigne, ajoute à la fin d'une des listes correspondant aux chainage par ligne
    *
    * int insereLigne(
    *   Virus* newVirus
    *   Virus* linevirus[7]
    *   int ligne
    * );
    */
    if ((ligne <1) || (ligne > 7)){
        fprintf(stderr, "insereligne : ligne invalide (%d)\n", ligne);
        return 0;
    }
    if (linevirus[ligne - 1] == NULL) { // newVirus est premier de sa ligne
        linevirus[ligne - 1] = newVirus;
        return 1; // on sort
    }
    Virus* current = linevirus[ligne - 1];
    while (current->next_line != NULL){ // Boucle pour arriver au dernier de la ligne
        current = current->next_line;
    }
    current->next_line = newVirus;
    newVirus->prev_line = current;
    return 1;
}

int insereChips(Chips** chips, char type, int ligne, int position, int price){
    /* insereChips, ajoute à la fin de la liste chips un chip correspondant aux paramètres
    *
    * int insereChips(
    *   Chips** chips
    *   char type
    *   int ligne
    *   int price
    * );
    */
    Chips* newChips;
    int typeChips;
    switch(type){
        case 'A':
            typeChips = A;
            break;
        case 'R':
            typeChips = R;
            break;
        case 'P':
            typeChips = P;
            break;
        case 'X':
            typeChips = X;
            break;
        case 'F':
            typeChips = F;
            break;
        default:
            fprintf(stderr, "insereChips : type invalide (%c)\n", type);
            exit(EXIT_FAILURE);
    }
    newChips = alloueChips(typeChips, ligne, position, price); 
    if ((ligne < 1) || (ligne > 7)){
        fprintf(stderr, "insereChips : ligne invalide (%d)\n", ligne);
        return 0;
    }
    if (*(chips) == NULL){
        *(chips) = newChips;
    }
    else{
        Chips* current = *(chips);
        while (current->next != NULL){ // boucle pour arriver au dernier chronologique
            current = current->next;
        }
        current->next = newChips;
    }
    return 1;
}

int insereVirus(Virus** Origin, char type, int ligne, int tour, Virus* linevirus[7]){
    /* insereVirus, fonction d'insertion générale pour les insertion de virus (chrono et par ligne)
    *
    * int insereVirus(
    *   Virus** Origin
    *   char type
    *   int ligne
    *   int tour
    *   Virus* lignevirus[7]
    * );
    */
    Virus* newVirus;
    int typeVirus;
    switch(type){
        case 'E':
            typeVirus = E;
            break;
        case 'D':
            typeVirus = D;
            break;
        case 'S':
            typeVirus = S;
            break;
        case 'M':
            typeVirus = M;
            break;
        case 'B':
            typeVirus = B;
            break;
        default:
            fprintf(stderr, "insereVirus : type invalide (%c)\n", type);
            exit(EXIT_FAILURE);
    }
    newVirus = alloueVirus(typeVirus, ligne, max_pos, tour);
    if (insereLigne(newVirus, linevirus, ligne)){
        insereChrono(Origin, newVirus);
        return 1;
    }else {
        fprintf(stderr, "insereVirus : echec insereligne\n");
        exit(EXIT_FAILURE);
    }
}

int initTableau(char tab[][max_pos + 1]){
    /* initTableau, remet à "blanc" un tableau 2D de char (permet d'aider l'affichage ascii et graphique)
    *
    * int initTableau(
    *   char tab[][max_pos + 1]
    * );
    */
    for (int i = 0; i < 7; i++){
        for (int k = 0; k <= max_pos; k++){
            tab[i][k] = ' ';
        }
    }
    return 1;
}

int chipsTableau(char tab[][max_pos + 1], Chips* chips){
    /* chipsTableau, pose les char correspondant aux chips dans le tableau tab
    *
    * int chipsTableau(
    *   char tab[][max_pos + 1]
    *   Chips* chips
    * );
    */
    Chips* current = chips;
    while (current != NULL){
        if (current->type == 1){
            tab[(current->line) - 1][current->position] = 'A';
        }
        if (current->type == 2){
            tab[(current->line) - 1][current->position] = 'R';
        }
        if (current->type == 3){
            tab[(current->line) - 1][current->position] = 'P';
        }
        if (current->type == 4){
            tab[(current->line) - 1][current->position] = 'X';
        }
        if (current->type == 5){
            tab[(current->line) - 1][current->position] = 'F';
        }
        if (current->type < 1 || current->type > 5){
            fprintf(stderr, "chipsTableau: type chips inconnu (%d)\n", current->type);
            return 0;
        }
        current = current->next;
    }
    return 1;
}

void virusTableau(char tab[][max_pos + 1], Virus** virus){
    /* virusTableau, pose les char correspondant aux virus dans le tableau tab
    *
    * int virusTableau(
    *   char tab[][max_pos + 1]
    *   Chips* chips
    * );
    */
    Virus* current = *(virus);
    while (current != NULL){
        if (current->type == 1){
            tab[(current->line) - 1][current->position] = 'E';
        }
        if (current->type == 2){
            tab[(current->line) - 1][current->position] = 'D';
        }
        if (current->type == 3){
            tab[(current->line) - 1][current->position] = 'S';
        }
        if (current->type == 4){
            tab[(current->line) - 1][current->position] = 'M';
        }
        if (current->type == 5){
            tab[(current->line) - 1][current->position] = 'B';
        }
        if (current->type < 1 || current->type > 5){
            fprintf(stderr, "virusTableau: type virus inconnu (%d)\n", current->type);
            exit(EXIT_FAILURE);
        }
        current = current->next;
    }
}

int nombreVague(char* fichier, int* vaguemax){
    /* chipsTableau, pose les char correspondant aux chips dans le tableau tab
    *
    * int insereLigne(
    *   char tab[][max_pos + 1]
    *   Chips* chips
    * );
    */
    char singleLine[7], nbvague;
    FILE* fPointer;
    fPointer = fopen(fichier, "r");
    fgets(singleLine, 7, fPointer); // premier ligne, ne sert à rien pour connaître le nombre de vague
    while (!feof(fPointer)){
        fgets(singleLine, 7, fPointer);
        nbvague = atoi(&singleLine[0]);
    }
    *vaguemax = nbvague;
    return 1;
}

void insereVague(char* fichier, Virus** Origin, int tour, Virus* linevirus[7]){
    FILE* fPointer;
    char prev = '0';
    char prev2 = '0';
    fPointer = fopen(fichier, "r");
    char singleLine[7];
    fgets(singleLine, 7, fPointer); // premiere ligne, ne sert à rien pour le spawn de virus
    while (!feof(fPointer)){
        fgets(singleLine, 7, fPointer);
        if ((atoi(&singleLine[0]) == prev) && (atoi(&singleLine[2]) == prev2)) break;
        if (atoi(&singleLine[0]) == tour){
            insereVirus(Origin, singleLine[4], atoi(&singleLine[2]), tour, linevirus);
        }
        prev = atoi(&singleLine[0]);
        prev2 = atoi(&singleLine[2]);

    }
}

int conditionFinPartie(Virus* virus, int tour, int vaguemax){
    int cpt=0;
    Virus* current = virus;
    if (tour >= vaguemax){
        if (current == NULL){
            return 2;
        }
        else if (current != NULL){
            if (current->position == 0) return 1;
            while (current->next != NULL){
                if (current->position == 0) return 1;
                cpt ++;
                current = current->next;
            }
        }
        if (cpt == 0) return 2;
    }
    return 0;
}

int cash(char* fichier){
    FILE* fPointer;
    fPointer = fopen(fichier, "r");
    char singleLine[100];
    fgets(singleLine, 100, fPointer);
    int cash = atoi(singleLine);
    return cash;
}

void affichePlateau(int tour, char tab[][max_pos + 1]){
    int i, j, ligne;
    printf("\nGame turn %d\n", tour);
    printf("\n");
    for (i=0; i<7; i++){
        ligne = i+1;
        printf("%d| ", ligne);
        for (j=0; j <= max_pos; j++){
            printf("%c", tab[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void freeVirus(Virus* virus){
    free(virus);
}

void chipAtk(int dmg, int line, Virus** Origin, Virus* target, Virus* linevirus[7]){
    if (target == NULL) return;
    if ((target->life) - dmg <= 0){
        linevirus[line - 1] = (linevirus[line - 1])->next_line;
        if (linevirus[line - 1] != NULL)(linevirus[line - 1])->prev_line = NULL;
        Virus* current = *(Origin);
        if (*(Origin) == target){
            *(Origin) = (*(Origin))->next;
            freeVirus(target);
            return;
        }
        while(current->next != NULL){
            if (current->next == target){
                current->next = target->next;
                freeVirus(target);
                return;
            }
            current = current->next;
        }
        fprintf(stderr, "chipAtk: virus mort introuvable \n");
        exit(EXIT_FAILURE);
    }
    else {
        (target->life) = (target->life) - dmg;
        return;
    }
}

void chipSlow(Virus* target){
    if (target == NULL) return;
    target->speed = 1;
}

void actionChip(Chips* chip, Virus** Origin, Virus* linevirus[7]){
    if (chip == NULL) return;
    int dmg;
    int slow;
    if (chip != NULL){
        slow = 0;
        switch(chip->type){
            case A:
                dmg = 1; // sentry standard
                break;
            case R:
                dmg = 1; // sentry slow
                slow = 1;
                break;
            case P:
                dmg = 1; // sentry standard
                break;
            case X:
                dmg = 2; // sentry elite
                break;
            case F:
                dmg = 0; // wall
                break;
        }
        if (linevirus[chip->line - 1] == NULL){
            return;
        }
        else{
            Virus* target = linevirus[chip->line - 1];
            if (slow) chipSlow(target);
            if (dmg > 0 && target != NULL) chipAtk(dmg, chip->line, Origin, target, linevirus);
        }
    }
}

void freeChip(Chips* chip){
    free(chip);
}

void virusAtk(Virus* virus, Chips* target, Chips** Origin){
    if (target->life - 1 <= 0){
        Chips* current = *(Origin);
        if (*(Origin) == target){
            *(Origin) = (*(Origin))->next;
            freeChip(target);
            return;
        }
        while(current->next != NULL){
            if (current->next == target){
                current->next = target->next;
                freeChip(target);
                return;
            }
        }
        fprintf(stderr, "virusAtk: chip mort introuvable \n");
        exit(EXIT_FAILURE);
    }
    else {
        target->life = target->life - 1;
    }
}

void buffVirus(Virus* virus, Virus* linevirus[7], Chips** OriginChips){
    if (virus == NULL) return;
    if (linevirus[virus->line - 1] == NULL) return;
    int presence_debuff = 0;
    Chips* current = *(OriginChips);
    while (current != NULL){
        if (current->line == virus->line && current->type){
            if (current->type == R){
                presence_debuff = 1;
                break;
            }
        }
        current = current ->next;
    }
    if (!presence_debuff){
        int type = (linevirus[virus->line - 1])->type;
        switch(type){
            case E:
                (linevirus[virus->line - 1])->speed = 3;
                break;
            case D:
                (linevirus[virus->line - 1])->speed = 3;
                break;
            case S:
                (linevirus[virus->line - 1])->speed = 5;
                break;
            case M:
                (linevirus[virus->line - 1])->speed = 3;
                break;
            case B:
                (linevirus[virus->line - 1])->speed = 3;
                break;
        }
    }
}

void healVirus(Virus* virus, Virus* linevirus[7]){
    if ((linevirus[virus->line - 1]) == NULL) return;
    if ((linevirus[virus->line - 1])->life == 0) return;
    else{
        (linevirus[virus->line - 1])->life = (linevirus[virus->line - 1])->life + 1;
    }
}

void actionVirus(Virus* virus, Chips** OriginChips, Virus* linevirus[7]){
    Chips* current = *(OriginChips);
    if (current == NULL) return;
    while (current != NULL){
        if (current->line == virus->line){
            if (virus->position - current->position == 1) {
                virusAtk(virus, current, OriginChips);
                break;
            }
            if (current->position == virus->position){
                fprintf(stderr, "actionVirus: superposition de chip et virus \n");
                exit(EXIT_FAILURE);
            }
        }
        current = current->next;
    }
    if (virus->type == M) healVirus(virus, linevirus);
    if (virus->type == B) buffVirus(virus, linevirus, OriginChips);
}

void moveVirus(Virus* virus, Chips** OriginChips){
    if (virus == NULL){
        fprintf(stderr, "moveVirus : fonction sur NULL");
        exit(EXIT_FAILURE);
    }
    if (virus->speed > 5 || virus->speed <= 0){
        fprintf(stderr, "moveVirus : Vitesse du virus incohérent");
        exit(EXIT_FAILURE);
    }
    if (virus->position > max_pos || virus->position < 0){
        fprintf(stderr, "moveVirus : Position du virus incohérent");
        exit(EXIT_FAILURE);
    }
    if (virus->position == 0) return;
    Chips* current = *(OriginChips);
    int chip_in_front = 0;
    int gap_to_nearest;
    while (current != NULL){ // verification de la position des chips devant le virus, intervalle entre chip et virus
        if (current->line == virus->line) {
            if (current->position <= virus->position){
                if (current->position == virus->position){
                    fprintf(stderr, "moveVirus : Superposition entre chip et virus");
                    exit(EXIT_FAILURE);
                }
                if (!chip_in_front){
                    chip_in_front = 1;
                    gap_to_nearest = virus->position - current->position;
                    if ((gap_to_nearest > max_pos) || (gap_to_nearest < 0)){
                        fprintf(stderr, "S1 : gtn %d, vp %d, curr %d", gap_to_nearest, virus->position, current->position);
                        fprintf(stderr, "moveVirus : distance entre virus et chip incohérent");
                        exit(EXIT_FAILURE);
                    }
                }
                else{
                    if ((current->position - virus->position) < gap_to_nearest) {
                        gap_to_nearest = virus->position - current->position;
                        if (gap_to_nearest > max_pos || gap_to_nearest < 0){
                            fprintf(stderr, "S2 : gtn %d, vp %d, curr %d", gap_to_nearest, virus->position, current->position);
                            fprintf(stderr, "moveVirus : distance entre virus et chip incohérent");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
            }
        }
        current = current->next;
    }
    if (virus->prev_line == NULL){
        if (chip_in_front && (virus->speed >= gap_to_nearest)){
            virus->position = virus->position - (gap_to_nearest - 1);
        }
        else virus->position = virus->position - virus->speed;
    }
    if (virus->prev_line != NULL){
        int gap_virus_in_front = virus->position - (virus->prev_line)->position;
        if (gap_virus_in_front > max_pos || gap_virus_in_front < 0){
            fprintf(stderr, "vp = %d, vprevp = %d \n", virus->position, (virus->prev_line)->position);
            fprintf(stderr, "moveVirus : distance entre virus et virus incohérent");
            exit(EXIT_FAILURE);
        }
        if (chip_in_front && (gap_virus_in_front >= gap_to_nearest)){
            if (gap_to_nearest >= virus->speed){
                if (gap_to_nearest == virus->speed){
                    virus->position = virus->position - (virus->speed - 1);
                }
                else virus->position = virus->position - virus->speed;
            }
            else{
                virus->position = virus->position - (gap_to_nearest - 1);
            }
        }
        else{
            if (gap_virus_in_front >= virus->speed){
                if (gap_virus_in_front == virus->speed){
                    virus->position = virus->position - (virus->speed - 1);
                }
                else virus->position = virus->position - virus->speed;
            }
            else{
                virus->position = virus->position - (gap_virus_in_front - 1);
            }
        }
    }
    if (virus->position < 0) virus->position = 0;
    return;
}

void initLinevirus(Virus* linevirus[7]){
    for (int i = 0; i < 7; i++) linevirus[i] = NULL;
}
int calculCash(int* argent, int price){
    *argent = *argent - price;
    if (*argent >= 0){
        return 1;
    }
    return 0;
}

void visualiseVague(char* fichier, char tab[][max_pos + 1], int x1_line, int x2_line, int y_line1, int y_line2, int y_line3, int y_line4, int y_line5, int y_line6, int y_line7){
    int width=30, height=30, x_virus, y_virus, i, j, diff;
    int width_pc=50, height_pc=50, x_pc, y_pc;
    MLV_free_window(); //libération de la fenêtre précedente.
    MLV_create_window("Chips vs Virus", "Chips vs Virus", TAILLE_X, TAILLE_Y);
    MLV_Image *image1, *image2, *image3, *image4, *image5, *image6;
    image1 = MLV_load_image("virus1.png"), image2 = MLV_load_image("virus2.png"), image3 = MLV_load_image("virus3.png"), image4 = MLV_load_image("virus4.png");
    image5 = MLV_load_image("virus5.png"), image6 = MLV_load_image("pc.png");
    MLV_resize_image_with_proportions(image1, width, height);
    MLV_resize_image_with_proportions(image2, width, height);
    MLV_resize_image_with_proportions(image3, width, height);
    MLV_resize_image_with_proportions(image4, width, height);
    MLV_resize_image_with_proportions(image5, width, height);
    MLV_resize_image_with_proportions(image6, width_pc, height_pc);
    FILE* fPointer;
    fPointer = fopen(fichier, "r");
    char singleLine[7], type;
    int prev=0, prev2=0;
    fgets(singleLine, 7, fPointer); //La première ligne ne sert à rien
    while (!feof(fPointer)){
        fgets(singleLine, 7, fPointer);
        if (atoi(&singleLine[0]) == prev && atoi(&singleLine[2]) == prev2) break;
        printf("%d %d %c\n", atoi(&singleLine[0]), atoi(&singleLine[2]), singleLine[4]);
        prev = atoi(&singleLine[0]);
        prev2 = atoi(&singleLine[2]);
        tab[atoi(&singleLine[2]) - 1][atoi(&singleLine[0]) - 1] = singleLine[4];
    }
    MLV_draw_filled_rectangle(0, 0, TAILLE_X, TAILLE_Y, MLV_COLOR_DARK_SLATE_GREY);
    x_pc = 5, y_pc = (TAILLE_Y / 2) + 20; //coordonnée de l'ordinateur centrale
    MLV_draw_line(x1_line, y_line1, x2_line, y_line1, MLV_COLOR_RED); //premiere ligne
    MLV_draw_line(x1_line, y_line2, x2_line, y_line2, MLV_COLOR_RED); //deuxième ligne
    MLV_draw_line(x1_line, y_line3, x2_line, y_line3, MLV_COLOR_RED); //troisième ligne
    MLV_draw_line(x1_line, y_line4, x2_line, y_line4, MLV_COLOR_RED); //quatrième ligne
    MLV_draw_line(x1_line, y_line5, x2_line, y_line5, MLV_COLOR_RED); //cinquième ligne
    MLV_draw_line(x1_line, y_line6, x2_line, y_line6, MLV_COLOR_RED); //sixième ligne
    MLV_draw_line(x1_line, y_line7, x2_line, y_line7, MLV_COLOR_RED); //septième ligne
    MLV_draw_line(x_pc+25, y_pc+25, x1_line, y_line1, MLV_COLOR_RED);
    MLV_draw_line(x_pc+25, y_pc+25, x1_line, y_line2, MLV_COLOR_RED);
    MLV_draw_line(x_pc+25, y_pc+25, x1_line, y_line3, MLV_COLOR_RED);
    MLV_draw_line(x_pc+25, y_pc+25, x1_line, y_line4, MLV_COLOR_RED);
    MLV_draw_line(x_pc+25, y_pc+25, x1_line, y_line5, MLV_COLOR_RED);
    MLV_draw_line(x_pc+25, y_pc+25, x1_line, y_line6, MLV_COLOR_RED);
    MLV_draw_line(x_pc+25, y_pc+25, x1_line, y_line7, MLV_COLOR_RED);
    MLV_draw_image(image6, x_pc, y_pc);
    affichePlateau(1, tab);
    diff = (x2_line - x1_line) / max_pos;
    for (i=0; i<7; i++){
        if (i == 0)
            y_virus = y_line1;
        if (i == 1)
            y_virus = y_line2;
        if (i == 2)
            y_virus = y_line3;
        if (i == 3)
            y_virus = y_line4;
        if (i == 4)
            y_virus = y_line5;
        if (i == 5)
            y_virus = y_line6;
        if (i == 6)
            y_virus = y_line7;
        for (j=0; j <= max_pos; j++){
            if (tab[i][j] != ' '){
                x_virus = x1_line + (diff * j);
                type = tab[i][j];
                if (type == 'E')
                    MLV_draw_image(image1, x_virus, y_virus - 15);
                if (type == 'D')
                    MLV_draw_image(image2, x_virus, y_virus - 15);
                if (type == 'S')
                    MLV_draw_image(image3, x_virus, y_virus - 15);
                if (type == 'M')
                    MLV_draw_image(image4, x_virus, y_virus - 15);
                if (type == 'B')
                    MLV_draw_image(image5, x_virus, y_virus - 15);
            }
        }
    }
    MLV_actualise_window();
    MLV_wait_seconds(10); //On attend 10 seconds. Pour donner le temps au joueur de regarder la vague
    MLV_free_window(); //On ferme la fenêtre de visualisation 
}

void afficheCash(int argent, int* x_money, int* y_money){
    int x, y;
    char money[10];
    sprintf(money, "%d", argent);
    x = *x_money + 70, y = *y_money;
    MLV_draw_filled_rectangle(x - 10, y - 20, 60, 40, MLV_COLOR_BLACK);
    MLV_draw_text(x, y, money, MLV_COLOR_WHITE);
    MLV_actualise_window();
}

void choixniveau(int* niveau){
    int x1, x2, y1, y2, x_souris, y_souris;
    int a1, a2, b1, b2, c1, c2, d1, d2;
    bool jouer=false, choix=false;
    //création de la première fenêtre d'accueil du jeu
    MLV_create_window("Chips vs Virus", "Chips vs Virus", TAILLE_X-300, TAILLE_Y-100);
    MLV_draw_text(250, 150, "Bienvenue sur le jeu Chips vs Virus", MLV_COLOR_WHITE);
    MLV_draw_text(240, 200, "Veuillez appuyer sur le bouton 'PLAY'", MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(280, 250, 130, 70, MLV_COLOR_GREEN);
    x1 = 280, y1 = 250, x2 = x1 + 130, y2 = y1 + 70;
    MLV_draw_text(325, 280, "PLAY", MLV_COLOR_BLACK);
    MLV_actualise_window(); // Met à jour l'affichage
    while( jouer == false){
        MLV_wait_mouse(&x_souris, &y_souris);
        if ((x1 <= x_souris && x2 >= x_souris) && (y1 <= y_souris && y2 >= y_souris)){
            jouer = true;
            MLV_free_window(); // fermeture de la fenêtre d'acceuil   
        }
    }
    //création de la fenre de choix de niveau
    MLV_create_window("Chips vs Virus", "Chips vs Virus", TAILLE_X-300, TAILLE_Y-100);
    MLV_draw_text(250, 150, "Veuillez choisir votre niveau de difficulté", MLV_COLOR_WHITE);
    MLV_draw_text(100, 200, "Appuyer sur le bouton 1 pour 'level1', bouton 2 pour 'level2' et bouton 3 pour 'level3'", MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(180, 250, 130, 70, MLV_COLOR_GREEN);
    x1 = 180, y1 = 250, x2 = x1 + 130, y2 = y1 + 70;
    MLV_draw_text(225, 280, "level1", MLV_COLOR_BLACK);
    MLV_draw_filled_rectangle(330, 250, 130, 70, MLV_COLOR_GREEN);
    a1 = 330, b1 = 250, a2 = a1 + 130, b2 = b1 + 70;
    MLV_draw_text(375, 280, "level2", MLV_COLOR_BLACK);
    MLV_draw_filled_rectangle(480, 250, 130, 70, MLV_COLOR_GREEN);
    c1 = 480, d1 = 250, c2 = c1 + 130, d2 = d1 + 70;
    MLV_draw_text(525, 280, "level3", MLV_COLOR_BLACK);
    MLV_actualise_window(); // Met à jour l'affichage
    while( choix == false){
        MLV_wait_mouse(&x_souris, &y_souris);
        if ((x1 <= x_souris && x2 >= x_souris) && (y1 <= y_souris && y2 >= y_souris)){
            choix = true;
            *niveau = 1;
            MLV_free_window(); // fermeture de la fenêtre de choix
        }
        if ((a1 <= x_souris && a2 >= x_souris) && (b1 <= y_souris && b2 >= y_souris)){
            choix = true;
            *niveau = 2;
            MLV_free_window(); // fermeture de la fenêtre de choix
        }
        if ((c1 <= x_souris && c2 >= x_souris) && (d1 <= y_souris && d2 >= y_souris)){
            choix = true;
            *niveau = 3;
            MLV_free_window(); //fermeture de la fenêtre de choix
        }
    } 
}

void afficheJeu(int* x1, int* x2, int* x3, int* x4, int* x5, int* x_money, int* x_bouton1, int* x_bouton2, int* x_bouton3, int* y1, int* y2, int* y3, int* y4, int* y5, int* y_money, int* y_bouton1, int* y_bouton2, int* y_bouton3){
    int width=30, height=30;
    *x1 = 2;
    MLV_create_window("Chips vs virus", "Chips vs Virus", TAILLE_X, TAILLE_Y); //Création de la fenêtre de jeu
    MLV_Image *image1, *image2, *image3, *image4, *image5;
    image1 = MLV_load_image("chips1.png"), image2 = MLV_load_image("chips2.png"), image3 = MLV_load_image("chips3.png"), image4 = MLV_load_image("chips4.png");
    image5 = MLV_load_image("chips5.png");
    MLV_draw_filled_rectangle(0, 0, TAILLE_X, TAILLE_Y, MLV_COLOR_DARK_SLATE_GREY);
    MLV_resize_image_with_proportions(image1, width, height);
    MLV_resize_image_with_proportions(image2, width, height);
    MLV_resize_image_with_proportions(image3, width, height);
    MLV_resize_image_with_proportions(image4, width, height);
    MLV_resize_image_with_proportions(image5, width, height);
    MLV_draw_filled_rectangle(2, 2, 400, 70, MLV_COLOR_BLACK);

    //création de la barre de choix des chips
    *x1 = *x1 + 5, *y1 = 2 + 25, *x2 = *x1 + 35, *y2 = 2 + 25, *x3 = *x2 + 35, *y3 = 2 + 25, *x4 = *x3 + 35, *y4 = 2 + 25, *x5 = *x4 + 35, *y5 = 2 + 25;
    MLV_draw_image(image1, *x1, *y1);
    MLV_draw_text(*x1, *y1+30, "100", MLV_COLOR_WHITE);
    MLV_draw_image(image2, *x2, *y2);
    MLV_draw_text(*x2, *y2+30, "150", MLV_COLOR_WHITE);
    MLV_draw_image(image3, *x3, *y3);
    MLV_draw_text(*x3, *y3+30, "300", MLV_COLOR_WHITE);
    MLV_draw_image(image4, *x4, *y4);
    MLV_draw_text(*x4, *y4+30, "50", MLV_COLOR_WHITE);
    MLV_draw_image(image5, *x5, *y5);
    MLV_draw_text(*x5, *y5+30, "125", MLV_COLOR_WHITE);
    *x_money = 207, *y_money = 37;
    MLV_draw_text(*x_money, *y_money, "Money : ", MLV_COLOR_WHITE);

    //création du bouton de visualisation de la vague
    *x_bouton1 = 450, *y_bouton1 = 2;
    MLV_draw_filled_rectangle(*x_bouton1, *y_bouton1, 100, 70, MLV_COLOR_BLACK);
    MLV_draw_text(*x_bouton1+3, *y_bouton1+35, "voir la vague !", MLV_COLOR_WHITE);

    //création du bouton de début de partie
    *x_bouton2 = 600, *y_bouton2 = 2;
    MLV_draw_filled_rectangle(*x_bouton2, *y_bouton2, 100, 70, MLV_COLOR_BLACK);
    MLV_draw_text(*x_bouton2+3, *y_bouton2+35, "Commencer !", MLV_COLOR_WHITE);

    //création du bouton quitter
    *x_bouton3 = 895, *y_bouton3 = 2;
    MLV_draw_filled_rectangle(*x_bouton3, *y_bouton3, 100, 70, MLV_COLOR_BLACK);
    MLV_draw_text(*x_bouton3+35, *y_bouton3+35, "Quit !", MLV_COLOR_WHITE);

    MLV_actualise_window(); //Met à jour l'affichage
}

void dessinePlateau(int x1_line, int x2_line, int y_line1, int y_line2, int y_line3, int y_line4, int y_line5, int y_line6, int y_line7){
    int width_pc=50, height_pc=50, x_pc, y_pc, x, y, i, j, diff;
    MLV_Image *image;
    image = MLV_load_image("pc.png");
    MLV_resize_image_with_proportions(image, width_pc, height_pc);

    //création des lignes d'attaque
    MLV_draw_line(x1_line, y_line1, x2_line, y_line1, MLV_COLOR_RED); //Premiere ligne
    MLV_draw_line(x1_line, y_line2, x2_line, y_line2, MLV_COLOR_RED); //deuxième ligne
    MLV_draw_line(x1_line, y_line3, x2_line, y_line3, MLV_COLOR_RED); //troisième ligne
    MLV_draw_line(x1_line, y_line4, x2_line, y_line4, MLV_COLOR_RED); //quatrième ligne
    MLV_draw_line(x1_line, y_line5, x2_line, y_line5, MLV_COLOR_RED); //Cinquième ligne
    MLV_draw_line(x1_line, y_line6, x2_line, y_line6, MLV_COLOR_RED); //Sixième ligne
    MLV_draw_line(x1_line, y_line7, x2_line, y_line7, MLV_COLOR_RED); //Septième ligne
    x_pc = 5, y_pc = (TAILLE_Y / 2) + 20; //coordonnée de l'ordinateur centrale
    MLV_draw_line(x_pc+15, y_pc+15, x1_line, y_line1, MLV_COLOR_RED);
    MLV_draw_line(x_pc+15, y_pc+15, x1_line, y_line2, MLV_COLOR_RED);
    MLV_draw_line(x_pc+15, y_pc+15, x1_line, y_line3, MLV_COLOR_RED);
    MLV_draw_line(x_pc+15, y_pc+15, x1_line, y_line4, MLV_COLOR_RED);
    MLV_draw_line(x_pc+15, y_pc+15, x1_line, y_line5, MLV_COLOR_RED);
    MLV_draw_line(x_pc+15, y_pc+15, x1_line, y_line6, MLV_COLOR_RED);
    MLV_draw_line(x_pc+15, y_pc+15, x1_line, y_line7, MLV_COLOR_RED);
    //Création des positions
    diff = (x2_line - x1_line) / max_pos;
    for (i=0; i<7; i++){
        if (i == 0)
            y = y_line1;
        if (i == 1)
            y = y_line2;
        if (i == 2)
            y = y_line3;
        if (i == 3)
            y = y_line4;
        if (i == 4)
            y = y_line5;
        if (i == 5)
            y = y_line6;
        if (i == 6)
            y = y_line7;
        for (j=0; j <= max_pos; j++){
            x = x1_line + (diff * j);
            MLV_draw_filled_circle(x + 3, y - 2, 3, MLV_COLOR_GREEN);
        }
    }
    //placement de l'odinateur centrale
    MLV_draw_image(image, x_pc, y_pc);
    MLV_actualise_window(); //Met à jour l'affichage
}

int clicPoseChips(int* x_souris, int* y_souris, int x1_line, int x2_line, int y_line1, int y_line2, int y_line3, int y_line4, int y_line5, int y_line6, int y_line7){
    int x, y, i, j, diff;
    diff = (x2_line - x1_line) / max_pos;
    MLV_wait_mouse(x_souris, y_souris);
    MLV_wait_mouse(x_souris, y_souris);
    for (i=0; i<7; i++){
        if (i == 0)
            y = y_line1 - 2;
        if (i == 1)
            y = y_line2 - 2;
        if (i == 2)
            y = y_line3 - 2;
        if (i == 3)
            y = y_line4 - 2;
        if (i == 4)
            y = y_line5 - 2;
        if (i == 5)
            y = y_line6 - 2;
        if (i == 6)
            y = y_line7 - 2;
        for (j=0; j <= max_pos; j++){
            x = x1_line + (diff * j);
            if (((x-30) <= *x_souris && (x+30) >= *x_souris) && ((y-30) <= *y_souris && (y+30) >= *y_souris)){
                return 1;
            }
        }
    }
    return 0;
}

int PoseChips(Game* game, char typeChips, int price, int* x_souris, int* y_souris, int x1_line, int x2_line, int y_line1, int y_line2, int y_line3, int y_line4, int y_line5, int y_line6, int y_line7){
    int x, y, diff, i, j;
    diff = (x2_line - x1_line) / max_pos;
    for (i=0; i<7; i++){
        if (i == 0)
            y = y_line1 - 2;
        if (i == 1)
            y = y_line2 - 2;
        if (i == 2)
            y = y_line3 - 2;
        if (i == 3)
            y = y_line4 - 2;
        if (i == 4)
            y = y_line5 - 2;
        if (i == 5)
            y = y_line6 - 2;
        if (i == 6)
            y = y_line7 - 2;
        for (j=0; j <= max_pos; j++){
            x = x1_line + (diff * j);
            if (((x-30) <= *x_souris && (x+30) >= *x_souris) && ((y-30) <= *y_souris && (y+30) >= *y_souris)){
                printf("%d %d\n", i+1, j+1);
                printf("%c\n", typeChips);
                insereChips(&(game->chips), typeChips, i+1, j+1, price);
                return 1;
            }            
        }
    }
    return 0;
}

int selectChips(char* typeChips, int* price, int* x1, int* x2, int* x3, int* x4, int* x5, int* y1, int* y2, int* y3, int* y4, int* y5, int* x_souris, int* y_souris){
    MLV_wait_mouse(x_souris, y_souris);
    if (((*x1-30) <= *x_souris && (*x1+30) >= *x_souris) && ((*y1-30) <= *y_souris && (*y1+30) >= *y_souris)){
        *typeChips = 'A';
        *price = 100;
        return 1;
    }
    if (((*x2-30) <= *x_souris && (*x2+30) >= *x_souris) && ((*y2-30) <= *y_souris && (*y2+30) >= *y_souris)){
        *typeChips = 'R';
        *price = 150;
        return 1;
    }
    if (((*x3-30) <= *x_souris && (*x3+30) >= *x_souris) && ((*y3-30) <= *y_souris && (*y3+30) >= *y_souris)){
        *typeChips = 'P';
        *price = 300;
        return 1;
    }
    if (((*x4-30) <= *x_souris && (*x4+30) >= *x_souris) && ((*y4-30) <= *y_souris && (*y4+30) >= *y_souris)){
        *typeChips = 'X';
        *price = 50;
        return 1;
    }
    if (((*x5-30) <= *x_souris && (*x5+30) >= *x_souris) && ((*y5-30) <= *y_souris && (*y5+30) >= *y_souris)){
        *typeChips = 'F';
        *price = 125;
        return 1;
    }
    return 0;
}

int clicOnCommencer(int* x_bouton2, int* x_souris, int* y_souris, int* y_bouton2){
    int x1, x2, y1, y2;
    x1 = *x_bouton2, x2 = x1 + 100, y1 = *y_bouton2, y2 = y1 + 70;
    MLV_wait_mouse(x_souris, y_souris);
    if ((x1 <= *x_souris && x2 >= *x_souris) && (y1 <= *y_souris && y2 >= *y_souris)){
        return 1;
    }
    return 0;
}

int clicOnVisualiser(int* x_bouton1, int* x_souris, int* y_souris, int* y_bouton1){
    int x1, x2, y1, y2;
    x1 = *x_bouton1, x2 = x1 + 100, y1 = *y_bouton1, y2 = y1 + 70;
    MLV_wait_mouse(x_souris, y_souris);
    if ((x1 <= *x_souris && x2 >= *x_souris) && (y1 <= *y_souris && y2 >= *y_souris)){
        return 1;
    }
    return 0;
}

int clicOnQuit(int* x_bouton3, int* x_souris, int* y_souris, int* y_bouton3){
    int x1, x2, y1, y2;
    x1 = *x_bouton3, x2 = x1 + 100, y1 = *y_bouton3, y2 = y1 + 70;
    MLV_wait_mouse(x_souris, y_souris);
    if ((x1 <= *x_souris && x2 >= *x_souris) && (y1 <= *y_souris && y2 >= *y_souris)){
        return 1;
    }
    return 0;
}

void affichePion(char tab[][max_pos+1], int x1_line, int x2_line, int y_line1, int y_line2, int y_line3, int y_line4, int y_line5, int y_line6, int y_line7){
    int x, y, diff, i, j, width=30, height=30;
    MLV_Image *image1, *image2, *image3, *image4, *image5;
    MLV_Image *image6, *image7, *image8, *image9, *image10;
    image1 = MLV_load_image("chips1.png"), image2 = MLV_load_image("chips2.png"), image3 = MLV_load_image("chips3.png"), image4 = MLV_load_image("chips4.png"), image5 = MLV_load_image("chips5.png");
    image6 = MLV_load_image("virus1.png"), image7 = MLV_load_image("virus2.png"), image8 = MLV_load_image("virus3.png"), image9 = MLV_load_image("virus4.png"), image10 = MLV_load_image("virus5.png");
    MLV_resize_image_with_proportions(image1, width, height);
    MLV_resize_image_with_proportions(image2, width, height);
    MLV_resize_image_with_proportions(image3, width, height);
    MLV_resize_image_with_proportions(image4, width, height);
    MLV_resize_image_with_proportions(image5, width, height);
    MLV_resize_image_with_proportions(image6, width, height);
    MLV_resize_image_with_proportions(image7, width, height);
    MLV_resize_image_with_proportions(image8, width, height);
    MLV_resize_image_with_proportions(image9, width, height);
    MLV_resize_image_with_proportions(image10, width, height);
    
    diff = (x2_line - x1_line) / max_pos;
    for (i=0; i<7; i++){
        if (i == 0)
            y = y_line1 - 2;
        if (i == 1)
            y = y_line2 - 2;
        if (i == 2)
            y = y_line3 - 2;
        if (i == 3)
            y = y_line4 - 2;
        if (i == 4)
            y = y_line5 - 2;
        if (i == 5)
            y = y_line6 - 2;
        if (i == 6)
            y = y_line7 - 2;
        for (j=0; j <= max_pos; j++){
            x = x1_line + (diff * j);
            if (tab[i][j] == 'A')
                MLV_draw_image(image1, x, y);
            if (tab[i][j] == 'R')
                MLV_draw_image(image2, x, y);
            if (tab[i][j] == 'P')
                MLV_draw_image(image3, x, y);
            if (tab[i][j] == 'X')
                MLV_draw_image(image4, x, y);
            if (tab[i][j] == 'F')
                MLV_draw_image(image5, x, y);
            if (tab[i][j] == 'E')
                MLV_draw_image(image6, x, y);
            if (tab[i][j] == 'D')
                MLV_draw_image(image7, x, y);
            if (tab[i][j] == 'S')
                MLV_draw_image(image8, x, y);
            if (tab[i][j] == 'M')
                MLV_draw_image(image9, x, y);
            if (tab[i][j] == 'B')
                MLV_draw_image(image10, x, y);
        }
    }
    MLV_actualise_window(); //met à jour l'affichage
}

//ascii
int ascii(){
    //INITIALISATION DES VARIABLES NECESSAIRES
    char* fichier = NULL;
    char tab[7][max_pos + 1], typeChips;
    bool fin_partie=false, test=false;
    int niveau, argent, lineChips, posChips, nb_chips, i, price, vaguemax, tour;
    Game* game;
    Virus* linevirus[7];
    Virus* current_virus;
    Chips* current_chip;
    int status_partie;
    //CHOIX DU NIVEAU DE DIFFICULTE
    while (test == false){
        printf("Veuillez tapez votre niveau de difficulté (niveau facile = 1, niveau intermédiaire = 2, niveau difficile = 3) : ");
        scanf(" %d", &niveau);
        if (niveau == 1){
            fichier = "level1.txt";
            test = true;
            break;
        }
        if (niveau == 2){
            fichier = "level2.txt";
            test = true;
            break;
        }
        if (niveau == 3){
            fichier = "level3.txt";
            test = true;
            break;
        }
    }
    //INITIALISATION D'UNE PARTIE
    argent = cash(fichier);
    game = allouePartie(argent);
    nombreVague(fichier, &vaguemax);
    game->turn = 1;
    initLinevirus(linevirus);
    //POSE DES CHIPS
    printf("Combien voulez vous inserer de chips ? : ");
    scanf(" %d", &nb_chips);
    for (i=0; i<nb_chips; i++){
            here:
            printf("Veuillez choisir le type de chips : ");
            scanf(" %s", &typeChips);
        switch(typeChips){
            case 'A':
                price = priceChips1;
                break;
            case 'R':
                price = priceChips2;
                break;
            case 'P':
                price = priceChips3;
                break;
            case 'X':
                price = priceChips4;
                break;
            case 'F':
                price = priceChips5;
                break;
            default:
                fprintf(stderr, "type invalide (%c)\n", typeChips);
                goto here;
                break;
        }
        printf("Veuillez entrer les coordonnées de l'endroit ou veuillez insérer un chips (sous forme [ligne][position]) :");
        scanf(" %d %d", &lineChips, &posChips);
        insereChips(&(game->chips), typeChips, lineChips, posChips, price);
    }
    while(fin_partie == false){
        tour = game->turn;
        if (!initTableau(tab)){
            fprintf(stderr, "Tableau non initialisé\n");
            fin_partie = true;
            break;
        }
        insereVague(fichier, &(game->virus), tour, linevirus);
        //résolution des action chips selon leur chainage next
        current_chip = (game->chips);
        while(current_chip != NULL){
            actionChip(current_chip, &(game->virus), linevirus);
            current_chip = current_chip->next;
        }
        //résolution des action virus arrivant au contact des chips selon leur chainage next
        current_virus = (game->virus);
        while(current_virus != NULL){
            actionVirus(current_virus, &(game->chips), linevirus);
            current_virus = current_virus->next;
        }
        //deplacement des virus
        current_virus = (game->virus);
        while(current_virus != NULL){
            moveVirus(current_virus, &(game->chips));
            current_virus = current_virus->next;
        }
        if (!chipsTableau(tab, game->chips)){
            fprintf(stderr, "Problème rencontré lors du remplissage du tableau\n");
            fin_partie = true;
            break;
        }
        virusTableau(tab, &(game->virus));
        affichePlateau(tour, tab);
        status_partie = conditionFinPartie(game->virus, tour, vaguemax);
        if (status_partie == 1){
            printf("you loose ! \n");
            fin_partie = true;
            break;
        }
        if (status_partie == 2) {
            printf("you win ! \n");
            fin_partie = true;
            break;
        }
        MLV_wait_milliseconds(1000);
        (game->turn) = ++tour;
        
    }
    free(game->chips);
    free(game->virus);
    free(game);
    return 1;
}

int graphique(){
    //INITIALISATION DES VARIABLES NECESSAIRES
    char* fichier = NULL;
    char tab[7][max_pos + 1], typeChips;
    bool fin_partie=false, lancer_jeu=false, test=false;
    int niveau, argent, nb_chips, i, price, vaguemax, tour;
    int x1, x2, x3, x4, x5, x_money, x_bouton1, x_bouton2, x_bouton3, y1, y2, y3, y4, y5, y_money, y_bouton1, y_bouton2, y_bouton3, x_souris, y_souris;
    int x1_line=100, x2_line=1000, y_line1=200, y_line2=255, y_line3=310, y_line4=365, y_line5=420, y_line6=475, y_line7=530;
    Game* game;
    Virus* linevirus[7];
    Virus* current_virus;
    Chips* current_chip;
    int status_partie;
    //CHOIX DU NIVEAU DE DIFFICULTE
    choixniveau(&niveau);
    if (niveau == 1){
        fichier = "level1.txt";
    }
    if (niveau == 2){
        fichier = "level2.txt";
    }
    if (niveau == 3){
        fichier = "level3.txt";
    }
    //INITIALISATION D'UNE PARTIE
    argent = cash(fichier);
    game = allouePartie(argent);
    nombreVague(fichier, &vaguemax);
    game->turn = 1;
    initLinevirus(linevirus);

    afficheJeu(&x1, &x2, &x3, &x4, &x5, &x_money, &x_bouton1, &x_bouton2, &x_bouton3, &y1, &y2, &y3, &y4, &y5, &y_money, &y_bouton1, &y_bouton2, &y_bouton3);
    afficheCash(argent, &x_money, &y_money);
    dessinePlateau(x1_line, x2_line, y_line1, y_line2, y_line3, y_line4, y_line5, y_line6, y_line7);
    while(lancer_jeu == false){
        if (clicOnCommencer(&x_bouton2, &x_souris, &y_souris, &y_bouton2)){
            lancer_jeu = true;
            break;
        }
        if (clicOnQuit(&x_bouton3, &x_souris, &y_souris, &y_bouton3)){
            lancer_jeu = true;
            fin_partie = true;
            break;
        }
        if (clicOnVisualiser(&x_bouton1, &x_souris, &y_souris, &y_bouton1)){
            lancer_jeu = true;
            visualiseVague(fichier, tab, x1_line, x2_line, y_line1, y_line2, y_line3, y_line4, y_line5, y_line6, y_line7);
            afficheJeu(&x1, &x2, &x3, &x4, &x5, &x_money, &x_bouton1, &x_bouton2, &x_bouton3, &y1, &y2, &y3, &y4, &y5, &y_money, &y_bouton1, &y_bouton2, &y_bouton3);
            afficheCash(argent, &x_money, &y_money);
            dessinePlateau(x1_line, x2_line, y_line1, y_line2, y_line3, y_line4, y_line5, y_line6, y_line7);
        }
    }
    //POSE DES CHIPS
    printf("Combien voulez vous inserer de chips ? : ");
    scanf(" %d", &nb_chips);
    for(i=0; i<nb_chips; i++){
        afficheCash(argent, &x_money, &y_money);
        while(test == false){
            here:
            if (selectChips(&typeChips, &price, &x1, &x2, &x3, &x4, &x5, &y1, &y2, &y3, &y4, &y5, &x_souris, &y_souris)){
                switch(typeChips){
                    case 'A':
                        price = priceChips1;
                        break;
                    case 'R':
                        price = priceChips2;
                        break;
                    case 'P':
                        price = priceChips3;
                        break;
                    case 'X':
                        price = priceChips4;
                        break;
                    case 'F':
                        price = priceChips5;
                        break;
                    default:
                        fprintf(stderr, "type invalide (%c)\n", typeChips);
                        goto here;
                        break;
                }
                if (calculCash(&argent, price)){
                    if (clicPoseChips(&x_souris, &y_souris, x1_line, x2_line, y_line1, y_line2, y_line3, y_line4, y_line5, y_line6, y_line7)){
                        if (PoseChips(game, typeChips, price, &x_souris, &y_souris, x1_line, x2_line, y_line1, y_line2, y_line3, y_line4, y_line5, y_line6, y_line7)){
                            test = true;
                            break;
                        }
                    }
                }else{
                    test = true;
                    break;
                }
            }
        }
        test=false;
    }
    if (!initTableau(tab)){
        fprintf(stderr, "Tableau non initialisé\n");
        fin_partie = true;
    }
    if (!chipsTableau(tab, game->chips)){
        fprintf(stderr, "Problème rencontré lors du remplissage du tableau\n");
        fin_partie = true;
    }
    affichePion(tab, x1_line, x2_line, y_line1, y_line2, y_line3, y_line4, y_line5, y_line6, y_line7);
    while(fin_partie == false){
        tour = game->turn;
        if (!initTableau(tab)){
            fprintf(stderr, "Tableau non initialisé\n");
            fin_partie = true;
            break;
        }
        insereVague(fichier, &(game->virus), tour, linevirus);
        //résolution des action chips selon leur chainage next
        current_chip = (game->chips);
        while(current_chip != NULL){
            actionChip(current_chip, &(game->virus), linevirus);
            current_chip = current_chip->next;
        }
        //résolution des action virus arrivant au contact des chips selon leur chainage next
        current_virus = (game->virus);
        while(current_virus != NULL){
            actionVirus(current_virus, &(game->chips), linevirus);
            current_virus = current_virus->next;
        }
        //deplacement des virus
        current_virus = (game->virus);
        while(current_virus != NULL){
            moveVirus(current_virus, &(game->chips));
            current_virus = current_virus->next;
        }
        if (!chipsTableau(tab, game->chips)){
            fprintf(stderr, "Problème rencontré lors du remplissage du tableau\n");
            fin_partie = true;
            break;
        }
        virusTableau(tab, &(game->virus));
        affichePlateau(tour, tab);
        status_partie = conditionFinPartie(game->virus, tour, vaguemax);
        MLV_wait_milliseconds(1000); 
        if (status_partie == 1){
            printf("you loose ! \n");
            fin_partie = true;
            break;
        }
        if (status_partie == 2) {
            printf("you win ! \n");
            fin_partie = true;
            break;
        }
        if (tour > 1){
            MLV_draw_filled_rectangle(0, 100, 1000, 500, MLV_COLOR_DARK_SLATE_GREY);
            MLV_actualise_window(); //Met à jour l'affichage
            dessinePlateau(x1_line, x2_line, y_line1, y_line2, y_line3, y_line4, y_line5, y_line6, y_line7);
        }
        affichePion(tab, x1_line, x2_line, y_line1, y_line2, y_line3, y_line4, y_line5, y_line6, y_line7);
        (game->turn) = ++tour;
    }
    MLV_free_window(); //fermeture de la fenêtre de jeu
    free(game->chips);
    free(game->virus);
    free(game);
    return 1;
}
int main(int argc, char * argv[]) {
    int a, g;
    if( argc == 2 ) {
        for (int i = 1; i <= 1; i++) {
            if (argv[i][0] == '-') {
                if (argv[i][1] == 'a'){
                    a = 1;
                    continue;
                }
                if (argv[i][1] == 'g'){
                    g = 1;
                    continue;
                }
                fprintf(stderr, "Arguments érronés \n");
                return 1;
            }
            else {
                fprintf(stderr, "Arguments érronés \n");
                return 1;
            }
        }
        if (a == 1) {
            ascii();
            return 1;
        }
        if (g == 1){
            graphique();
            return 1;
        }
        else {
            fprintf(stderr, "Arguments incompatibles \n");
            return 1;
        }
    }
    if( argc > 4 ) {
        fprintf(stderr, "Trop d'arguments obtenus \n");
        return 1;
    }
    else {
        fprintf(stderr, "Un argument est attendus \n");
        return 1;
    }
    return 0;
}