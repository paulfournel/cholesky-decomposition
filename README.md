Cholesky decomposition
======================


Pour executer le programme sous un terminal Linux:
--------------------------------------------------
- gcc matrix.c  cholesky.c main.c -o cholesky -lpthread -lm
- ./cholesky
  
Bugs:
-----

- La fonction cholesky2Para marche 60% du temps et donne une erreur les autre fois. La source du bug est inconnue.


TODO List:
----------

- Ajouter une mise en forme du menu plus sympa
- Faire une fonction qui permet de tester la vitesse de chaque algorithme
- Donner la possibilité de choisir le nombre de processeur à utiliser.
- Faire un makefile
- Nouvel algo avec http://www.eecs.berkeley.edu/Pubs/TechRpts/2009/EECS-2009-29.pdf
