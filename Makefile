run: $(exec)
	bin/$(exec)

main: $(file)
	g++ $(file) -o ./bin/$(exec) -Wall -fsanitize=address

clean:
	rm $(exec)
