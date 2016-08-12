FLAG = lita
CFLAGS = -g -O2
LDFLAGS = -g -lpthread

all:
	g++ $(CFLAGS) -c -o $(FLAG).o $(FLAG).cpp
	g++ -o $(FLAG).exe *.o  $(LDFLAGS)

clean:
	rm -rf *.o $(FLAG).exe
	rm -rf *.log

run:
	./$(FLAG).exe 200 18 6 8
	
