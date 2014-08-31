Cholesky decomposition
======================

Ce projet a pour objectif de proposer une version parallèle de la décomposition de Cholesky. Plus d'information sur http://en.wikipedia.org/wiki/Cholesky_decomposition


### Comment ça marche

Pour executer le programme sous un terminal Linux:

```
- gcc matrix.c  cholesky.c main.c -o cholesky -lpthread -lm
- ./cholesky
```

Ensuite il suffit de naviguer dans le menu pour tester les différentes fonctions.

### Bugs:

- La fonction cholesky2Para marche 60% du temps et donne une erreur les autre fois. La source du bug est inconnue.


### TODO List:

- Trouver un nouvel algo http://www.eecs.berkeley.edu/Pubs/TechRpts/2009/EECS-2009-29.pdf
- Possibilité de choisir le nombre de threads
- 
