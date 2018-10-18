P=predictors
CFLAGS= -Wall -Wextra -DDBUG -g -pedantic -std=c++14

all: $(P)

$(P).o: $(P).cpp
	g++ $(CFLAGS) -c $(P).cpp -o $(P).o

$(P): $(P).o
	g++ $(CFLAGS) $(P).o -o $(P)

run: all
	./$(P) short_trace1.txt output.txt

clean:
	rm $(P).o
	rm $(P)
