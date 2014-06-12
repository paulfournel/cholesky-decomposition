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

Matrix *cholesky1(Matrix *mat){
    int i,j,k;
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
    pthread_mutex_lock(&mut);
    *totalsum += sum;
    pthread_mutex_unlock (&mut);
    //printf("t: %d, sumtmp: %.2f, sumTot: %.2f\n", pas, sum, *totalsum);
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
    pthread_mutex_lock(&mut);
    *totalsum += sum;
    pthread_mutex_unlock (&mut);
    return NULL;
}


Matrix *cholesky2Para(Matrix *mat){
    int threadNumber = 2;
    int i,j,k;
    int n = mat->rows;

    Matrix *l = Matrix_create(n,n);

    // Algorithme avec threads.
    // - i le numéro de la colonne et j celui de la ligne
    // - Note, dans cette version on parallélise seulement la troisème boucle.
    for(i=0;i<n; i++){
        for(j=i;j<n; j++){
            double somme = 0;
            if(i==j){
                printf("i=%d", i);
                // On lance plusieurs threads
                pthread_t threads[threadNumber];
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

                    // On lance les threads.
                    if (pthread_create(&threads[i], NULL, calculSommeDiag, (void *)&args[tn]) != 0) {
                        return -1;
                    }

                }
                // On attend la fin de tous les threads.
                for(tn=0;tn<threadNumber;tn++){
                    pthread_join(threads[i], NULL);
                }

                /*int somme2 = 0;
                int kkk;
                for(kkk=0; kkk<(i); kkk++){
                    somme2 += Matrix_get(l,i,kkk) * Matrix_get(l,i,kkk);
                }
                printf("s1; %.2f, s2: %.2f\n", somme, somme2);*/
                // On actualise l'élément de la matrice.
                Matrix_set(l,i,j, sqrt(Matrix_get(mat,i,j)-somme));
            }else{
                // On lance plusieurs threads
                pthread_t threads[threadNumber];
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

                    // On lance les threads.
                    if (pthread_create(&threads[i], NULL, calculSommeAutre, (void *)&args[tn]) != 0) {
                        return -1;
                    }
                }
                // On attend la fin de tous les threads.
                for(tn=0;tn<threadNumber;tn++){
                        pthread_join(threads[i], NULL);
                    }
                Matrix_set(l,j,i, (Matrix_get(mat,i,j)-somme)/Matrix_get(l,i,i));
            }
        }
    }
    return(l);
}

