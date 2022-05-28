compile:
	rm -rf build/*
	mkdir -p build
	flex  -o build/lex.c src/lex/lex.l 
	bison -o build/parse.tab.c -d src/parser/parser.y
	gcc  -o build/app.out build/parse.tab.c build/lex.c -lfl 
	cp src/gui/main.py build/gui.py
clean:
	rm -rf build/*