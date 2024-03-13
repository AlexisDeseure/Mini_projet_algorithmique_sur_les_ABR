# Projet de TP centré sur les arbres binaires de recherche dans le cadre d'une matière d'algorithmique à l'UTC

Dans ce TP, nous utiliserons les arbres binaires de recherche pour implémenter un exemple d'indexation et de recherche sur un fichier contenant un texte quelconque.
L'arbre binaire de recherche contiendra tous les mots présents dans le texte à indexer :
* Chaque nœud de l'arbre contient un mot, ainsi que la liste de ses positions dans le texte (les positions seront donc gérées par une liste simplement chaînée).
* Une position correspond au numéro de ligne dans laquelle se trouve le mot, son « ordre » dans la ligne (1 pour le premier mot de la ligne, 2 pour le deuxième mot, …etc.), ainsi que le numéro de la phrase dans laquelle il se trouve (1ère phrase du texte, 2ème, 3ème, …etc.).
Dans le cadre de ce TP, on se contentera d’indexer des textes qui ne contiennent que des lettres majuscules, des minuscules et des points (séparateur de phrases). Tout autre caractère ne sera pas pris en considération.
