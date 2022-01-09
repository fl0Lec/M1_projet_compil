mkdir out
for file in test/unitaire/*; do 
filename=$(basename -- "$file")
filename=${filename%%.*}
echo "========================= compile $filename ========================="
./decaf $file -o out/$filename.asm -tos > out/$filename.cerr;
spim -quiet -file out/$filename.asm > out/$filename.out;
done
