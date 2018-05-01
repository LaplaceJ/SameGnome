#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NBL 10 //Nombre de lignes de la matrice
#define NBC 15 //Nombre de colonnes de la matrice
#define reset "\033[0m"
#define reverse "\033[7m"
#define rouge "\033[;31m"
#define vert "\033[;32m"
#define bleu "\033[;34m"
#define jaune "\033[;33m"


//typedef Colonne : Structure:
/**
 *
typedef colonne: structure 

		eleLig[NLB] Vecteur de caractère de [1..NLB]
{NLB est une constante représentant le nombre de lignes. Cette variable contient les états des billes d’une colonne}

    		indLimVide Entier ;
{nombre de case "vide" = "." dans le vecteur eleLig}
 *
 */
typedef struct Colonne{
    char eleLig[NBL];
    int indLimVide ;
}Colonne;

/**
 *
typedef PJeu: structure 

		col[NBC] Vecteur de colonne de [1..NBC]
{NBC est une constante représentant le nombre de colonnes. Cette variable contient les colonnes du jeu}

    		indDecalage Entier
{nombre de case colonnes contenant encore des billes dans le vecteur col }

		points Entier
{nombre points de l’utilisateur }

  limGauche Entier
{Lorsque nous utilisons la fonction rCouleurCon. Nous gardons en mémoire l'indice de la colonne là plus au gauche affecté par la suppression d'une bille. Cette variable est utilisée dans la fonction écoulement et décalage pour un parcours plus optimal }

  limDroite Entier
{Lorsque nous utilisons la fonction rCouleurCon. Nous gardons en mémoire l'indice de la colonne là plus au droite affecté par la suppression d'une bille.Cette variable est utilisée dans la fonction écoulement pour un parcours plus optimal}

 */
typedef struct PJeu{
    Colonne col[NBC];
    int indDecalage ;
    int points;
    int limGauche;
    int limDroite;
}PJeu;


/**
 * @but Permets d'afficher la couleur correspondant au caractère envoyé
 *
 * @Donnée lig,col {indice de la bille affichée actuellement  }
 *
 * @DonnéeRésultat pjeu
 *
 *
 */
void switchCouleur(PJeu *pjeu , int lig , int col ){
    switch ((*pjeu).col[col].eleLig[lig]){
        case 'R':
            printf("%s%s   %s",rouge,reverse,reset);
            break ;
        case 'B':
            printf("%s%s   %s",bleu,reverse,reset);
            break ;
        case 'V':
            printf("%s%s   %s",vert,reverse,reset);
            break ;
        case 'X':
            printf("%s%s   %s",jaune,reverse,reset);
            (*pjeu).col[col].eleLig[lig] = '.';
            break ;
        default:
            printf("   ");
            break ;
    }
}

/**
 * @but : Afficher une matrice  donnée
 *
 * @Donnée:  pjeu PJeu
 *
 * @Variable_local:
 *  -i,j Entier {indices de parcours }
 *  -nbTrait Entier {nombre de '_' à écrire pour rentrer l'affichage plus esthétique }
 */

void affichage(PJeu *pjeu){
    int i , j ;

    // affichage de la partie haute du jeu
    for( i=NBL-1;i>=0;i--){
        //numéro des lignes
        printf("%d| ",i);
        for(j=0;j<NBC;j++){
            // affichage de la couleur ij de matJeu
            switchCouleur(pjeu ,i ,j );
        }
        printf("\n");
    }
    //affichage de la partie en dessous  du jeu

    // nombre de '_' à afficher
    int nbTrait = NBC + NBC + NBC + 3 ;
    for(i=0;i<nbTrait;i++){
        printf("_");
    }

    //affichage des indices de colonnes
    printf("\n | ");

    for(i=0;i<10;i++){
        printf("%d  ",i);
    }
    for(i=10;i<NBC;i++){
        printf("%d ",i);
    }
    printf("\n");

}

/**
 *
 * @but : avec une matrice de caractère donné, déplacer tous les caractères ".' vers le haut
 *
 * @DonnéeRésultat pjeu PJeu
 *
 * @Variable_local:
 *  -colonne, ligne,k   Entier {indice de boucles dans matJet } 
 *  -indEchange Entier
 *
 *
 *
 */
void ecoulement(PJeu *pjeu ){
    int indEchange   ;
    for (int colonne = pjeu->limGauche ; colonne <= pjeu->limDroite ; ++colonne) {
    //for (int colonne = 0; colonne < (*pjeu).indDecalage ; ++colonne) {
        indEchange = 0 ;

        //on fais décendre toutes les cases de couleur du nombre de case vide en dessous d'eux
        for ( int ligne = 0; ligne < (*pjeu).col[colonne].indLimVide ; ++ligne) {
            if ((*pjeu).col[colonne].eleLig[ligne] != '.') {
                if (indEchange != ligne){
                    (*pjeu).col[colonne].eleLig[indEchange] = (*pjeu).col[colonne].eleLig[ligne] ;

                }
                indEchange++;
            }
        }

        //les cases entre  indEchange et pjeu.col[colonne].indLimVide sont mis à vide
        //l'écart entre les deux valeurs correspond au nombre de case vide apparue lors de ce tour de jeu
        for (int k = indEchange ; k <(*pjeu).col[colonne].indLimVide ; ++k) {
            (*pjeu).col[colonne].eleLig[k] = '.' ;
        }

        //on met a jour la indLimVide dans notre colonne

        (*pjeu).col[colonne].indLimVide = indEchange ;
    }
}


/**
 * @but décaler les colonnes "vide" (remplie de '.') vers la droite du jeu
 *
 * @DonnéeRésultat: PJeu pjeu
 *
 *  @Variable_local
 *      -i,j, ii ,jjEntier { indice de boucle dans matJet } 
 *      -indEchange Entier {indice d'échange entre les colonnes
 */

void decalage(PJeu *pjeu ){
    int indEchange = (*pjeu).limGauche ;
    int i , j ;

    for ( i = (*pjeu).limGauche; i < (*pjeu).indDecalage ; ++i) {
        //si indLimVide == 0  alors toute la colonne est constituée de '.'
        //il nous faut donc décaler la ligne non vide la plus proche sur cette dernière.
        if((*pjeu).col[i].indLimVide != 0){
            if (indEchange != i){

                //decalage de la Ligne
                for ( j = 0; j < NBL ; ++j) {
                    (*pjeu).col[indEchange].eleLig[j] = (*pjeu).col[i].eleLig[j] ;
                }
                //on modifie indLimVide de la colonne d'arriver
                (*pjeu).col[indEchange].indLimVide = (*pjeu).col[i].indLimVide ;
            }
            indEchange++;

        }
    }

    //clean colonne
    for ( i = indEchange;  i < (*pjeu).indDecalage ; ++i) {
        for ( j = 0 ; j <(*pjeu).col[i].indLimVide ; ++j) {
            (*pjeu).col[i].eleLig[j] = '.' ;
        }
    }

    //nouvelle indice limite entre les colonnes jouables et vides
   (*pjeu).indDecalage = indEchange ;

}

/**
 * @but : initialiser le jeu aléatoirement
 *
 * @DonnéeRésultat PJeu pjeu
 *
 * @Variable_local i , j Entier { indice de boucle} 
*/
void remplissageAlea(PJeu *pjeu){
    char chaine[3]="RVB";

    srand(time(NULL));
    for (int i=0;i< NBC;i++){
        for( int j=0;j<NBL;j++){
            (*pjeu).col[i].eleLig[j] = chaine[rand()%3]  ;
        }
        (*pjeu).col[i].indLimVide = NBL;

    }
    (*pjeu).indDecalage = NBC ;
}



/**
 *
 * @but : avec 2 indices col  et lig qui désignent l'emplacement d'une couleur dans pjeu :
 *  -Remplace toutes les cases connexes et  de même couleurs par une case vide (='.')
 *  -donne le nombre de case modifier avec la variable nbCouleurCon
 *
 * @DonnéeRésultat
 *      -PJeu *pjeu
 *      -nbCouleuron entier {Nombre total de billes connexes}
 *
 * @Donnée col,lig entier {indices de la bille que l'on doit "vider" et chercher les billes de sa couleur autour d'elle }
 *
 *  @Variable_local couleur caractère {variable permettant de stocker la couleur de la case sélectionnée }
 */
void rCouleurCon(PJeu *pjeu ,int col,int lig, int *nbCouleurCon){
    (*nbCouleurCon )++;
    char couleur = pjeu->col[col].eleLig[lig];

    if  (pjeu->limDroite < col) {
        pjeu->limDroite = col ;
    }

    if  (pjeu->limGauche > col) {
        pjeu->limGauche = col ;
    }
    pjeu->col[col].eleLig[lig] = 'X';


    //récursion bas
    if(lig > 0 &&  pjeu->col[col].eleLig[lig-1] == couleur){
        rCouleurCon(pjeu,col,lig-1,nbCouleurCon );

    }
    //récursion haut
    if(lig < NBL &&  pjeu->col[col].eleLig[lig+1] == couleur){
        rCouleurCon(pjeu,col,lig+1,nbCouleurCon );
    }
    //récursion droite
    if(col < NBC && pjeu->col[col+1].eleLig[lig] == couleur){
        rCouleurCon(pjeu,col+1,lig,nbCouleurCon );
    }
    //récursion gauche
    if(col > 0 &&  pjeu->col[col-1].eleLig[lig] == couleur){
        rCouleurCon(pjeu,col-1,lig,nbCouleurCon );
    }
}

/**
 *  @but Jouer au jeu SameGnome
 *
 *  @Variable_local
 *      -pjeu PJeu
 *      -points Entier {points du joueur } 
 *      -colonne,ligne Entier {indice de la bille choisi par le joueur  }
 *      -nbBilleConnexes Entier {variable servant au calcul des points }
 */
void jeu() {
    PJeu pjeu ;

    //initialisation du jeu
    remplissageAlea(&pjeu);

    pjeu.points = 0 ;
    int colonne,ligne,nbBilleConnexes ;

    //tant que le jeu n'est pas fini
    while (pjeu.indDecalage != 0){
        printf("Vous avez %d points \n",pjeu.points );
        affichage(&pjeu);

        //Choix d'une colonne
        puts("choisir une colonne : ");
        scanf("%d",&colonne );

        //choix d'une ligne
        puts("choisir une ligne : ");
        scanf("%d",&ligne );

        //si la case selectionné est une case non vide
        if (pjeu.col[colonne].eleLig[ligne] != '.'){
            nbBilleConnexes = 0 ;
            pjeu.limDroite = colonne ;
            pjeu.limGauche = colonne ;
            //recherche des billes connexes de même couleur à la bille choisi par le joueur
            rCouleurCon(&pjeu,colonne,ligne,&nbBilleConnexes);
            //calcul des points

            pjeu.points  = (nbBilleConnexes-1)*(nbBilleConnexes-1) + pjeu.points  ;

            affichage(&pjeu);

            //écoulement des billes
            ecoulement(&pjeu);
            affichage(&pjeu);

            //décalage des billes
            decalage(&pjeu);



        } else{
            //message d'erreur lorsque la case selectionné est vide
            printf("la case colonne : %d  ligne : %d  est une case vide ! choisissez d'autre indices \n",colonne,ligne );
        }

    }

    printf("vous avez fini le jeu avec %d points!! Bravo\n",pjeu.points  );

}


//lancement du JEU SAME GNOME
int main(){

    jeu();
    return 0;

}



