all:
	g++ -IC:/mingw_dev_lib/include/SDL2 -LC:/mingw_dev_lib/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf