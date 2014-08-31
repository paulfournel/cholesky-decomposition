#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>

#include "matrix.h"

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

struct arg_struct {
    Matrix *m;
    int i0;
    int in;
    int i;
    int j;
    int threadNb;
    int pas;
    float *s;
};

Matrix *cholesky1(Matrix *matOrigin){
    int i,j,k;
    Matrix *mat = Matrix_clone(matOrigin);
    int n = mat->rows;
    // A faire...
    // Ajouter des tests pour savoir si la matrice est bien SDP

    Matrix *l = Matrix_create(n,n);

    // Algorithme sans threads.
    // - i le numéro de la colonne et j celui de la ligne
    // - Note, il y a peut être moyen de réduire à seulement deux boucles?
    for(i=0;i<n; i++){
        for(j=i;j<n; j++){
            double somme = 0;
            if(i==j){
                for(k=0; k<(i); k++){
                    somme += Matrix_get(l,i,k) * Matrix_get(l,i,k);
                }
                Matrix_set(l,i,j, sqrt(Matrix_get(mat,i,j)-somme));
            }else{
                for(k=0; k<j-1; k++){
                    somme += Matrix_get(l,i,k)*Matrix_get(l,j,k);
                }
                Matrix_set(l,j,i, (Matrix_get(mat,i,j)-somme)/Matrix_get(l,i,i));
            }
        }
    }
    return(l);
}

void *calculSommeDiag(void *arguments)
{
    struct arg_struct *args = arguments;

    double sum=0;
    double *totalsum = args -> s;
    int ii = args -> i;
    int pas = args -> pas;
    int k0 = args -> threadNb;
    int kk;
    for(kk=k0; kk<ii; kk+=pas){
        sum += Matrix_get(args -> m,ii,kk) * Matrix_get(args -> m,ii,kk);
    }
    //pthread_mutex_lock(&mut);
    *totalsum += sum;
    //pthread_mutex_unlock (&mut);
    return NULL;
}

void *calculSommeAutre(void *arguments)
{
    struct arg_struct *args = arguments;
    int k;
    double sum=0;
    double *totalsum = args -> s;
    int ii = args -> i;
    int jj = args -> j;
    int pas = args -> pas;
    int k0 = args -> threadNb;
    for(k=k0; k<jj-1; k+=pas){
        sum += Matrix_get(args -> m,ii,k) * Matrix_get(args -> m,jj,k);
    }
    //pthread_mutex_lock(&mut);
    *totalsum += sum;
    //pthread_mutex_unlock (&mut);
    return NULL;
}


Matrix *cholesky2Para(Matrix *matOrigin){
    int threadNumber = 3;
    int i,j,k;
    Matrix *mat = Matrix_clone(matOrigin);
    int n = mat->rows;

    Matrix *l = Matrix_create(n,n);
    // Algorithme avec threads.
    // - i le numéro de la colonne et j celui de la ligne
    // - Note, dans cette version on parallélise seulement la troisème boucle.
    for(i=0;i<n; i++){
        for(j=i;j<n; j++){
            double somme = 0;
            if(i==j){
                // On lance plusieurs threads
                pthread_t thread1, thread2;

                int tn;
                // On définit un tableau donc chaque element va être passé en param à un thread
                struct arg_struct args[threadNumber];
                for(tn=0;tn<threadNumber;tn++){
                    // On initialise les arguments.
                    args[tn].s = &somme;
                    args[tn].m = l;
                    args[tn].i = i;
                    args[tn].j = j;
                    args[tn].threadNb = tn;
                    args[tn].pas = threadNumber;
                }

                // On lance les threads.
                pthread_create (&thread1,  NULL, calculSommeDiag, (void *)&args[0]);
                pthread_create (&thread2,  NULL, calculSommeDiag, (void *)&args[1]);

                // On attend la fin de tous les threads.
                // On attend la fin de tous les threads.
                pthread_join(thread1, NULL);
                pthread_join(thread2, NULL);


                // On actualise l'élément de la matrice.

                Matrix_set(l,i,j, sqrt(Matrix_get(mat,i,j)-somme));
            }else{
                // On lance plusieurs threads
                //pthread_t threads[threadNumber];

                pthread_t thread1, thread2;
                int tn;
                // On définit un tableau donc chaque element va être passé en param à un thread
                struct arg_struct args[2];
                for(tn=0;tn<2;tn++){
                    // On initialise les arguments.
                    args[tn].s = &somme;

                    args[tn].m = l;
                    args[tn].i = i;
                    args[tn].j = j;
                    args[tn].threadNb = tn;
                    args[tn].pas = threadNumber;
                }

                pthread_create (&thread1, NULL, calculSommeAutre, (void *)&args[0]);
                pthread_create (&thread2, NULL, calculSommeAutre, (void *)&args[1]);

                // On attend la fin de tous les threads.
                pthread_join(thread1, NULL);
                pthread_join(thread2, NULL);

                Matrix_set(l,j,i, (Matrix_get(mat,i,j)-somme)/Matrix_get(l,i,i));
            }
        }
    }
    return(l);
}

