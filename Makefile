all:  
	@echo "Compiling"
	@c++ -std=c++17 BESDL.cpp gameMain.cpp -I /usr/include/SDL2 -I . -lSDL2 -lSDL2_image -lSDL2_ttf -o main -g
	@./main
	@rm main

beshan: 
	@echo "Compiling"
	@c++ -std=c++17 BESDL.cpp test/main.cpp -I /usr/include/SDL2 -I . -lSDL2 -lSDL2_image -lSDL2_ttf -o main
	@./main
	@rm main

