#include <stdio.h>
#include <MLV/MLV_all.h>
#define hauteur 500
#define largeur 500

typedef struct{
  int x;
  int y;
}Point;

typedef struct cellulePoint{
  Point p;
  struct cellulePoint *suivant;
  struct cellulePoint *precedent;
}CellulePoint, *ListePoint;

typedef struct {
  ListePoint pol;
  int curlen;
  int maxlen;
} ConvexHull;