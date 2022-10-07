#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void affTab_char(char *tab, int taille);
void affTab_int(int *tab, int taille);
void affTab_float(float *tab, int taille);


// Remplit le tableau des entrées et renvoie la valeur Ykd
int remplirEntree(char *entree, int taille, char *nomFichier){
    FILE* fichier = NULL;
    int caractereActuel = 0;
    int i = 0;
    int Ykd; // Valeur en fin de fichier (1 ou 0) pour indiquer au programme le fichier (cercle/triangle) qui a été choisi

    fichier = fopen(nomFichier, "r"); // r pour indiquer que le fichier est juste lu
    if (fichier != NULL){
        do
        {
            caractereActuel = fgetc(fichier); // Lire un caractère dans le fichier
            entree[i] = caractereActuel;
            i++;
            if(i == taille +1){
                Ykd = caractereActuel; // taille correspond juste aux caractères * et . on récupère donc Ykd avec taille+1
            }
        } while (caractereActuel != EOF); // = End Of File
        fclose(fichier);
        Ykd = Ykd - 48;
    }
    return Ykd;
}


// Valeurs aléatoires des poids
void randomPoids(float *poids, int taille){
    for(int i = 0; i<taille; i++) {
            poids[i] = rand() %20; // %nbr définit le maximum des valeurs choisies par le random
    }
}


// Convertit les . en 0 et les * en 1
void convertissage(char *entree, int *neurone, int taille){
    for(int i = 0; i<taille; i++) {
        if (entree[i] == '.'){
            neurone[i] = 0;
        }
        else if (entree[i] == '*'){
            neurone[i] = 1;
        }
    }
}   


// Calcul du potentiel du neurone de sortie
float potentielCalcul(float *poids, int *neurone, int taille, float theta){
    float potentiel = 0;
    for(int i = 0; i<taille; i++) {
        potentiel += poids[i] * neurone[i];
    }
    return potentiel - theta;
}


// Calcul de la réponse du neurone de sortie X_i
int sortie(float potentiel){
    if (potentiel > 0){
        return 1;
    }
    else{
        return 0;
    }
}


// Calcul de l'erreur entre Ykd (valeur du début) et X_i (réponse du neurone de sortie)
float erreurCalcul(int X_i, int Ykd){
    return Ykd - X_i;
}


// Calcul des nouveaux poids par apprentissage
void poidsApprentissage(float *poids, float epsilon, float erreur, int *neurone, int taille){
    for(int i = 0; i<taille; i++) {
        poids[i] = poids[i] + epsilon * erreur * neurone[i];
    }
}

void affTab_char(char *tab, int taille) {
    for(int i=0; i<taille; i++) {
        printf("%c ", tab[i]);
    }
    printf("\n");
    puts("");
}

void affTab_int(int *tab, int taille) {
    for(int i=0; i<taille; i++) {
        printf("%d ", tab[i]);
    }
    printf("\n");
    puts("");
}

void affTab_float(float *tab, int taille) {
    for(int i=0; i<taille; i++) {
        printf("%.3f ", tab[i]);
    }
    printf("\n");
    puts("");
}



int main(int argc, char** argv) {
    int occurence;
    printf("Entrez le nombre de tests effectues : ");
    scanf("%d", &occurence);

    int taille = 11*7; // Nombre de pixels
    float *poids; // Tableau des poids
    poids = (float*) malloc(taille*sizeof(float));
    randomPoids(poids, taille);
    
    char *entree; // Tableau des caractères du fichier
    int *neurone; // Tableau des caractères du fichier convertit en 0 et 1
    entree = (char*) malloc(taille*sizeof(char));
    neurone = (int*) malloc(taille*sizeof(int));
    convertissage(entree, neurone, taille);

    float theta = 0.5; // Valeur seuil 
    float epsilon = 0.01; // Vitesse d'apprentissage du neurone

    int Ykd; // Valeur en fin de fichier (1 ou 0) pour indiquer au programme le fichier qui a été choisi
    float potentiel;
    float X_i; // Réponse du neurone de sortie
    float erreur; // Erreur entre Ykd et X_i
    int nbrProbleme = 0;

    srand(time(NULL)); // initialise la fonction srand sur le temps actuel
    for( int accumulateur = 0; occurence > accumulateur; accumulateur++ ){
        if(rand() % 2){
            Ykd = remplirEntree(entree, taille, "cercle.txt");
            convertissage(entree, neurone, taille);
        }
        else{
            Ykd = remplirEntree(entree, taille, "triangle.txt");
            convertissage(entree, neurone, taille);
        }
        potentiel = potentielCalcul(poids, neurone, taille, theta); // Calcul du potentiel du neurone de sortie
        X_i = sortie(potentiel); // Calcul de la réponse du neurone de sortie X_i
        erreur = erreurCalcul(X_i, Ykd); // Calcul de l'erreur entre Ykd (valeur du début) et X_i (réponse du neurone de sortie)
        poidsApprentissage(poids, epsilon, erreur, neurone, taille); // Calcul des nouveaux poids par apprentissage

        if(X_i != Ykd){
            nbrProbleme ++;
        }
    }
    printf("\nPourcentage d'erreurs lors de l'apprentisage : %.4f \n\n", (float) nbrProbleme/occurence*100);

    int saisie = 0;

    do{
    printf("Saisir ce que vous donnez au perceptron : \n 0 = cercle, 1 = triangle, -1 = quitter le programme \n ");

    scanf("%d",&saisie);
    if(saisie != -1){

    if (saisie){
        Ykd = remplirEntree(entree, taille, "triangle.txt");
        convertissage(entree, neurone, taille);
    }
    else{
        Ykd = remplirEntree(entree, taille, "cercle.txt");
        convertissage(entree, neurone, taille);
    }

        potentiel = potentielCalcul(poids, neurone, taille, theta); // Calcul du potentiel du neurone de sortie
        X_i = sortie(potentiel); // Calcul de la réponse du neurone de sortie X_i

    if (X_i){
        printf("Le perceptron dis que c'est un triangle \n\n");
    }
    else{
        printf("Le perceptron dis que c'est un cercle \n\n");
    }
    }

    }while(saisie != -1);


    return 0;
}
