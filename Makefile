CC := g++ 
EXE := ./bin/main
DIR_SRC := ./src
DIR_OBJ := ./obj
DIR_LIB := ./lib
DIR_INC := ./include

SRC := $(wildcard $(DIR_SRC)/*.cpp)
OBJ := $(patsubst %.cpp, $(DIR_OBJ)/%.o, $(notdir $(SRC)))

CFLAG := -I$(DIR_INC)  -L$(DIR_LIB) -g  -Wall -lpthread -llog4cpp -std=c++11 -DMY_DEBUG


$(EXE): $(OBJ)
	$(CC)  -o $@ $^ $(CFLAG)  

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp
	$(CC) -c -o $@ $^ $(CFLAG)  

.PHONY: clean
clean:
	find $(DIR_OBJ) -name *.o -exec rm -rf {} \; 


