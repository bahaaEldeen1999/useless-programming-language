generate_lex:
	lex  -o build/lex.c src/lex/lex.l 
	gcc -o build/lex.out build/lex.c -lfl 
clean:
	rm -rf build/*