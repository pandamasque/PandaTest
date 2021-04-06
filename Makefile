DPNDCS= #This is the code/lib that should be tested, a receipe for it can be added
EXEC=Tests.exe
TST_FLDR=tests/
SRC_FLDR=sources/
BLD_FLDR=build/
STD=-std=c++17
CC=g++
INCLD_PTH= -I $(TST_FLDR) -I $(SRC_FLDR)
CPPFLAGS= -O $(STD) $(INCLD_PTH)
SRC=$(wildcard $(TST_FLDR)*.cpp)
RPLC=$(SRC:.cpp=.o)
OBJ=$(subst $(TST_FLDR), $(BLD_FLDR), $(RPLC))


all: $(EXEC) clean
	./$(BLD_FLDR)$(EXEC)
$(BLD_FLDR)%.o : $(TST_FLDR)%.cpp
	$(CC) $(CPPFLAGS) -c $^ -o $@
$(EXEC) : $(OBJ) $(DPNDCS)
	$(CC) $(CPPFLAGS) $^ -o $(BLD_FLDR)$@
clean:
	rm -f $(BLD_FLDR)*.o
mrproper: clean
	rm -f $(BLD_FLDR)$(EXEC)
