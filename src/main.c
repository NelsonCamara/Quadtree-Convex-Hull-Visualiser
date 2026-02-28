#include <stdio.h> 
#include <MLV/MLV_all.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "polygone.h"
#define TAILLE_X 512
#define TAILLE_Y 512
#define RAND_MA 10000

CellulePoint* allouerCellule(Point pval){
    /*Cette fonction prends en parametre un Point p et l'insère dans une cellule pour l'alouer sur la pile*/
    CellulePoint *cellule = (CellulePoint*)malloc(sizeof(CellulePoint));/*on alloue la cellule sur la pile*/
    if (cellule != NULL){
        cellule->p = pval;/*on insère le Point dans la cellule*/
    }
    return cellule;
}

int inserernext(ConvexHull *poly, ListePoint adressecell,Point val){
    /*Cette fonction prends en argument un ConvexHull, une adresse ListePoint et un Point val et insère la cellule
    contenant le point après la cellule dont l'adresse a été prise en argument*/
    ListePoint cell = allouerCellule(val);
    ListePoint parcours = poly->pol;
    if (cell == NULL){
        return 0;
    }
    else if (parcours == NULL){/*On vérifie que poly.pol n'est pas NULL : qu'il y ait une cellule dans la liste 
    sinn on insère la cellule a la place*/
       poly->pol = cell;
       poly->curlen ++;
       return 1;
    }
    else if (poly->curlen == 1){/*On vérifie qu'il n'y ai pas qu'une cellule dans dans la liste sinn on insere 
    la cellule après la première*/
        parcours->suivant = cell;
        cell->precedent = parcours;
        cell->suivant = parcours;
        parcours->precedent = cell;
        poly->curlen ++;
        return 1;
    }
    for (int i = 0; i < poly->curlen; i++){/*si il y a au moins deux cellule dans la liste on insère la cellule
     après la cellule demander en argument*/
        if (parcours->precedent == adressecell){
            parcours->precedent->suivant = cell;
            cell->precedent = parcours->precedent;
            cell->suivant = parcours;
            parcours->precedent = cell;
            poly->curlen ++;
            return 1;
        }
        parcours = parcours->suivant;
    }
    return 0;
}

void dessinePolygone(ListePoint polygone, int taille){
    /*Cette fonction prends en argument une adresse de cellule ListePoint et la taille de la liste Puis affiche
    dans la fenetre le polygone correspondant a la suite de points*/
  int vx[taille], vy[taille];
  for (int i = 0; i < taille; i++){
    vx[i] = polygone->p.x;
    vy[i] = polygone->p.y;
    polygone = polygone->suivant;
  }
  MLV_draw_polygon(vx, vy, taille, MLV_COLOR_WHITE);
  MLV_actualise_window();
}

int effacenext(ConvexHull *poly, ListePoint adressecell){
    /*Cette fonction prend en argument un ConvexHull et une adresse ListePoint et supprime et libère la Cellule qui suit*/
    ListePoint parcours = poly->pol;
    for (int i = 0; i < poly->curlen; i++){
        if (parcours->precedent == adressecell){
            /*On detecte la cellule en parcourant le polygone puis on effectue lmes changementrs nécéssaire pour chaques cellules*/
            parcours->precedent->suivant = parcours->suivant;
            parcours->suivant->precedent = parcours->precedent;
            poly->curlen --;
            free(parcours);
            return 1;
        }
        parcours = parcours->suivant;
        
    }
    return 0;
}

void ajouter_decimal(Point *p){
    /*Cette fonction prend en argument un Point p et donnes aux coordonnées de ce points des décimales aléatoires (1/10000)*/
    double PERTURB = 1.0/RAND_MA;
    p->x = p->x+(rand()%RAND_MA)*PERTURB;
    p->y = p->y+(rand()%RAND_MA)*PERTURB;
}

ConvexHull point_alea_cercle(int nbpoints, Point centrecarre, double rayon){
  /* Cette Fonction permet de créer une enveloppe convexe a partir de points genérés dans un carre
  aléatoirement et qui tend vers la forme de ce carre, Elle possède trois arguments; nbpoints : le 
  nombres de points a générer aléatoirement afin de créer l'enveloppe; centrecarre : le Point avec 
  les coordonnées de centre du carre; rayon un int deffinissant le demi-coté du carre. La fonction
  renvoie un ConvexHull de l'enveloppe*/
  ConvexHull poly;/*Polygone où est stocké les Points de l'enveloppe*/
  poly.pol = NULL;
  poly.curlen = 0;
  poly.maxlen = 0;
  int r = rayon;/*Conversion en int pour la generation des points aléatoire*/
  Point p;
  for(int i = 0; i < nbpoints; i++){
    while( ( (p.x-centrecarre.x)*(p.x-centrecarre.x)  + (p.y-centrecarre.y)*(p.y-centrecarre.y) ) > rayon*rayon ){
        /*Formule de restriction des coodronnées pour rester dans le cercle*/
        p.x = (centrecarre.x-r)+(rand()%(2*r+1));/*on genere un nouveau point aléatoire*/
        p.y = (centrecarre.y-r)+(rand()%(2*r+1));
        ajouter_decimal(&p);
    }
    inserernext(&poly,poly.pol,p);
    p.x = centrecarre.x-r*2;
  }
  return poly;
}

ConvexHull point_alea_carre(int nbpoints, Point centrecarre, double rayon){
  /* Cette Fonction permet de créer une enveloppe convexe a partir de points genérés dans un carre
  aléatoirement et qui tend vers la forme de ce carre, Elle possède trois arguments; nbpoints : le 
  nombres de points a générer aléatoirement afin de créer l'enveloppe; centrecarre : le Point avec 
  les coordonnées de centre du carre; rayon un int deffinissant le demi-coté du carre. La fonction
  renvoie un ConvexHull de l'enveloppe*/
  ConvexHull poly;/*Polygone où est stocké les Points de l'enveloppe*/
  poly.pol = NULL;
  poly.curlen = 0;
  poly.maxlen = 0;
  int r = rayon;/*Conversion en int pour la generation des points aléatoire*/
  Point p;
  for(int i = 0; i < nbpoints; i++){
    p.x = (centrecarre.x-r)+(rand()%(2*r+1));/*on genere un nouveau point aléatoire*/
    p.y = (centrecarre.y-r)+(rand()%(2*r+1));
    ajouter_decimal(&p); 
    inserernext(&poly,poly.pol,p);
  }
  return poly;
}

ConvexHull enveloppePolygone(ConvexHull polygone){
    /*Cette fonction est l'algorithme qui permet de construire une enveloppe convexe a partir d'un polygone donné 
    en argument*/
    ConvexHull convexe;
    convexe.pol = NULL;
    convexe.curlen = 0;
    convexe.maxlen = 0;
    ListePoint parcours = polygone.pol;
    for (int i = 0; i < 2; i++){/*ajoute les deux premiers points a l'enveloppe convexe s'ils existent*/
        Point point = parcours->p;
        inserernext(&convexe,convexe.pol,point);
        parcours = parcours->suivant;
    }
    ListePoint parcoursconvexe = convexe.pol;
    for (int i = 0; i < polygone.curlen - 2; i++){
        /*Parcours la liste des points du polygone donné en argument*/
        for (int j = 0; j < convexe.curlen ; j++){
            parcoursconvexe = parcoursconvexe->suivant;
            /*Parcours la liste convexe jusqu'a ajouter un point a l'enveloppe convexe si celui ci correspond a la formule*/
            if ( ( (parcoursconvexe->p.x - parcours->p.x) * (parcoursconvexe->suivant->p.y - parcours->p.y) - (parcoursconvexe->suivant->p.x - parcours->p.x) * (parcoursconvexe->p.y - parcours->p.y) ) < 0){
                inserernext(&convexe, parcoursconvexe, parcours->p);
                parcoursconvexe = parcoursconvexe->suivant;
                while(( (parcoursconvexe->p.x - parcoursconvexe->suivant->p.x) * (parcoursconvexe->suivant->suivant->p.y - parcoursconvexe->suivant->p.y) - (parcoursconvexe->suivant->suivant->p.x - parcoursconvexe->suivant->p.x) * (parcoursconvexe->p.y - parcoursconvexe->suivant->p.y) ) > 0 && convexe.curlen > 3){
                /*parcours la liste dans le sens horraire a partir du point ajouté et retire les points non convexes*/
                    effacenext(&convexe, parcoursconvexe);
                }
                while( ( (parcoursconvexe->p.x - parcoursconvexe->precedent->p.x) * (parcoursconvexe->precedent->precedent->p.y - parcoursconvexe->precedent->p.y) - (parcoursconvexe->precedent->precedent->p.x - parcoursconvexe->precedent->p.x) * (parcoursconvexe->p.y - parcoursconvexe->precedent->p.y) ) < 0 && convexe.curlen > 3){
                /*parcours la liste dans le sens anti-horraire a partir du point ajouté et retire les points non convexes*/
                    effacenext(&convexe, parcoursconvexe->precedent->precedent);
                }
                /*stop le parcours de la liste convexe si le point a été ajouter*/
                break;
            }
            }                       
        parcours = parcours->suivant;
        }
    convexe.pol = parcoursconvexe;/*évite de redonner la même adresse comme première cellule si jamais celle ci a été
    éffacée*/
    return convexe;
}

ConvexHull point_alea_cercle_affiche(int nbpoints, Point centrecarre, double rayon){
  /* Cette Fonction permet de créer une enveloppe convexe a partir de points genérés dans un cercle
  aléatoirement et qui tend vers la forme de ce cercle et d'afficher l'enveloppe convexe a chaque 
  étapes de la génération, Elle possède trois arguments; nbpoints : le nombres de points a générer
  aléatoirement afin de créer l'enveloppe; centrecarre : le Point avec les coordonnées de centre 
  du cercle; rayon un int deffinissant le rayon du cercle. La fonction renvoie un ConvexHull de l'enveloppe*/
  ConvexHull poly;/*Polygone où est stocké les Points de l'enveloppe*/
  poly.pol = NULL;
  poly.curlen = 0;
  poly.maxlen = 0;
  int r = rayon;/*Conversion en int pour la generation des points aléatoire*/
  Point p;
  for(int i = 0; i < nbpoints; i++){
    while( ( (p.x-centrecarre.x)*(p.x-centrecarre.x) + (p.y-centrecarre.y)*(p.y-centrecarre.y) ) > rayon*rayon ){
        /*Formule de restriction des coodronnées pour rester dans le cercle*/
        p.x = (centrecarre.x-r)+(rand()%(2*r+1));/*on genere un nouveau point aléatoire*/
        p.y = (centrecarre.y-r)+(rand()%(2*r+1));
        ajouter_decimal(&p);
    }
    inserernext(&poly,poly.pol,p);
    p.x = centrecarre.x-r*2; /*redeffinition de p.x pour que le prochain point repasse dans la boucle while et soit genéré*/
    if(poly.curlen>3){
            ConvexHull enveloppe = enveloppePolygone(poly);
            poly.pol = enveloppe.pol;
            poly.curlen = enveloppe.curlen;
        }
    MLV_clear_window(MLV_COLOR_BLACK);
    dessinePolygone(poly.pol, poly.curlen);/*on affiche l'enveloppe a chaques points genérés*/
    MLV_wait_keyboard_or_mouse(NULL,NULL,NULL,NULL,NULL);
  }
  return poly;
}

ConvexHull point_alea_carre_affiche(int nbpoints, Point centrecarre, double rayon){
  /* Cette Fonction permet de créer une enveloppe convexe a partir de points genérés dans un carre
  aléatoirement et qui tend vers la forme de ce carre et d'afficher l'enveloppe convexe a chaque 
  étapes de la génération, Elle possède trois arguments; nbpoints : le nombres de points a générer
  aléatoirement afin de créer l'enveloppe; centrecarre : le Point avec les coordonnées de centre 
  du carre; rayon un int deffinissant le demi-coté du carre. La fonction renvoie un ConvexHull de l'enveloppe*/
  ConvexHull poly;/*Polygone où est stocké les Points de l'enveloppe*/
  poly.pol = NULL;
  poly.curlen = 0;
  poly.maxlen = 0;
  int r = rayon;/*Conversion en int pour la generation des points aléatoire*/
  Point p;
  for(int i = 0; i < nbpoints; i++){
    p.x = (centrecarre.x-r)+(rand()%(2*r+1));/*on genere un nouveau point aléatoire*/
    p.y = (centrecarre.y-r)+(rand()%(2*r+1));
    ajouter_decimal(&p); 
    inserernext(&poly,poly.pol,p);
    MLV_clear_window(MLV_COLOR_BLACK);
    if(poly.curlen>3){
            ConvexHull enveloppe = enveloppePolygone(poly);
            poly.pol = enveloppe.pol;
            poly.curlen = enveloppe.curlen;
        }
    MLV_clear_window(MLV_COLOR_BLACK);
    dessinePolygone(poly.pol, poly.curlen);/*on affiche l'enveloppe a chaques points genérés*/
    MLV_wait_keyboard_or_mouse(NULL,NULL,NULL,NULL,NULL);
  }
  return poly;
}

ConvexHull entrerPolygone(){
    ConvexHull poly;
    poly.pol = NULL;
    poly.curlen = 0;
    poly.maxlen = 0;
    int etat = 0;
    int nb = 0;
    int event = 0;
    int x = 0;
    int y = 0;
    while (1){
        Point p;
        event = MLV_wait_keyboard_or_mouse(NULL,NULL,NULL,&x,&y);
        p.x = x;
        p.y = y;
        if (event == MLV_KEY){
            break;
        }
        ajouter_decimal(&p);
        etat = etat + inserernext(&poly, poly.pol, p);
        if(poly.curlen>3){
            ConvexHull enveloppe = enveloppePolygone(poly);
            MLV_clear_window(MLV_COLOR_BLACK);
            dessinePolygone(enveloppe.pol, enveloppe.curlen);
            poly.pol = enveloppe.pol;
            poly.curlen = enveloppe.curlen;
        }
        else{
            MLV_clear_window(MLV_COLOR_BLACK);
            dessinePolygone(poly.pol, poly.curlen);
        }
        nb++;
    }
    return poly;
}

void afficheListe(ConvexHull poly){
    ListePoint parcoursconvexe = poly.pol;
    for (int i = 0; i < poly.curlen; i++){
        printf("p%d : (%f,%f) , suivant : (%f,%f)\n",i,parcoursconvexe->p.x,parcoursconvexe->p.y,parcoursconvexe->suivant->p.x,parcoursconvexe->suivant->p.y);
        parcoursconvexe = parcoursconvexe->suivant;
    }
}

void libererListe(ListePoint *liste){
    while(*liste != NULL){
        ListePoint temp = *liste;
        free(liste);
        liste = &temp;
        free(temp);
    }
    free(liste);
    liste = NULL;
}

int main( int argc, char *argv[] ){
    srand(time(NULL));/*initilise une graine aléatoire en fonction du moment ou est lancé le programme*/
    ConvexHull enveloppe;/*créer un autre polygone pour pouvoir dessiner les autres polygones 
    crées si ils sont transformés en enveloppe convexe*/
    MLV_create_window( "exo1", "test", TAILLE_X, TAILLE_Y);/*créer la fenetre MLV*/
    Point centre;/*créer un point servant de centre dans les fonctions de génération aléatoire*/
    centre.x = 200;
    centre.y = 200;
    ConvexHull p1 = entrerPolygone();/*crée un polygone avec tous les Points générés alétoirements :
                                      changer le enterPolygone() par example par point_alea_carre() pour choisir une autre option de génération */
    /*les fonctions point_alea_carre() et point_alea_cercle() nécéssitent de les transformer en enveloppe (ou pas si vous voulez)
    et de les affichers manuelement comme expliquer si dessous*/
    afficheListe(p1);/*affiche dans le terminales tous les points de p1*/
    enveloppe = enveloppePolygone(p1);/*construit une enveloppe convexe de p1*/
    /*
    CECI EST UN EXAMPLE D'AFFICHAGE DENVELOPPE DANS LA FENETRE
    MLV_clear_window(MLV_COLOR_BLACK);Affiche enveloppe (ici les points de p1 formants une enveloppe convexe) <-- efface ce qu'il y a sur la fenetre
    dessinePolygone(enveloppe.pol,enveloppe.curlen); <-- dessine l'enveloppe sur la fenetre
    MLV_wait_keyboard_or_mouse(NULL,NULL,NULL,NULL,NULL); <-- demande au programe d'attendre un clic ou un appuy de touche pour continuer : arretez le programme
    */
    MLV_free_window();
    return 1;
}