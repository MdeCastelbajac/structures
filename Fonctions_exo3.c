#include <stdlib.h>
#include <stdio.h>
#include "Entete_Fonctions.h"
#include "Liste_case.h"
#include <unistd.h>
#include <time.h>

/*
Remarque : le retour de la fonction agrandit_Zsg a été modifié :
	
	Elle renvoie le nombre de cases qui n'ont PAS été ajoutées à la Zsg.
	Cela permet de formuler simplement une condition de fin de jeu (lors du dernier tour) :
		- il existe une bordure non vide de la dernière couleur choisie.
		- Toute case parcourue lors de ce tour est :
				- déja dans la Zsg
				- ajoutée à la Zsg
				(ie. La fonction retourne 0)
*/

/* QUESTION 1*/

void init_Zsg(S_zsg *z, int dim, int nbcl){
	z->dim = dim;
	z->nbcl = nbcl;
	z->Lzsg = (ListeCase)malloc(sizeof(ListeCase));
	init_liste(&(z->Lzsg));
	z->B = (ListeCase *)malloc(nbcl * sizeof(ListeCase));
	int j;
	for(j=0; j<nbcl; j++){
		init_liste(&(z->B[j]));
	}
	z->App = (int **)malloc(dim * sizeof(int *));
	int i;
	for(i=0; i<dim; i++){
		z->App[i] = (int *)malloc(dim * sizeof(int));
	}
	return ;
}

void ajoute_Zsg(S_zsg *z, int i, int j){
	ajoute_en_tete(&(z->Lzsg), i, j);
	z->App[i][j] = -1;
	
	return;
}

void ajoute_Bordure(S_zsg *z, int i, int j, int cl){
	ajoute_en_tete((z->B)+cl, i, j);
	z->App[i][j] = cl;
	return;
}

int appartient_Zsg(S_zsg *z, int i, int j){
	return z->App[i][j] == -1;
}

int appartient_Bordure(S_zsg *z, int i, int j, int cl){
	return z->App[i][j] == cl;
}


/* QUESTION 2 */

/* Prédicat d'appartenance à la Grille (comme dans l'exercice 2)*/
int est_dans_grille(S_zsg *z, int i, int j){
	if((i < 0)||(j < 0)||(i >= z->dim)||(j >= z->dim))
		return 0; // la case n'est pas dans la grille
	return 1;
}

/* Cette fois, les quatre fonctions de recherche sont unifiées, on spécifiera les coordonnées (haut , bas...) en entrée */
int caseAdjacente(int **M, S_zsg *z, int cl, int i, int j, ListeCase *L){
	
	int cpt = 0;
	if(est_dans_grille(z, i, j) == 1){
		if(appartient_Zsg(z, i, j) == 0){		
			if(M[i][j] == cl){
				ajoute_Zsg(z, i, j);
				ajoute_en_tete(L, i, j);
			}
			else{			
				cpt++;
				if(appartient_Bordure(z, i, j, M[i][j])==0){
					ajoute_Bordure(z, i, j, M[i][j]);
				}			
			}
		}
	}
	return cpt;
}

// renvoie le nombre de cases non ajoutées & n'appartenant pas déjà à la ZSG  
int agrandit_Zsg(int **M, S_zsg *z, int cl, int k, int l){
	
	// déclaration et initialisation
	int cpt = 0;
	int i, j;
	
	ListeCase L; 
	init_liste(&L);
	ajoute_en_tete(&L, k, l);
	
	while(test_liste_vide(&L)==0){ // La liste est non vide	
		enleve_en_tete(&L, &i, &j);
		cpt += caseAdjacente(M, z, cl, i-1, j, &L);
		cpt += caseAdjacente(M, z, cl, i+1, j, &L);
		cpt += caseAdjacente(M, z, cl, i, j-1, &L);
		cpt += caseAdjacente(M, z, cl, i, j+1, &L);
	}
	
	detruit_liste(&L);
	return cpt;
}


/* QUESTION 3 */

int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff){
	
	srand(time(NULL));
	int cpt = 0;
	S_zsg *z = (S_zsg *)malloc(sizeof(S_zsg));
	init_Zsg(z, dim, nbcl);
	
	int cl = M[0][0];
	ListeCase pt; // parcours de la zsg pour la colorisation
	int k, l; // coordonnées des cases de B[cl] pour agrandit_Zsg
	
	// Initialisation du tableau App 
	for(k=0; k<dim; k++){
		for(l=0; l<dim; l++){
			z->App[k][l] = -2;
		}
	}
	
	
	// On commence par ajouter la case 0,0 + premier tour
	ajoute_Zsg(z, 0, 0);
	agrandit_Zsg(M, z, cl, 0, 0);
	int victoire = 1; // le nombre de cases qui restent en bordure de la Zsg à la fin du tour, lorsqu'il devient nul, la partie est finie
	
	while(victoire != 0){
		
		victoire = 0;
		
		/* Choisissons une couleur aléatoire différente de celle de notre Zsg */
		
		do{
			cl = rand()%nbcl;
		}
		while(cl == M[0][0]);
		
		pt = z->Lzsg;
		while(pt){
			M[pt->i][pt->j] = cl;		
			if(aff == 1)
				Grille_attribue_couleur_case(G, pt->i, pt->j, cl);
			pt = pt->suiv;
		}
		if(aff==1){
				Grille_redessine_Grille();
				sleep(1);	
		}
		// Si la couleur n'a aucun effet sur la grille, on empeche la partie de se terminer
		if(test_liste_vide(&(z->B[cl])) == 1){
			victoire = 1;
		}
		// Calcul de la nouvelle zsg (et nouvelle bordure)		
		while(test_liste_vide(&(z->B[cl]))==0){
			enleve_en_tete(&(z->B[cl]), &k, &l); // les cellules sont détruites lors du dépilement
			victoire += agrandit_Zsg(M, z, cl, k, l);
			ajoute_Zsg(z, k, l);
		}
		detruit_liste(&z->B[cl]);
		cpt ++; 
	}
	
	// Libération mémoire des différentes structures
	detruit_liste(&z->Lzsg);
	int i;
	for(i=0; i<dim; i++){
		free(z->App[i]);
	}
	free(z->App);
	for(i=0; i<nbcl; i++){
		free(z->B[cl]);
	}
	free(z->B);
	free(z);
	
	return cpt;
}



