#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

#include <pthread.h>
#include <stdio.h>
#include <time.h>

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
    printf("\nMatrix A\n");
    Matrix_dump(mat);
    printf("\What algorithm should we use?\n\n 1. Normal\n 2. Parallel\n 3. Return to main\n");
    int choice=-1;
    Matrix *m2;

    while(choice<1 || choice>3){
        printf("\n>: ");
        scanf("%d", &choice);
        if(choice==1){
            m2 = cholesky1(mat);
            printf("\nMatrix L\n");
            Matrix_dump(m2);
        }
        if(choice==2){
            m2 = cholesky2Para(mat);
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
    int same = Matrix_isSame(mat, m3, 0.01);
    if(same==1){
        printf("\n*******************************");
        printf("\n***                         ***");
        printf("\n***  The algorithm worked   ***");
        printf("\n***                         ***");
        printf("\n*******************************\n\n");
    }else{
        printf("\n*******************************");
        printf("\n***                         ***");
        printf("\n***  An error has occured   ***");
        printf("\n***                         ***");
        printf("\n*******************************\n\n");
    }
}

void random_matrix_test(){
    // Construction d'une matrice SDP
    int msize;
    printf("\nEnter the size of the matrix?\n");
    printf("\n>: ");
    scanf("%d", &msize);
    printf("\nMatrice A created:\n");
    Matrix *m1=Matrix_create_random_SDP(msize);//Matrix_create_sample();
    Matrix_dump(m1);


    printf("\nWhat algorithm should we use?\n\n 1. Normal\n 2. Parallel\n 3. Return to main\n");

    int choice;

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
        printf("\n*******************************");
        printf("\n***                         ***");
        printf("\n***  The algorithm worked   ***");
        printf("\n***                         ***");
        printf("\n*******************************\n\n");
    }else{
        printf("\n*******************************");
        printf("\n***                         ***");
        printf("\n***  An error has occured   ***");
        printf("\n***                         ***");
        printf("\n*******************************\n\n");
    }
}

void run_speed_test(){
    Matrix *m1 = Matrix_create_random_SDP(200);

    clock_t start = clock(), diff;
    // Start test
    Matrix *m2 = cholesky1(m1);


    diff = clock() - start;

    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds", msec/1000, msec%1000);

    start = clock();
    // Start test
    Matrix *m3 = cholesky2Para(m1);


    diff = clock() - start;

    msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds", msec/1000, msec%1000);
}

int main()
{
    // Ask user what to do

    int choice=-1;
    while(choice!=4){
        printf("\nWhat would you like to do?");
        printf("\n--------------------------");
        printf("\n\n 1. Enter a 4*4 matrix\n 2. Test with a random matrix\n 3. Run speed comparaison test\n 4. Exit\n");
        printf("\n>: ");
        scanf("%d", &choice);
        if(choice==1){
            user_input_test();
        }
        if(choice==2){
            random_matrix_test();
        }
        if(choice==3){
            run_speed_test();
        }
    }

    return 0;
}
