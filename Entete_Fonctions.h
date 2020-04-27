#ifndef __ENTETE_FONCTIONS__
#define __ENTETE_FONCTIONS__

#include "API_Grille.h"
#include "Liste_case.h"

typedef struct _t{
	int nbcl; 			/* nombre de couleurs */
	int dim;				/* dimension de la grille */
	
	ListeCase Lzsg; /* Liste des cases de la Zsg */
	ListeCase *B; 	/* Tableau de case de taille nbcl des cases bordant la Zsg */
	int **App; 			/* Tableau à double entrée donnant l'appartenance des case(i, j) (default = -2, Zsg = -1, Bordure = cl)*/
} S_zsg;



/*            
	EXO 1
*/


/* Retourne dans L la liste des cases de meme couleur que la case i,j
   et met -1 dans ces cases */
void trouve_zone_rec(int **M, int nbcase,int i, int j, int *taille, ListeCase *L);

/* Algorithme tirant au sort une couleur: il utilise la fonction recursive pour determiner la Zsg */
int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff);

/*
	EXO 2
*/
/* Reprise de trouve_zone_rec sans récursion, utilise les fonctions d'empilement et de dépilement de la structure ListeCase */
void trouve_zone_imp(int **M, int dim);
int sequence_aleatoire_imp(int **M, Grille *G, int dim, int nbcl, int aff);

/*
	EXO 3
*/

void init_Zsg(S_zsg *z, int dim, int nbcl); /* initialise une S_zsg */
void ajoute_Zsg(S_zsg *z, int i, int j); /* ajoute une case dans la Lzsg */
void ajoute_Bordure(S_zsg *z, int i, int j, int cl); /* ajoute une case dans la bordure */
int appartient_Zsg(S_zsg *z, int i, int j); /* renvoie vrai si une case est dans la Lzsg */
int appartient_Bordure(S_zsg *z, int i, int j, int cl); /* renvoie vrai si une case est dans la bordure */
int est_dans_grille(S_zsg *z, int i, int j); /* Vérifie si une case (i,j) dépasse ou non les dimensions de la grille */
int agrandit_Zsg(int **M, S_zsg *z, int cl, int k, int l); /* Met à jour la zsg et sa bordure, à partir d'une couleur cl */
int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff); /* Algorithme tirant au sort une couleur, colorie la zsg, et la met à jour via agrandit_Zsg() */

#endif  /* __ENTETE_FONCTIONS__ */
