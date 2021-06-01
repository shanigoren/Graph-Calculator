CC=/usr/local/bin/gcc
CXX=/usr/local/bin/g++
OBJS = Calculator.o CommandHandler.o Exceptions.o Graph.o Graph_Interface.o main.o
EXEC = gcalc
DEBUG = -g
CFLAGS = -std=c99 -Wall -Werror -pedantic-errors -DNDEBUG $(DEBUG) 
CXXFLAGS = -std=c++11 -Wall -Werror -pedantic-errors -fPIC -DNDEBUG

$(EXEC) : $(OBJS)
	$(CXX) $(DEBUG_FLAG) $(OBJS) -o $@


Calculator.o: Calculator.cpp Calculator.h Graph.h CommandHandler.h Exceptions.h
CommandHandler.o: CommandHandler.cpp CommandHandler.h Exceptions.h
Exceptions.o: Exceptions.cpp Exceptions.h
Graph.o: Graph.cpp Graph.h Exceptions.h CommandHandler.h
Graph_Interface.o: Graph_Interface.cpp Graph_Interface.h Graph.h
main.o: main.cpp Exceptions.h Calculator.h Graph.h CommandHandler.h
libgraph.a: $(OBJS)
	ar -rs $@ $^

clean:
	rm -f $(OBJS) libgraph.a $(EXEC)

tar:
	zip gcalc.zip Makefile design.pdf test_in.txt test_out.txt graph.i *.cpp *.h *.gc

swig:
	swig -python graph.i
	g++ -std=c++11 -DNDEBUG --pedantic-errors -Wall -Werror -I/usr/local/include/python3.6m -fPIC -shared graph_wrap.c libgraph.a -o _graph.so