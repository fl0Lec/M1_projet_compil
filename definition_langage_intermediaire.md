# spécification de la structure de donnée

Le code à trois adresse sera stocké dans un tableau 1D **genCode**.
Chaque case du tableau correspond à une ligne de code.
Chaque ligne est représenté sous forme d'une structure de donnée **code3add**.


# opérations arithmétiques

les opérateurs arithmétiques sont :
`{load, loadTab, loadimm, store, add, sub, mul, divi}`

attention les argument peuvent aussi être des constante

## load 
dst := arg1

## loadTab
dst := &arg1[arg2] :l'adresse de arg2 dans le tableau arg1 

## loadimm
dst := val 

## store
dst := arg1

## add
dst := arg1 + arg2

## sub
dst := arg1 - arg2

## mul
dst := arg1 * arg2

## divi
dst := arg1 / arg2

## mod
dst := arg1 % arg2

## subun
dst := - arg1

# opérations de controle

les opérateurs de controle sont :
`{goto, eq, inf, infeq, sup, supeq}`

## eq

goto dst if arg1 == arg2

## noteq

goto dst if arg1 != arg2

## inf

goto dst if arg1 < arg2

## infeq

goto dst if arg1 <= arg2

## sup

goto dst if arg1 > arg2

## supeq

goto dst if arg1 >= arg2

## goto_op
goto dst


## label
dst : est le nom du label (fonction)

## ret 
return dst

## param 
param arg1

## call
call arg1 -> dst : (dans dst seulement si dans expression et fonction a valeur de retour)