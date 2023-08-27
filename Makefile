all: compile link

compile:
	g++  -c main.cpp -I"C:\Users\colin\OneDrive\Desktop\chessProject\sfml\include"

	g++  -c board.cpp -I"C:\Users\colin\OneDrive\Desktop\chessProject\sfml\include"

	g++  -c rules.cpp -I"C:\Users\colin\OneDrive\Desktop\chessProject\sfml\include"

	g++  -c AI.cpp -I"C:\Users\colin\OneDrive\Desktop\chessProject\sfml\include"

	g++  -c test.cpp -I"C:\Users\colin\OneDrive\Desktop\chessProject\sfml\include"

	g++  -c utility.cpp -I"C:\Users\colin\OneDrive\Desktop\chessProject\sfml\include"

link:

	g++  main.o board.o rules.o AI.o utility.o -o main -L"C:\Users\colin\OneDrive\Desktop\chessProject\sfml\lib" -lsfml-graphics -lsfml-window -lsfml-system
          
	g++   board.o rules.o AI.o test.o utility.o -o test -L"C:\Users\colin\OneDrive\Desktop\chessProject\sfml\lib" -lsfml-graphics -lsfml-window -lsfml-system