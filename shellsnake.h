#ifndef SHELLSNAKE_H
#define SHELLSNAKE_H

#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define TAILLE 21

typedef struct {
    int x;
    int y;
} Tab2;

void affichPlateau(int tlog,char plateau[][TAILLE]);
void initPlateau(int tlog,char plateau[][TAILLE]);
void placerPomme(char plateau[][TAILLE],int cellulesVides[][2],int tlogCellulesVides);
void creerTableauSansSerpent(char plateau[][TAILLE],int cellulesVides[][2],int *tlogCellulesVides);
Tab2 deplacementSerpent(char plateau[][TAILLE],Tab2 tete, Tab2 dir);


#endif //SHELLSNAKE_H