
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

//utilisation de macros pour simplifier, on assumera que les arguments sont comparables
#define max(a, b) ( a>b ? a : b )
#define min(a, b) ( a<b ? a : b)

//Structures
struct rationnel {
    int num;
    int den;
};
typedef struct rationnel Rationnel;

struct polynome {
    int degre;
    Rationnel *poly;
};
typedef struct polynome Polynome;

//Fonctions +
int estNombre(char *string) {
    for ( int i=0; string[i]!='\0'; i++ ) {
        if ( i == 0 && string[i] == '-' ) {
            continue;
        }
        if ( !isdigit(string[i]) ) {
            return 0;
        }
    }
    return 1;
}


//Fonctions
void lirePolynome(Polynome *polynome);
void lireRationnel(Rationnel *rationnel);
void afficherPolynome(Polynome polynome);
int sommePolynomes(Polynome p1, Polynome p2, Polynome *p);
int produitPolynomes(Polynome p1, Polynome p2, Polynome *p);
Rationnel evaluerPolynome(Polynome p, int x);
int deriverPolynome(Polynome p, Polynome *d);
int simplifierPolynome(Polynome *p);

//Fonctions +
void simplifierRationnel(Rationnel *rationnel);
int pgcd(int a, int b);
void afficherMenu();
int afficherPolynomes(Polynome p1, Polynome p2);
int sontDeuxPolynomesValides(Polynome *p1, Polynome *p2);
int estPolynomeValide(Polynome *p);
void sommeRationnels(Rationnel r1, Rationnel r2, Rationnel *r);
void produitRationnels(Rationnel r1, Rationnel r2, Rationnel *r);


//Main
int main() {

    char input[10];
    Polynome p1, p2, p; //p reçoit les résultats
    Polynome * pointer;
    int choix, x;
    int a;

    do {

        afficherMenu();
        printf("Choisir une option : ");
        scanf("%s", input);
        if ( estNombre(input) ) { //saisi valide
            choix = atoi(input); //argument to integer
        } else {
            choix = -1;
        }

        switch(choix) {
            case 0 : 
                break;
            case 1 : //lire p1
                lirePolynome(&p1);
                break;
            case 2 : //lire p2
                lirePolynome(&p2);
                break;
            case 3 : //affichage
                if ( afficherPolynomes(p1, p2) == 0 ) {
                    printf("Les polynômes p1(x) et p2(x) n'ont pas été définis");
                }
                break;
            case 4 : //somme
                if ( sommePolynomes(p1, p2, &p) ) {
                    printf("p1(x) + p2(x) =");
                    afficherPolynome(p);
                } else {
                    printf("[!] Au moins un polynôme n'a pas été défini");
                }
                break;
            case 5 : //produit
                if ( produitPolynomes(p1, p2, &p) ) {
                    printf("p1(x) * p2(x) =");
                    afficherPolynome(p);
                } else {
                    printf("[!] Au moins un polynôme n'a pas été défini");
                }
                break;
            case 6 : //évaluation en point x
                //tant que la saisie n'est pas un nombre
                do {
                    printf("Point x (int) : "); //récupérer x
                    scanf("%s", input);
                } while ( !estNombre(input));
                x = atoi(input);
                //tant que la saisie n'est pas un nombre et != 1 et != 2
                do {
                    printf("Polynome à évaluer (1/2) : ");
                    scanf("%s", input);
                } while ( !estNombre(input)  && input != "1" && input != "2" );
                a = atoi(input);
                if ( a == 1 ) {
                    if ( estPolynomeValide(&p1) ) {
                        Rationnel r = evaluerPolynome(p1, x);
                        printf("p%d(%d) = (%d/%d)", a, x, r.num, r.den);
                    } else {
                        printf("[!] Le polynôme p%d(x) n'a pas été défini", a);
                    }
                } else if ( a == 2 ) {
                    if ( estPolynomeValide(&p2) ) {
                        Rationnel r = evaluerPolynome(p2, x);
                        printf("p%d(%d) = (%d/%d)", a, x, r.num, r.den);
                    } else {
                        printf("[!] Le polynôme p%d(x) n'a pas été défini", a);
                    }
                } else {
                    printf("[!] Le polynôme p%d(x) n'a pas été défini", a);
                }
                break;
            case 7 : //dérivé
                do {
                    printf("Polynôme à dériver (1/2) : ");
                    scanf("%s", input);
                } while ( !estNombre(input) );
                a = atoi(input);
                if ( a == 1 ) {
                    if ( deriverPolynome(p1, &p) ) {
                        printf("Dérivée de p%d(x) =", a);
                        afficherPolynome(p);
                    } else {
                        printf("[!] Le polynôme p%d(x) n'a pas été défini", a);
                    }
                } else if ( a == 2 ) {
                    if ( deriverPolynome(p2, &p) ) {
                        printf("Dérivée de p%d(x) =", a);
                        afficherPolynome(p);
                    } else {
                        printf("[!] Le polynôme p%d(x) n'a pas été défini", a);
                    }
                } else {
                    printf("[!] Le polynôme p%d(x) n'a pas été défini", a);
                }
                break;
            default : 
                printf("Veuillez choisir une option valide (0-7)");
                break;
        }

    } while ( choix != 0 );

    free(p1.poly);
    free(p2.poly);
    printf("Vous avez quitté le programme!\n");

    return 0;
}

//Fonctions
void lirePolynome(Polynome *polynome) {
    char input[10];
    if ( polynome->poly != NULL ) { //libérer la mémoire
        free(polynome->poly);
    }
    int degre;
    do {
        printf("Degré du polynome : "); //récupérer le degré
        scanf("%s", input);
    } while ( !estNombre(input) || atoi(input) < 0 );
    degre = atoi(input);
    polynome->degre = degre;
    polynome->poly = (Rationnel *) malloc((degre+1)*sizeof(Rationnel));
    for ( int i=0; i<=degre; i++ ) {
        polynome->poly[i].num = 0; //initaliser rationnel
        polynome->poly[i].den = 1;
        printf("Coefficient au degré %d : \n", i);
        lireRationnel(&polynome->poly[i]);
    }
    simplifierPolynome(polynome);
}

void lireRationnel(Rationnel *rationnel) {
    char input[10];
    do {
        printf("\tNumérateur : ");
        scanf("%s", input);
    } while ( !estNombre(input) );
    rationnel->num = atoi(input);
    do {
        printf("\tDénominateur ( != 0 ): ");
        scanf("%s", input);
    } while ( !estNombre(input) || atoi(input) == 0 );
    rationnel->den = atoi(input);
}

void afficherPolynome(Polynome polynome) {
    int plus = 0; //afficher "+" if plus == 1
    for ( int i=0; i<=polynome.degre; i++ ) {
        if ( polynome.poly[i].num != 0 ) { //afficher si coefficient != 0
            if ( plus == 1 ) {
                printf(" +");
            }
            if ( polynome.poly[i].den == 1 ) { //coefficient = num
                if ( polynome.poly[i].num < 0 ) {
                    printf(" (%d)", polynome.poly[i].num);
                } else {
                    printf(" %d", polynome.poly[i].num);
                }
            } else { //coefficant = num/den
                printf(" (%d/%d)", polynome.poly[i].num, polynome.poly[i].den);
            }
            if ( i == 1 ) { //x^1 = x
                printf("x");
            } else if ( i > 1 ) { //x^i
                printf("x^%d", i);
            }
            plus = 1;
        }
    }
    printf("\n");
}

int sommePolynomes(Polynome p1, Polynome p2, Polynome *p) {
    if ( !sontDeuxPolynomesValides(&p1, &p2) ) {
        return 0;
    }
    if ( p->poly != NULL ) {
        free(p->poly);
    }
    int degreMax = max(p1.degre, p2.degre);
    int degreMin = min(p1.degre, p2.degre);
    p->degre = degreMax;
    p->poly = (Rationnel *) malloc((degreMax+1)*sizeof(Rationnel)); //malloc
    Polynome *greaterPolynome = p1.degre == degreMax ? &p1 : &p2;
    for ( int i=0; i<=degreMin; i++ ) { //somme pour les degrés communs
        sommeRationnels(p1.poly[i], p2.poly[i], &(p->poly[i]));
    }
    for ( int i=degreMin+1; i<=degreMax; i++ ) { //ajout des coefficients restants
        p->poly[i] = greaterPolynome->poly[i];
    }
    return 1;
}

int produitPolynomes(Polynome p1, Polynome p2, Polynome *p) {
    if ( !sontDeuxPolynomesValides(&p1, &p2) ) {
        return 0;
    }
    if ( p->poly != NULL ) { //libérer la mémoire
        free(p->poly);
    }
    p->degre = p1.degre + p2.degre;
    p->poly = (Rationnel *) malloc((p->degre+1)*sizeof(Rationnel));
    for ( int i=0; i<=p->degre; i++ ) { //initaliser les Rationnels de p->poly pour effectuer la somme
        p->poly[i].num = 0;
        p->poly[i].den = 1;
    }
    //produit p1*p2
    for ( int i=0; i<=p1.degre; i++ ) {
        for ( int j=0; j<=p2.degre; j++ ) {
            //multiplier les coefficients
            Rationnel r;
            produitRationnels(p1.poly[i], p2.poly[j], &r);
            //additionner + ajouter dans p->poly
            sommeRationnels(p->poly[i+j], r, &(p->poly[i+j]));
        }
    }
    return 1;
}

int simplifierPolynome(Polynome *p) {
    if ( !estPolynomeValide(p) ) {
        return 0;
    }
    for ( int i=0; i<=p->degre; i++ ) {
        simplifierRationnel(&(p->poly[i]));
    }
    return 1;
}

Rationnel evaluerPolynome(Polynome p, int x) {
    Rationnel result, tmp;
    result.num = 0;
    result.den = 1;
    for ( int i=0; i<=p.degre; i++ ) {
        tmp.num = pow(x, i) * p.poly[i].num;
        tmp.den = p.poly[i].den;
        sommeRationnels(result, tmp, &result);
    }
    return result;
}

int deriverPolynome(Polynome p, Polynome *d) {
    if ( !estPolynomeValide(&p) ) {
        return 0;
    }
    if ( d->poly != NULL ) {
        free(d->poly);
        d->poly = NULL;
    }
    d->degre = ( p.degre > 0 ? p.degre-1 : 0 );
    d->poly = (Rationnel *) malloc((d->degre+1)*sizeof(Rationnel));
    for ( int i=1; i<=p.degre; i++ ) {
        d->poly[i-1].num = p.poly[i].num * i; //num * i
        d->poly[i-1].den = p.poly[i].den;
        simplifierRationnel(&(d->poly[i-1])); //x^i -> x^(i-1)
    }
    return 1;
}

//Fonctions +

int afficherPolynomes(Polynome p1, Polynome p2) { //appelle afficherPolynome si le polynome est défini
    int r = 0;
    if ( estPolynomeValide(&p1) ) {
        printf("Polynome p1(x) =");
        afficherPolynome(p1);
        r = 1;
    }
    if ( estPolynomeValide(&p2) ) {
        printf("Polynome p2(x) = ");
        afficherPolynome(p2);
         r = 1;
    }
    return r;

}

void simplifierRationnel(Rationnel *rationnel) {
    if ( rationnel->num == 0 ) {
        return;
    }
    int v = pgcd(rationnel->num, rationnel->den);
    rationnel->num = (int) rationnel->num/v;
    rationnel->den = (int) rationnel->den/v;
}

int pgcd(int a, int b) {
    if ( b == 0 ) {
        return a;
    } else {
        int r = a%b;
        return pgcd(b, r);
    }
}

void afficherMenu(){ 
    printf("\n\n    ------------ Polynômes ------------\n");
    printf("      1 - Lecture du polynome p1\n");
    printf("      2 - Lecture du polynome p2\n");
    printf("      3 - Affichage (p1, p2)\n");
    printf("      4 - Somme (p1 + p2)\n");
    printf("      5 - Produit (p1 * p2)\n");
    printf("      6 - Evaluation en un point\n");
    printf("      7 - Dérivé\n");
    printf("      0 - Quitter\n");
    printf("    -----------------------------------\n\n");
}

int sontDeuxPolynomesValides(Polynome *p1, Polynome *p2) {
    return estPolynomeValide(p1) && estPolynomeValide(p2);
}

int estPolynomeValide(Polynome *p) {
    return p->poly != NULL;
}

void sommeRationnels(Rationnel r1, Rationnel r2, Rationnel *r) {
    r->num = r1.num*r2.den + r2.num*r1.den;
    r->den = r1.den*r2.den;
    simplifierRationnel(r);
}

void produitRationnels(Rationnel r1, Rationnel r2, Rationnel *r) {
    r->num = r1.num * r2.num;
    r->den = r1.den * r2.den;
    simplifierRationnel(r);
}