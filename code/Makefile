SOURCE=./
BUILD=../build/
FLAGS=-g

all: $(BUILD)simulator.o $(BUILD)peer.o $(BUILD)event.o $(BUILD)txn.o $(BUILD)blk.o $(BUILD)main.o ./sim

$(BUILD)simulator.o : $(SOURCE)simulator.cpp ./include/header.hpp ./include/cxxopts.hpp
	$(CXX) $(FLAGS) -c -o $(BUILD)simulator.o $(SOURCE)simulator.cpp

$(BUILD)peer.o : $(SOURCE)peer.cpp ./include/header.hpp ./include/cxxopts.hpp
	$(CXX) $(FLAGS) -c -o $(BUILD)peer.o $(SOURCE)peer.cpp

$(BUILD)event.o : $(SOURCE)event.cpp ./include/header.hpp ./include/cxxopts.hpp
	$(CXX) $(FLAGS) -c -o $(BUILD)event.o $(SOURCE)event.cpp

$(BUILD)txn.o : $(SOURCE)txn.cpp ./include/header.hpp ./include/cxxopts.hpp
	$(CXX) $(FLAGS) -c -o $(BUILD)txn.o $(SOURCE)txn.cpp

$(BUILD)blk.o : $(SOURCE)blk.cpp ./include/header.hpp ./include/cxxopts.hpp
	$(CXX) $(FLAGS) -c -o $(BUILD)blk.o $(SOURCE)blk.cpp

$(BUILD)main.o : $(SOURCE)main.cpp ./include/header.hpp ./include/cxxopts.hpp
	$(CXX) $(FLAGS) -c -o $(BUILD)main.o $(SOURCE)main.cpp

./sim : $(BUILD)simulator.o $(BUILD)peer.o $(BUILD)event.o $(BUILD)txn.o $(BUILD) $(BUILD)blk.o $(BUILD)main.o
	$(CXX) $(FLAGS) -o ./sim $(BUILD)simulator.o $(BUILD)peer.o $(BUILD)event.o $(BUILD)txn.o $(BUILD)blk.o $(BUILD)main.o
