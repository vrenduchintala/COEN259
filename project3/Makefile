CXX		= g++
CXXFLAGS	= -g -Wall -std=c++11
EXTRAS		= lexer.cpp
OBJS		= Node.o Scope.o Symbol.o Type.o \
		  checker.o lexer.o literal.o parser.o string.o tokens.o
PROG		= tcc

all:		$(PROG)

$(PROG):	$(EXTRAS) $(OBJS)
		$(CXX) -o $(PROG) $(OBJS)

clean:;		$(RM) $(PROG) core a.out *.o

clobber:;	$(RM) $(EXTRAS) $(PROG) core a.out *.o

lexer.cpp:	lexer.l
		$(LEX) $(LFLAGS) -t lexer.l > lexer.cpp
