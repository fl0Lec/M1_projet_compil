# execution du compilateur
# ./decaf test_program_id.decaf  out/test1.asm
# execution du programme compilé
# java -jar Mars4_5.jar test/out/test1.asm
mkdir out
for file in test/unitaire/*; do 
filename=$(basename -- "$file")
filename=${filename%%.*}
echo "/================================================/"
echo "compilation et execution de ${filename}"
echo "/================================================/"
./decaf $file -o out/$filename.asm -tos;
spim -quiet -file out/$filename.asm > out/$filename.out;
done
