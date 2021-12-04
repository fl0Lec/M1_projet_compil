# spécification de la structure de donnée

Le code à trois adresse sera stocké dans un tableau 1D **genCode**.
Chaque case du tableau correspond à une ligne de code.
Chaque ligne est représenté sous forme d'une structure de donnée **code3add**.


# opérations arithmétiques

les opérateurs arithmétiques sont :
`{load, loadimm, store, add, sub, mul, divi}`

## load


## loadimm


## store


## add
dst := arg1 + arg2

## sub
dst := arg1 - arg2

## mul
dst := arg1 * arg2

## divi
dst := arg1 / arg2


# opérations de controle

les opérateurs de controle sont :
`{goto, eq, inf, infeq, sup, supeq}`

## eq

goto dst if arg1 == arg2

## inf

goto dst if arg1 < arg2

## infeq

goto dst if arg1 <= arg2

## sup

goto dst if arg1 > arg2

## supeq

goto dst if arg1 >= arg2


## goto
goto dst


