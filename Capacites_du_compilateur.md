# Capacités du compilateur



## Analyse lexicale (FLEX)

Notre analyseur lexical traite tout ce que le sujet nous demande de faire, c'est-à-dire :

- les mots clés (pour les structures de contrôle, le typage, etc..)
- les opérations arithmétiques et logiques
- les commentaires
- les chaînes de caractères

L'analyseur lexical se trouve dans `compilateur.lex`.



## Analyse syntaxique (YACC)

L'analyseur syntaxique gère les opérations arithmétiques ainsi que les opérations de contrôle.

En revanche il ne gère pas les tableaux. On pensait que cela rajouterait  de la complexité pour construire notre compilateur et une volonté de vouloir le créer de façon incrémentale, nous avons considéré que les tableaux n'étaient pas une priorité. Suite à des contraintes de temps il n'était plus envisageable d'implémenter la gestion des tableaux.

Notre analyseur syntaxique se trouve dans `compilateur.y`.



## Traduction en langage intermédiaire

### spécification de la structure de donnée utilisé

Le code à trois adresse sera stocké dans un tableau 1D **genCode**.
Chaque case du tableau correspond à une ligne de code.
Chaque ligne est représenté sous forme d'une structure de donnée **code3add**.



#### opérations arithmétiques

Les opérateurs arithmétiques sont :
`{load, loadimm, store, add, sub, mul, divi}`

La difference entre load et store est que load met une variable dans un temporaire tandis que store fais l'inverse.
Attention les argument peuvent aussi être des constantes

**load :** dst := arg1

**loadimm :** dst := val 

**store :** dst := arg1

**add :** dst := arg1 + arg2

**sub :** dst := arg1 - arg2

**mul :** dst := arg1 * arg2

**divi :** dst := arg1 / arg2

**mod :** dst := arg1 % arg2

**subun :** dst := - arg1

#### opérations de controle

les opérateurs de controle sont :
`{goto, eq, inf, infeq, sup, supeq}`

**eq :** goto dst if arg1 == arg2

**noteq :** goto dst if arg1 != arg2

**inf :** goto dst if arg1 < arg2

**infeq :** goto dst if arg1 <= arg2

**sup :** goto dst if arg1 > arg2

**supeq :** goto dst if arg1 >= arg2

**goto_op :** goto dst

**label :** dst : est le nom du label (fonction)

**ret :** return dst

**param :** param arg1

**call :** call arg1 -> dst : (dans dst seulement si dans expression et fonction a valeur de retour)



Toutes les fonctions liées au langage intermédiaire se trouvent dans `genCode.c`.

On utilise deux structures pour générer du code intermédiaire : 

- `code3add`,  qui possède un opérateur, deux opérandes et la destination de l'opération
- `genCode`, possède une structure `code3add` ainsi que des informations tels que la taille ou encore un marqueur `current` permettant de savoir quel symbole on est en train de traiter





## Génération en langage MIPS

La plupart des fonctions utilisées pour générer du code MIPS prennent en argument une structure `code3add`, celle-ci est nécessaire puisque c'est cette structure qui possède le code intermédiaire à traduire. Le second argument est un fichier, c'est dans ce fichier que l'on va écrire le code MIPS résultant des fonctions. *Exemple : `void genMod(struct code3add instr, FILE* out)`, `void genGoto(struct code3add instr, FILE* out)`* 

Le sujet nous demande d'implémenter des fonctions tels que "WriteString", "WriteInt", etc... Elles sont générés dans la fonction `void genIOFunctions(FILE* out)` qui s'occupe des fonctions d'entrée et sorties.

Enfin, nous avons une fonction `void genMips(FILE* out)` qui va appeler toutes les fonctions générant du code MIPS en fonction des opérations rencontrées dans le langage intermédiare.

On retrouve toutes ces fonctions dans le fichier `genMips.c`.





## Gestion de la table des symbole

La table des symbole est initialisé dans le fichier `decaf.c`.

On ajoute, dépile et effectue d'autres opérations sur les symbole et on créée, affiche et d'autres opérations sur les tables de symbole avec les fonctions dans le ficher `symTab.c`.

On affiche la table de symbole avec l'option `-tos`.





## Gestion des options

On lit tout les arguments entrés lors de l'éxécution du programme et si un argument est reconnu comme une option existante, on réalise l'action qui lui est associé.