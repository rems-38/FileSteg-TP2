main: main.o
	gcc -o $@ $^ -g

main.o: main.c
	gcc -c $< -g


clean:
	rm -rf *.o main