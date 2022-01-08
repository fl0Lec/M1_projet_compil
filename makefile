CC=gcc
CFLAGS=-Wall -g

src=src/compilateur
bin=bin/compilateur
doc=doc/compilateur


all: bin decaf

bin:
	mkdir bin

decaf: $(bin).tab.o bin/lex.yy.o bin/decaf.o bin/symTab.o bin/tabD.o bin/genCode.o bin/genMips.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(src).tab.c: $(src).y
	bison -t -d $< -o $@

src/lex.yy.c: $(src).lex $(src).tab.h
	flex $(src).lex
	mv lex.yy.c src/lex.yy.c

bin/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

doc:
	mkdir -p doc
	bison --report=all --report-file=$(doc).output \
		--graph=$(doc).dot --output=/dev/null \
		$(src).y
	dot -Tpdf < $(doc).dot > $(doc).pdf

clean:
	rm -rf out bin doc decaf src/compilateur.tab.h src/compilateur.tab.c src/lex.yy.c

test: test.sh decaf
	./test.sh
