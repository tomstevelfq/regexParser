CC=g++ -g
RM=del
target=main.exe
Outputs=Outputs\\
ofile=$(Outputs)nfa.o $(Outputs)regex.o $(Outputs)lexer.o $(Outputs)global.o $(Outputs)dfa.o $(Outputs)main.o

$(target):$(ofile)
	$(CC) $(ofile) -o $(target)
$(Outputs)nfa.o:nfa.cpp
	$(CC) nfa.cpp -c -o $(Outputs)nfa.o
$(Outputs)regex.o:regex.cpp
	$(CC) regex.cpp -c -o $(Outputs)regex.o
$(Outputs)lexer.o:lexer.cpp
	$(CC) lexer.cpp -c -o $(Outputs)lexer.o
$(Outputs)global.o:global.cpp
	$(CC) global.cpp -c -o $(Outputs)global.o
$(Outputs)dfa.o:dfa.cpp
	$(CC) dfa.cpp -c -o $(Outputs)dfa.o
$(Outputs)main.o:main.cpp
	$(CC) main.cpp -c -o $(Outputs)main.o
clean:
	$(RM) $(ofile) $(target)