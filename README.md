# Quadtree & Convex Hull

Implementation en C d'un algorithme d'enveloppe convexe avec visualisation graphique utilisant la bibliotheque MLV.

## Description

Ce projet implemente deux structures de donnees fondamentales en geometrie computationnelle :

1. **Enveloppe Convexe (Convex Hull)** : Calcul incremental de l'enveloppe convexe d'un ensemble de points
2. **Quadtree** : Structure de partitionnement spatial pour l'indexation efficace de points 2D

## Fonctionnalites

### Enveloppe Convexe (`main.c`)

- **Generation aleatoire** : Creation de points dans un carre ou un cercle
- **Algorithme incremental** : Construction pas-a-pas de l'enveloppe convexe
- **Mode interactif** : Ajout de points par clic souris avec mise a jour en temps reel
- **Visualisation** : Affichage graphique de l'enveloppe a chaque etape
- **Liste doublement chainee circulaire** : Structure de donnees optimisee pour les insertions/suppressions

### Quadtree (`quadtree.c`)

- **Partitionnement recursif** : Division automatique de l'espace en quadrants
- **Gestion dynamique** : Ajout et suppression de particules
- **Seuil configurable** : Division declenchee apres N points par noeud

## Technologies

- **Langage C** (C99)
- **Bibliotheque MLV** - Graphiques 2D
- **Structures de donnees** : Listes chainees, arbres quaternaires

## Structure du projet

```
src/
├── main.c       # Programme principal - Enveloppe convexe
├── quadtree.c   # Implementation du Quadtree
└── polygone.h   # Definitions des structures de donnees
```

## Compilation

### Prerequis

- GCC ou Clang
- Bibliotheque MLV installee

### Compilation

```bash
# Enveloppe convexe
gcc -o convex_hull src/main.c -lMLV -lm

# Quadtree
gcc -o quadtree src/quadtree.c -lm
```

### Installation MLV (Linux)

```bash
# Debian/Ubuntu
sudo apt-get install libmlv3-dev

# Depuis les sources
# https://www-igm.univ-mlv.fr/~Music/music/music.html
```

## Utilisation

### Mode interactif (Enveloppe Convexe)

```bash
./convex_hull
```

- **Clic souris** : Ajouter un point
- **Touche clavier** : Terminer et afficher l'enveloppe finale

### Generation aleatoire

Modifiez la fonction `main()` pour utiliser :

```c
// Points aleatoires dans un carre
ConvexHull p1 = point_alea_carre(100, centre, 150);

// Points aleatoires dans un cercle
ConvexHull p1 = point_alea_cercle(100, centre, 150);

// Avec affichage etape par etape
ConvexHull p1 = point_alea_carre_affiche(50, centre, 150);
```

## Algorithme d'enveloppe convexe

L'algorithme utilise le **produit vectoriel** pour determiner l'orientation de trois points :

```
orientation(P1, P2, P3) = (P2.x - P3.x) * (P1.y - P3.y) - (P1.x - P3.x) * (P2.y - P3.y)
```

- **< 0** : Rotation horaire (point a l'exterieur)
- **> 0** : Rotation anti-horaire (point a l'interieur)
- **= 0** : Points colineaires

### Complexite

- **Insertion** : O(n) dans le pire cas
- **Construction complete** : O(n²) pour n points

## Structure de donnees

### Point

```c
typedef struct {
    double x, y;
} Point;
```

### Liste doublement chainee circulaire

```c
typedef struct cellulePoint {
    Point p;
    struct cellulePoint *suivant;
    struct cellulePoint *precedent;
} CellulePoint, *ListePoint;
```

### Enveloppe Convexe

```c
typedef struct {
    ListePoint pol;  // Liste des sommets
    int curlen;      // Nombre de sommets
    int maxlen;      // Capacite maximale
} ConvexHull;
```

## Captures d'ecran

Le programme affiche une fenetre 512x512 avec :
- Les points en blanc
- L'enveloppe convexe en blanc (polygone)

## Applications

- **Jeux video** : Detection de collisions
- **Robotique** : Planification de trajectoires
- **SIG** : Analyse spatiale
- **Computer Vision** : Detection de contours


