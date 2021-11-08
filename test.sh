# compilation
make
# execution du compilateur
./decaf -o out/test1.asm
# execution du programme compilé
java -jar Mars4_5.jar test/out/test1.asm
# nettoyage auto
make clean
