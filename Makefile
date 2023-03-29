File = sfml.cpp

all:
	g++ -fsanitize=address $(File) -I/opt/homebrew/Cellar/sfml/2.5.1_2/include -o prog -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio




