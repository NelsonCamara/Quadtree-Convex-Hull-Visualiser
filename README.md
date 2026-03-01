# Quadtree & Convex Hull

Implémentation en C d'un algorithme d'enveloppe convexe avec visualisation graphique utilisant la bibliothèque MLV.

## Description

Ce projet implémente deux structures de données fondamentales en géométrie computationnelle :

1. **Enveloppe Convexe (Convex Hull)** : Calcul incrémental de l'enveloppe convexe d'un ensemble de points
2. **Quadtree** : Structure de partitionnement spatial pour l'indexation efficace de points 2D

## Fonctionnalités

### Enveloppe Convexe (`main.c`)

- **Génération aléatoire** : Création de points dans un carré ou un cercle
- **Algorithme incrémental** : Construction pas-à-pas de l'enveloppe convexe
- **Mode interactif** : Ajout de points par clic souris avec mise à jour en temps réel
- **Visualisation** : Affichage graphique de l'enveloppe à chaque étape
- **Liste doublement chaînée circulaire** : Structure de données optimisée pour les insertions/suppressions

### Quadtree (`quadtree.c`)

- **Partitionnement récursif** : Division automatique de l'espace en quadrants
- **Gestion dynamique** : Ajout et suppression de particules
- **Seuil configurable** : Division déclenchée après N points par nœud

## Technologies

- **Langage C** (C99)
- **Bibliothèque MLV** - Graphiques 2D
- **Structures de données** : Listes chaînées, arbres quaternaires

## Structure du projet

```
src/
├── main.c       # Programme principal - Enveloppe convexe
├── quadtree.c   # Implémentation du Quadtree
└── polygone.h   # Définitions des structures de données
```

## Compilation

### Prérequis

- GCC ou Clang
- Bibliothèque MLV installée

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

### Génération aléatoire

Modifiez la fonction `main()` pour utiliser :

```c
// Points aléatoires dans un carré
ConvexHull p1 = point_alea_carre(100, centre, 150);

// Points aléatoires dans un cercle
ConvexHull p1 = point_alea_cercle(100, centre, 150);

// Avec affichage étape par étape
ConvexHull p1 = point_alea_carre_affiche(50, centre, 150);
```

## Algorithme d'enveloppe convexe

L'algorithme utilise le **produit vectoriel** pour déterminer l'orientation de trois points :

```
orientation(P1, P2, P3) = (P2.x - P3.x) * (P1.y - P3.y) - (P1.x - P3.x) * (P2.y - P3.y)
```

- **< 0** : Rotation horaire (point à l'extérieur)
- **> 0** : Rotation anti-horaire (point à l'intérieur)
- **= 0** : Points colinéaires

### Complexité

- **Insertion** : O(n) dans le pire cas
- **Construction complète** : O(n²) pour n points

## Structure de données

### Point

```c
typedef struct {
    double x, y;
} Point;
```

### Liste doublement chaînée circulaire

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
    int maxlen;      // Capacité maximale
} ConvexHull;
```

## Captures d'écran

Le programme affiche une fenêtre 512x512 avec :
- Les points en blanc
- L'enveloppe convexe en blanc (polygone)

## Applications

- **Jeux vidéo** : Détection de collisions
- **Robotique** : Planification de trajectoires
- **SIG** : Analyse spatiale
- **Computer Vision** : Détection de contours
