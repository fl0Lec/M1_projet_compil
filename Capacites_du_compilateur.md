**Antoine DUMOULIN, Antoine PIERRE, Florent LECOULTRE**

# Capacités du compilateur

## Tests

Le fichier `test.sh` compile tout les fichiers de test du dossier 
`test/` les fichiers de sorties sont redirigés dans `out/`.
Les fichiers .cerr contiennent les tables de symboles, .asm le code mips et .out la sortie du code executé.



## Analyse lexicale (FLEX) & Analyse syntaxique (YACC)

Nos analyseurs lexical et syntaxique traitent tout ce dont le sujet nous demande de faire.

L'analyseur lexical se trouve dans `compilateur.lex`.

L'analyseur syntaxique se trouve dans `compilateur.y`.



## Traduction en langage intermédiaire

### spécification de la structure de donnée utilisé

Le code à trois adresse sera stocké dans un tableau 1D **genCode**.
Chaque case du tableau correspond à une ligne de code.
Chaque ligne est représenté sous forme d'une structure de donnée **code3add**.


#### opérations arithmétiques

Les opérateurs arithmétiques sont :
`{load, loadTab loadimm, store, add, sub, mul, divi}`

Les opérations load et loadimm n'ont finalement pas trouvé leurs places dans la génération de code. 
En effet, la traduction du code en mips s'occupe de load les variables.

Les arguments peuvent être des constantes ou des variables (globales ou locales).

**load :** dst := arg1

**loadimm :** dst := val 

**loadTab:** dst := &arg1[arg2]

**store :** dst := arg1

**add :** dst := arg1 + arg2

**sub :** dst := arg1 - arg2

**mul :** dst := arg1 * arg2

**divi :** dst := arg1 / arg2

**mod :** dst := arg1 % arg2

**subun :** dst := - arg1

#### opérations de controle

les opérateurs de controle sont :
`{label, return, param, call, goto, eq, inf, infeq, sup, supeq}`

**eq :** goto dst if arg1 == arg2

**noteq :** goto dst if arg1 != arg2

**inf :** goto dst if arg1 < arg2

**infeq :** goto dst if arg1 <= arg2

**sup :** goto dst if arg1 > arg2

**supeq :** goto dst if arg1 >= arg2

**goto :** goto dst

**label :** dst : est le nom du label (fonction)

**return :** return dst

**param :** param arg1

**call :** call arg1 -> dst : (dans dst seulement si dans expression et fonction a valeur de retour)


Toutes les fonctions liées au langage intermédiaire se trouvent dans `genCode.c`.

On utilise deux structures pour générer du code intermédiaire : 

- `code3add`, possède un opérateur, deux opérandes et la destination de l'opération de type `symbole`
- `genCode`, possède le tableau de `code3add`



## Gestion de la table des symbole

On affiche la table de symbole avec l'option `-tos`.
La table des symbole est implémentée dans le fichier `symTab.c`.

La structure `symbole` y est présente. Elle contient pour chaque symbole :
- son type ou son descripteur de fonction
- sa valeur (l'identifiant pour les variables, la chaine de caractère pour les string et la valeur numérique pour les constantes)
- son alignement dans la pile $sp



## Génération en langage MIPS

Dès que la totalité du code decaf a été traduite en code intermédiaire, il traduit en mips dans le fichier `genMips.c`. 
Chaque type d'opération de code à 3 adresses est traduit par une fonction.

Le sujet nous demande d'implémenter les fonctions : "WriteString", "WriteInt", "ReadInt" et "WriteBool". Elles sont générés manuellement dans la fonction `void genIOFunctions(FILE* out)` qui s'occupe des fonctions d'entrée et sorties.



## Gestion des options

On lit tout les arguments entrés lors de l'éxécution du programme et si un argument est reconnu comme une option existante, on réalise l'action qui lui est associée.

- tos : affiche la table des symboles
- version : affiche les membres de l'équipe
- o [fichier] : fichier de sortie



## limites du compilateur

Avec une volonté de vouloir développer de manière incrémentale, nous avons considéré que les tableaux n'étaient pas une priorité. Suite à des contraintes de temps la gestion des tableaux est restée à un état primitif où il vaut mieux les utiliser de façon minimaliste pour éviter des bugs.

Les booléens ne sont également pas stables, notamment en paramètre ou en retour de fonction.
Néanmoins les opérations de controles fonctionnent.

La fonction "WriteBool" n'a pas été implémentée par manque de courage, il suffira de l'implémenter en decaf tel que :

```
WriteBool(boolean b) {
    if (b) {
        WriteString("true");
    }
    else {
        WriteString("false");
    }
}
```









