#include <stdio.h>

typedef struct chips{
    int type;
    int life;
    int line;
    int position;
    int price;
    struct chips* next;
}Chips;

typedef struct virus{
    int type;
    int life;
    int line;
    int position;
    int speed;
    int turn;
    struct virus* next;
    struct virus* next_line;
    struct virus* prev_line;
}Virus;

typedef struct{
    Virus* virus;
    Chips* chips;
    int turn;
    int money;
}Game;

