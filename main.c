#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

#include <pthread.h>
#include <stdio.h>

void user_input_test(){
    double d;
    int i,j;
    Matrix *mat = Matrix_create(4,4);
    for(i=0;i<4;i++){
        for(j=i;j<4;j++){
            printf("\nA(%d,%d) = ", i+1,j+1);
            scanf("%lf", &d);
            Matrix_set(mat,i,j,d);
            Matrix_set(mat,j,i,d);
        }
    }
    printf("\nMatrice A\n");
    Matrix_dump(mat);

    printf("\nWhat algorithm should we use?\n\n 1. Normal\n 2. Parallel\n 3. Return to main\n");
    int choice=-1;
    while(choice<1 || choice>3){
        printf("\n>: ");
        scanf("%d", &choice);
        if(choice==1){
            Matrix *m2 = cholesky1(mat);
            printf("\nMatrice L\n");
            Matrix_dump(m2);
        }
        if(choice==2){
            Matrix *m2 = cholesky2Para(mat);
            printf("\nMatrice L\n");
            Matrix_dump(m2);
        }
        if(choice==3){
            //do nothing
        }
    }
}

void random_matrix_test(){
    // Construction d'une matrice SDP
    printf("\nMatrice A created:\n");
    Matrix *m1=Matrix_create_random_SDP(10);//Matrix_create_sample();
    Matrix_dump(m1);


    printf("\nWhat algorithm should we use?\n\n 1. Normal\n 2. Parallel\n 3. Return to main\n");
    int choice=-1;
    Matrix *m2;
    while(choice<1 || choice>3){
        printf("\n>: ");
        scanf("%d", &choice);
        if(choice==1){
            m2 = cholesky1(m1);
            printf("\nMatrix L\n");
            Matrix_dump(m2);
        }
        if(choice==2){
            m2 = cholesky2Para(m1);
            printf("\nMatrix L\n");
            Matrix_dump(m2);
        }
        if(choice==3){
            //do nothing
        }
    }

    // On reconstruit la matrice initiale pour tester le résultat.
    Matrix *m3bis = Matrix_transpose(m2);
    Matrix *m3 = Matrix_product(m2, m3bis);
    printf("\nMatrix L.t(L)\n");
    Matrix_dump(m3);
    int same = Matrix_isSame(m1, m3, 0.01);
    if(same==1){
        printf("\n***  The algorithm worked   ***\n");
    }else{
        printf("An error has occured\n");
    }
}

int main()
{
    // ask user what to do

    int choice=-1;
    while(choice!=3){
        printf("\nWhat would you like to do?\n\n 1. Enter a 4*4 matrix\n 2. Test with a random matrix\n 3. Exit\n");
        printf("\n>: ");
        scanf("%d", &choice);
        if(choice==1){
            user_input_test();
        }
        if(choice==2){
            random_matrix_test();
        }
    }

/*
//    fscanf (stdin, "%s", mychar);

    // Construction d'une matrice SDP
    printf("\nMatrice A\n");
    Matrix *m1=Matrix_create_random_SDP(10);//Matrix_create_sample();
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
*/
    return 0;
}
