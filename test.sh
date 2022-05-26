mkdir out
for file in test/*; do 
filename=$(basename -- "$file")
filename=${filename%%.*}
./decaf $file -o out/$filename.asm -tos > out/$filename.cerr || continue;
spim -quiet -file out/$filename.asm > out/$filename.out;
done
