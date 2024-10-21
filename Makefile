run: $(exec)
	$(exec)

main: $(file)
	g++ $(file) -o $(exec) -Wall -fsanitize=address

clean:
	rm $(exec)
