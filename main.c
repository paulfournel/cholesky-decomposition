#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

#include <pthread.h>
#include <stdio.h>

// J'ai ajouté un arrondi pour l'affichage des matrices
// afin de simplifier la lecture.

int main()
{
    // Construction d'une matrice SDP
    printf("\nMatrice A\n");
    Matrix *m1=Matrix_create_random_SDP(200);//Matrix_create_sample();
    Matrix_dump(m1);


    // On lance l'agorithme
    Matrix *m2 = cholesky1(m1);
    printf("\nMatrice L\n");
    Matrix_dump(m2);

    // On reconstruit la matrice initiale pour tester le résultat.
    Matrix *m3bis = Matrix_transpose(m2);
    Matrix *m3 = Matrix_product(m2, m3bis);
    printf("\nMatrice L.t(L)\n");
    Matrix_dump(m3);
    int same = Matrix_isSame(m1, m3, 0.01);
    printf("\nres: %d!\n", same);
    printf("\nthis is how it's done!\n");


    return 0;
}
