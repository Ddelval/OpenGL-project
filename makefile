#CC=g++-9 -g -fsanitize=address -std=c++17
#CC=g++-9 -Ofast -std=c++17
CC = clang++ -std=c++17 -g -fsanitize=address
LIB= -IGame/ 
FRAMEWOKS=-lglfw3  -framework Cocoa -framework OpenGL -framework IOKit -framework CoreFoundation -framework CoreVideo 

ENTITY=$(shell find Game/Entity -name "*.cpp" )
GAMECUBES=$(shell find Game/GameCubes -name "*.cpp" )
GENERALMODULES=$(shell find Game/GeneralModules -name "*.cpp" )
GRAPHICS=$(shell find Game/Graphics -name "*.cpp" )
RANDOM=$(shell find Game/Random -name "*.cpp" )
TESTING=$(shell find Game/Testing -name "*.cpp" )
EXTERNAL=$(shell find Game/External -name "*.cpp" )

MODS= $(ENTITY) $(GAMECUBES) $(GENERALMODULES) $(GRAPHICS) $(RANDOM) $(TESTING) $(EXTERNAL)

OBJS=$(foreach F, $(MODS),bin/$(subst Game/,,$(subst .cpp,,$F)).o)
all: executable
	@echo $(OBJS)
clean: 
	rm $(OBJS)
	rm executable

executable: $(OBJS)

	$(CC) -o $@ $(OBJS) $(LIB) $(FRAMEWOKS)
run: executable 
	./executable


$(OBJS):bin/%.o: Game/%.cpp
	
	$(CC) -c $< -o $@ $(LIB)
