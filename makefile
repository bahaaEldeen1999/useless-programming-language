compile:
	rm -rf build/*
	mkdir -p build
	lex  -o build/lex.c src/lex/lex.l 
	bison -o build/parse.tab.c -d src/parser/parser.y
	gcc -w -o build/app.out build/parse.tab.c build/lex.c -lfl 
clean:
	rm -rf build/*