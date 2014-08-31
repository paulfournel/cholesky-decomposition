#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "chol.h"

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
    printf("\nMatrix A created:\n");
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

void run_cholesky_2(){
    // Matrices for the program
	Matrix2 A; // The N x N input matrix
	Matrix2 reference; // The upper triangular matrix computed by the CPU
	Matrix2 U_pthreads; // The upper triangular matrix computed by the pthread implementation
	Matrix2 U_openmp; // The upper triangular matrix computed by the openmp implementation

	// Initialize the random number generator with a seed value
	srand(time(NULL));

	// Create the positive definite matrix. May require a few tries if we are unlucky
	int success = 0;
	while(!success){
		A = create_positive_definite_matrix(MATRIX_SIZE, MATRIX_SIZE);
		if(A.elements != NULL)
				  success = 1;
	}
	 print_matrix(A);
}

void run_speed_test(){
    int i;
    for(i=0;i<10;i++){
        Matrix *m1 = Matrix_create_random_SDP(1400);
        printf("\nTest on a 200*200 matrix:\n");

        clock_t start = clock(), diff;
        // Start test
        Matrix *m2 = cholesky1(m1);


        diff = clock() - start;

        int msec = diff * 1000 / CLOCKS_PER_SEC;
        printf("Time taken for the normal algorithm: %d seconds %d milliseconds", msec/1000, msec%1000);

        clock_t start2 = clock(), diff2;
        // Start test
        Matrix *m3 = cholesky2Para(m1);


        diff2 = clock() - start2;

        int msec2 = diff2 * 1000 / CLOCKS_PER_SEC;
        printf("\nTime taken for the parallel algorithm: %d seconds %d milliseconds\n", msec2/1000, msec2%1000);
    }
    printf("\n***   Conclusion this algorithm is bad   ***\n");
}

int main()
{
    // Ask user what to do

    int choice=-1;
    while(choice!=5){
        printf("\nWhat would you like to do?");
        printf("\n--------------------------");
        printf("\n\n 1. Enter a 4*4 matrix\n 2. Test with a random matrix\n 3. Run speed comparaison test\n 4. Cholesky2\n 5. Exit\n");
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
        if(choice==4){
            run_cholesky_2();
        }
    }

    return 0;
}
