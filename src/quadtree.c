#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#define MAXPART 3

#define TAILLE 512

#define MIN 64

typedef struct Point Point;
struct Point{
    int x;
    int y;

};




typedef struct Particules_noeud Particules_noeud;
struct Particules_noeud{

    Point part;
    Particules_noeud *suiv;
    

};

typedef struct tabpoints tabpoints;
struct tabpoints{
    Point *particules;
    int parts_in_tab;
};

typedef struct quadtree quadtree;
struct quadtree{
    
    int x,y,hauteur,largeur,nbPART,kp;

    struct quadtree *HG;
    struct quadtree *HD;
    struct quadtree *BG;
    struct quadtree *BD;
    
    
    

    Particules_noeud *particules_noeud; 

};



Particules_noeud* newPart(Point point){
    Particules_noeud *new;
    new = (Particules_noeud*)malloc(sizeof(Particules_noeud));
    new->part = point;
    new->suiv = NULL;
    return new;
}

void init_quad(quadtree *qtree,int x,int y,int hauteur,int largeur){
    
    if(hauteur >= MIN && largeur >= MIN){
        
        printf("HAUTEUR:%d || LARGEUR:%d\n",hauteur,largeur);
        qtree->HG = (quadtree*)malloc(sizeof(quadtree));
        
        (qtree->HG)->x = x;
        (qtree->HG)->y = y;
        (qtree->HG)->hauteur = hauteur/2;
        (qtree->HG)->largeur = largeur/2;
        (qtree->HG)->nbPART =0;
        (qtree->HG)->kp =0;

        qtree->HD = (quadtree*)malloc(sizeof(quadtree));
        (qtree->HD)->x = x+(largeur/2);
        (qtree->HD)->y = y;
        (qtree->HD)->hauteur = hauteur/2;
        (qtree->HD)->largeur = largeur/2;
        (qtree->HD)->nbPART =0;
        (qtree->HD)->kp =0;

        qtree->BG = (quadtree*)malloc(sizeof(quadtree));
        (qtree->BG)->x = x;
        (qtree->BG)->y = y + (hauteur/2);
        (qtree->BG)->hauteur = hauteur/2;
        (qtree->BG)->largeur = largeur/2;
        (qtree->BG)->nbPART =0;
        (qtree->BG)->kp =0;

        qtree->BD = (quadtree*)malloc(sizeof(quadtree));
        (qtree->BD)->x = x+(largeur/2);
        (qtree->BD)->y = y + (hauteur/2);
        (qtree->BD)->hauteur = hauteur/2;
        (qtree->BD)->largeur = largeur/2;
        (qtree->BD)->nbPART =0;
        (qtree->BD)->kp =0;
        
        init_quad(&(*qtree->HG),(qtree->HG)->x,(qtree->HG)->y,(qtree->HG)->hauteur, (qtree->HG)->largeur);
        init_quad(&(*qtree->HD),(qtree->HD)->x,(qtree->HD)->y,(qtree->HD)->hauteur, (qtree->HD)->largeur);
        init_quad(&(*qtree->BG),(qtree->BG)->x,(qtree->BG)->y,(qtree->BG)->hauteur, (qtree->BG)->largeur);
        init_quad(&(*qtree->BD),(qtree->BD)->x,(qtree->BD)->y,(qtree->BD)->hauteur, (qtree->BD)->largeur);

    
    
    }
    else{
        return;
    }
}

void suppr_in_tab(tabpoints *tab,int x,int y){
    int i,found_or_not;
    found_or_not = 0;
    for(i=0;i<tab->parts_in_tab;i++){
        if(tab[i].particules->x == x && tab[i].particules->y ==y){
            found_or_not =1;

        }
        if(found_or_not == 1 && i < tab->parts_in_tab -1){
            tab[i] =tab[i+1];
        }
    }
    tab->parts_in_tab = tab->parts_in_tab -1;

}

void add_in_tab(tabpoints *tab,int x,int y){
    int i;
    (tab[tab->parts_in_tab].particules)->x =x;
    (tab[tab->parts_in_tab].particules)->y =y;
    tab->parts_in_tab++;

}

void ajout_quad(quadtree* qTree,Point point){
    Particules_noeud* chain_iter;
    
    qTree->nbPART++;
    qTree->kp++;
    if (&(qTree->particules_noeud) != NULL){
        chain_iter = qTree->particules_noeud;
        while (chain_iter->suiv !=NULL)
        {
            chain_iter = chain_iter->suiv;
        }
        chain_iter->suiv = newPart(point);
        
    }
    else{
        qTree->particules_noeud = newPart(point);
    }
    

}

void trouve_et_suppr(int x,int y,quadtree* qTree){
    Particules_noeud* chain_iter;
    if (qTree->particules_noeud != NULL){
        chain_iter = qTree->particules_noeud;
        if(chain_iter->part.x == x && chain_iter->part.y == y){
            Particules_noeud* tempsuiv;
            if(chain_iter->suiv != NULL){
                tempsuiv =chain_iter->suiv;
            }
            free(chain_iter);
            chain_iter = tempsuiv;
        }
        while (chain_iter->suiv !=NULL)
        {
            if(chain_iter->suiv->part.x == x && chain_iter->suiv->part.y == y){
                Particules_noeud* tempsuiv;
                tempsuiv = chain_iter->suiv;
                if(chain_iter->suiv->suiv != NULL){
                    chain_iter->suiv =chain_iter->suiv->suiv;
                }
                free(tempsuiv);
                tempsuiv = NULL;
                return;
            }
            chain_iter = chain_iter->suiv;
        }
         
        
        
    }
    else{
        return;
    }


}

int estdanscarre(Point point,int x_carre,int y_carre,int hauteur,int largeur){
    if (point.x >= x_carre && point.x <= x_carre+largeur && point.y >= y_carre && point.y <= y_carre+ hauteur){
        return 1;
    }
    else{
        return 0;
    }
}

void casse(Point point,quadtree* qtree,tabpoints *tab){
    int i,HG,HD,BG,BD,KP_HG,KP_HD,KP_BG,KP_BD;
    KP_HG =0;
    KP_HD =0;
    KP_BG =0;
    KP_BD =0;
    Particules_noeud *chain_iter;
    chain_iter =qtree->particules_noeud;
    for(i=0;i<qtree->kp;i++){
        HG = estdanscarre(chain_iter->part,(qtree->HG)->x,(qtree->HG)->y,(qtree->HG)->hauteur,(qtree->HG)->largeur);
        HD = estdanscarre(chain_iter->part,(qtree->HD)->x,(qtree->HD)->y,(qtree->HD)->hauteur,(qtree->HD)->largeur);
        BG = estdanscarre(chain_iter->part,(qtree-BG)->x,(qtree->BG)->y,(qtree->BG)->hauteur,(qtree->BG)->largeur);
        BD = estdanscarre(chain_iter->part,(qtree->BD)->x,(qtree->BD)->y,(qtree->BD)->hauteur,(qtree->BD)->largeur);

        if(HG == 1){
            trouve_et_suppr((chain_iter->part).x,(chain_iter->part).y,qtree);
            ajout_quad(qtree->HG,chain_iter->part);
            KP_HG++;
        }
        if(HD == 1){
            trouve_et_suppr((chain_iter->part).x,(chain_iter->part).y,qtree);
            ajout_quad(qtree->HD,chain_iter->part);
            KP_HD++;
        }
        if(BG == 1){
            trouve_et_suppr((chain_iter->part).x,(chain_iter->part).y,qtree);
            ajout_quad(qtree->BG,chain_iter->part);
            KP_BG++;
        }
        if(BD == 1){
            trouve_et_suppr((chain_iter->part).x,(chain_iter->part).y,qtree);
            ajout_quad(qtree->BD,chain_iter->part);
            KP_BD++;
        }

    }
    HG = 0;
    HD = 0;
    BG = 0;
    BD =0;
    if(KP_HG == 3 && estdanscarre(point,(qtree->HG)->x,(qtree->HG)->y,(qtree->HG)->hauteur,(qtree->HG)->largeur)){
        casse(point,qtree->HG,tab);
    }
    if(KP_HD == 3 && estdanscarre(point,(qtree->HD)->x,(qtree->HD)->y,(qtree->HD)->hauteur,(qtree->HD)->largeur)){
        casse(point,qtree->HD,tab);
    }
    if(KP_BG == 3 && estdanscarre(point,(qtree->BG)->x,(qtree->BG)->y,(qtree->BG)->hauteur,(qtree->BG)->largeur)){
        casse(point,qtree->BG,tab);
    }
    if(KP_BD == 3 && estdanscarre(point,(qtree->BD)->x,(qtree->BD)->y,(qtree->BD)->hauteur,(qtree->BD)->largeur)){
        casse(point,qtree->BD,tab);
    }

    else{
        HG = estdanscarre(point,(qtree->HG)->x,(qtree->HG)->y,(qtree->HG)->hauteur,(qtree->HG)->largeur);
        HD = estdanscarre(point,(qtree->HD)->x,(qtree->HD)->y,(qtree->HD)->hauteur,(qtree->HD)->largeur);
        BG = estdanscarre(point,(qtree-BG)->x,(qtree->BG)->y,(qtree->BG)->hauteur,(qtree->BG)->largeur);
        BD = estdanscarre(point,(qtree->BD)->x,(qtree->BD)->y,(qtree->BD)->hauteur,(qtree->BD)->largeur);

        if(HG == 1){
            
            ajout_quad(qtree->HG,point);
            return;
            
        }
        if(HD == 1){
   
            ajout_quad(qtree->HD,point);
            return;
         
        }
        if(BG == 1){
            
            ajout_quad(qtree->BG,point);
            return;
        
        }
        if(BD == 1){
            
            ajout_quad(qtree->BD,point);
            return;
         
        }

    }
}



void gere_point(Point point,quadtree* qtree,tabpoints *tab){
    if (estdanscarre(point,qtree->x,qtree->y,qtree->hauteur,qtree->largeur) == 1){
        if(qtree->kp == 3){
            casse(point,qtree,tab);
        }
        else{
            ajout_quad(qtree,point);
            add_in_tab(tab,point.x,point.y);
            return;
        }
    }
    else{
        gere_point(point,qtree->HG,tab);
        gere_point(point,qtree->HD,tab);
        gere_point(point,qtree->BG,tab);
        gere_point(point,qtree->BD,tab);
    }

}

int main(void){
    quadtree *qtree;
    tabpoints points[TAILLE];
    qtree = NULL;
    qtree =(quadtree*)malloc(sizeof(quadtree));
    qtree->hauteur = 512;
    qtree->largeur = 512;
    qtree->x =0;
    qtree->y =0;
    
    init_quad(qtree,0,0,512,512);

    

    return 0;

}