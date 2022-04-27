generate_lex:
	mkdir -p build
	lex  -o build/lex.c src/lex/lex.l 
	gcc -o build/lex.out build/lex.c -lfl 
clean:
	rm -rf build/*