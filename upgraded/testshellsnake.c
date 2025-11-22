#include "shellsnake.h"

int lancerPartie(int largeur, int hauteur,float vitesse,int nbPommes){
    //initialisation plateau
    char **plateau;
    plateau=allouerPlateau(hauteur,largeur);
    
    //initialisation fenêtre(ncurses)
    WINDOW *winsnake = initscr();
    box(winsnake, 0, 0);  // Dessine un cadre autour de la fenêtre

    //initialisation aléatoire
    srand(time(NULL));

    // entrées clavier
    keypad(winsnake, true); //active les touches spéciales (flèches)
    nodelay(winsnake,true);//permet de lire l'entrée du joueur en permanance
    curs_set(0); //masque le curseur

    //initialisation taille
    int taille = 1;
    
    // char plateau[hauteur][largeur];
    initPlateau(plateau,largeur,hauteur); 

    //snake au démarrage
    Tab2 tete = {largeur/2,hauteur/2}; //coordonnées tête
    Tab2 dir={0,0}; //direction
    Tab2 corps[(largeur-2)*(hauteur-2)];//={tete,tete};
    corps[0]=tete;
    corps[1]=tete;

    //pomme
    int tlogCellulesVides;
    int cellulesVides[(largeur-2)*(hauteur-2)][2];
    for(int itPommes=1;itPommes<=nbPommes;itPommes++){
        creerTableauSansSerpent(plateau,largeur,hauteur,cellulesVides,&tlogCellulesVides);
        if(tlogCellulesVides>0){
            placerPomme(plateau,largeur,hauteur,cellulesVides,tlogCellulesVides);
        }
    }
    //initialisation
    int surPomme,collision=0;

    //gameloop
    while(true){
        int fleche = wgetch(winsnake);
        if (fleche == KEY_LEFT){
            if(dir.x == 1) continue;
            dir.x =-1;
            dir.y=0;
        }
        if (fleche == KEY_RIGHT){
            if(dir.x == -1) continue;
            dir.x =1;
            dir.y=0;
        }
        if (fleche == KEY_DOWN){
            if(dir.y == -1) continue;
            dir.x =0;
            dir.y=1;
        }
        if (fleche == KEY_UP){
            if(dir.y == 1) continue;
            dir.x =0;
            dir.y=-1;
        }
        if(fleche=='q'){
            break;
        }
        
        surPomme=FALSE;

        tete=deplacementTete(plateau,largeur,hauteur,tete,dir);
        testCollisions(plateau,largeur,hauteur,tete,&surPomme,&collision);
        
        if(surPomme==TRUE){
            taille++;
            majPlateau(plateau,largeur,hauteur,corps,tete,taille,dir);
            creerTableauSansSerpent(plateau,largeur,hauteur,cellulesVides,&tlogCellulesVides);
            if(tlogCellulesVides!=0){
                placerPomme(plateau,largeur,hauteur,cellulesVides,tlogCellulesVides);
            }
        }
        else majPlateau(plateau,largeur,hauteur,corps,tete,taille,dir);


        //affichage
        erase();
        affichPlateau(plateau,largeur,hauteur);
        wprintw(winsnake,"\tsize: %d",taille);
        refresh();//sinon le plateau n'a pas le temps de s'afficher complétement et le usleep() s'active
        //condition de partie perdue
        if(collision==1){
            usleep(800000);
            break;
        }
        if(taille==(largeur-2)*(hauteur-2)){//si le serpent à atteint la taille maximale
            usleep(1000000);
            break;
        }
        usleep(vitesse*100000);
        

    }

    endwin();
    libererPlateau(plateau,hauteur);
    return taille;
}


int customMenuSize(int tailleCustom,int vitesseCustom,int *largeur,int *hauteur,float *vitesse,int itColor){
    WINDOW *wincustom= newwin(8,18,11,22);
    keypad(wincustom, true);
    
    wattron(wincustom,COLOR_PAIR(itColor));
    wattron(wincustom,COLOR_PAIR(itColor));


    int tmpLargeur=(*largeur),tmpHauteur=(*hauteur);
    float selecteurVitesse=((0.1-(*vitesse))/0.1)+49;
    int key=-1;
    if(tailleCustom==TRUE){
        while(key!='\n' && key!=' '){
            werase(wincustom);
            if(tmpLargeur-2<10){
                mvwprintw(wincustom,1,2,"width   < %d  >",tmpLargeur-2);
            }
            else{
                mvwprintw(wincustom,1,2,"width   < %d >",tmpLargeur-2);
            }
            box(wincustom,0,0);
            key=wgetch(wincustom);
            if(key==KEY_LEFT && tmpLargeur>5){
                tmpLargeur=tmpLargeur-2;
            }
            else if(key==KEY_RIGHT && tmpLargeur<51){
                tmpLargeur=tmpLargeur+2;
            }
            else if(key=='q'){
                return TRUE;
            }
        }
        key=-1;
        while(key!='\n' && key!=' '){
            werase(wincustom);
            if(tmpLargeur-2<10){
                mvwprintw(wincustom,1,2,"width   < %d  >",tmpLargeur-2);
            }
            else{
                mvwprintw(wincustom,1,2,"width   < %d >",tmpLargeur-2);
            }
            if(tmpHauteur-2<10){
                mvwprintw(wincustom,2,2,"height  < %d  >",tmpHauteur-2);
            }
            else{
                mvwprintw(wincustom,2,2,"height  < %d >",tmpHauteur-2);

            }
            box(wincustom,0,0);
            key=wgetch(wincustom);
            if(key==KEY_LEFT && tmpHauteur>5){
                tmpHauteur=tmpHauteur-2;
            }
            else if(key==KEY_RIGHT && tmpHauteur<51){
                tmpHauteur=tmpHauteur+2;
            }
            else if(key=='q'){
                return TRUE;
            }
        }
    }
    if(vitesseCustom==TRUE){
        key=-1;
        while(key!='\n' && key!=' '){
            werase(wincustom);
            if(selecteurVitesse<10){
                mvwprintw(wincustom,3,2,"speed   < %.0f  >",selecteurVitesse);
            }
            else{
                mvwprintw(wincustom,3,2,"speed   < %.0f >",selecteurVitesse);
            }
            if(tailleCustom==TRUE){
                if(tmpLargeur-2<10){
                    mvwprintw(wincustom,1,2,"width   < %d  >",tmpLargeur-2);
                }
                else{
                    mvwprintw(wincustom,1,2,"width   < %d >",tmpLargeur-2);
                }
                if(tmpHauteur-2<10){
                    mvwprintw(wincustom,2,2,"height  < %d  >",tmpHauteur-2);
                }
                else{
                    mvwprintw(wincustom,2,2,"height  < %d >",tmpHauteur-2);
                }
            }
            box(wincustom,0,0);
            key=wgetch(wincustom);
            if(key==KEY_LEFT && selecteurVitesse>1){
                selecteurVitesse=selecteurVitesse-1;
            }
            else if(key==KEY_RIGHT && selecteurVitesse<49){
                selecteurVitesse=selecteurVitesse+1;
            }
            else if(key=='q'){
                return TRUE;
            }
        }
    }
    (*largeur)=tmpLargeur;
    (*hauteur)=tmpHauteur;
    (*vitesse)= 0.1 + (49 - selecteurVitesse) * 0.1;
        //0.1: vitesse float minimale, 49: plus grand int possible pour l'utilisateur, 0.1: pas entre chaque intervalle
        //permet de convertir un int en un intervalle entre deux frame pour determiner la vitesse du serpent
        //en fonction de selecteurVitesse compris entre 1 et 49, cela va donner un float intervalle entre 0.1 et 5 avec un pas de 0.1
    
    delwin(wincustom); 
    return FALSE;
}

void menu(void){
    initscr();
    noecho();
    curs_set(0); //masque le curseur

    if (!has_colors()) {  // permet de vérifier si le terminal supporte ncurses
        endwin();
        printf("Your terminal does not support color display or window management.\n");
        exit(1);
    }

    start_color();
    use_default_colors();
    //initialisations des couples de couleurs(police,arrière plan)
    init_pair(1, COLOR_WHITE,-1);
    init_pair(2, COLOR_RED,-1);
    init_pair(3, COLOR_GREEN,-1);
    init_pair(4, COLOR_YELLOW,-1);
    init_pair(5, COLOR_BLUE,-1);
    init_pair(6, COLOR_MAGENTA,-1);
    init_pair(7, COLOR_CYAN,-1);
    init_pair(8, COLOR_BLACK,-1);

    int hauteur=19,largeur=31;
    float vitesse=1.5;

    int pos=5,score=-1;
    WINDOW *win1= newwin(20,100,0,0);
    WINDOW *win2= newwin(8,58,11,40);


    keypad(win1, true); //active les touches spéciales (flèches)
    char speedTab[4][10]={" slow    "," medium  "," fast    "," custom  "};
    char sizeTab[4][10]={" small   "," medium  "," large   "," custom  "};
    char colorTab[9][10]={" default "," white   "," red     "," green   "," yellow  "," blue    "," magenta "," cyan    "," black   "};
    int itSpeed=1,itSize=1,itColor=0;
    int nbPommes=1;
    afficheMenu(win1,win2,colorTab,sizeTab,speedTab,pos,itSize,itSpeed,itColor,nbPommes,score);
    
    int fleche = wgetch(win1);
    
    while(true){ //selecteur
        if (fleche == KEY_LEFT){
            if(pos==4){
                if(nbPommes==1){
                    nbPommes=9;
                }
                else nbPommes--;
            }
            else if(pos==3){
                if(itSpeed==0){
                    itSpeed=3;
                }
                else itSpeed--;
            }
            else if(pos==2){
                if(itSize==0){
                    itSize=3;
                }
                else itSize--;
            }
            else if(pos==1){
                if(itColor==0){
                    itColor=8;
                }
                else itColor--;
            }
        }
        else if (fleche == KEY_RIGHT){
            if(pos==4){
                if(nbPommes==9){
                    nbPommes=1;
                }
                else nbPommes++;
            }
            else if(pos==3){
                if(itSpeed==3){
                    itSpeed=0;
                }
                else itSpeed++;
            }
            else if(pos==2){
                if(itSize==3){
                    itSize=0;
                }
                else itSize++;
            }
            else if(pos==1){
                if(itColor==8){
                    itColor=0;
                }
                else itColor++;
            }
        }
        else if (fleche == KEY_DOWN){
            if(pos==5) pos=1;
            else pos++;
        }   
        else if (fleche == KEY_UP){
            if(pos==1) pos=5;
            else pos--;
        }
        else if(fleche=='q'){
            break;
        }
        else if(fleche=='\n' || fleche==' '){
            if(pos==5){
                int quitter=FALSE;
                int vitesseCustom=FALSE,tailleCustom=FALSE;
                switch(itSpeed){
                    case 0:
                        vitesse=2.5;
                    break;
                    case 1:
                        vitesse=1.5;
                    break;
                    case 2:
                        vitesse=1;
                    break;
                    case 3:
                        vitesseCustom=TRUE;
                    break;
                }
                switch(itSize){
                    case 0:
                        hauteur=13;
                        largeur=21;
                    break;
                    case 1:
                        hauteur=19;
                        largeur=31;
                    break;
                    case 2:
                        hauteur=25;
                        largeur=41;
                    break;
                    case 3:
                        tailleCustom=TRUE;
                    break;
                }
                if(tailleCustom==TRUE||vitesseCustom==TRUE){
                    quitter=customMenuSize(tailleCustom,vitesseCustom,&largeur,&hauteur,&vitesse,itColor);
                }
                if(quitter==FALSE){
                    attron(COLOR_PAIR(itColor));
                    score=lancerPartie(largeur,hauteur,vitesse,nbPommes);
                    flushinp();//efface le buffer des touches préssés après avoir perdu la partie, pour ne pas influencer l'affichage du menu
                    attroff(COLOR_PAIR(itColor));
                    clear();
                    refresh();
                }
            }
            else if(pos!=5){
                pos++;
            }
        }

        afficheMenu(win1,win2,colorTab,sizeTab,speedTab,pos,itSize,itSpeed,itColor,nbPommes,score);
        fleche = wgetch(win1);

    }
    endwin();

}

int main(void){
    menu();
    return 0;
}