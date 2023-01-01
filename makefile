CC=g++
RM=del
target=regex.exe
Outputs=Outputs\\
ofile=$(Outputs)nfa.o $(Outputs)regex.o $(Outputs)lexer.o $(Outputs)global.o $(Outputs)dfa.o
header=nfa.h
$(target):$(ofile) $(header)
	$(CC) $(ofile) -o $(target)
$(Outputs)nfa.o:nfa.cpp $(header)
	$(CC) nfa.cpp -c -o $(Outputs)nfa.o
$(Outputs)regex.o:regex.cpp $(header)
	$(CC) regex.cpp -c -o $(Outputs)regex.o
$(Outputs)lexer.o:lexer.cpp $(header)
	$(CC) lexer.cpp -c -o $(Outputs)lexer.o
$(Outputs)global.o:global.cpp $(header)
	$(CC) global.cpp -c -o $(Outputs)global.o
$(Outputs)dfa.o:dfa.cpp $(header)
	$(CC) dfa.cpp -c -o $(Outputs)dfa.o
clean:
	$(RM) $(ofile) regex.exe