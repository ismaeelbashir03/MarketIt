CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -I include/ -I /opt/homebrew/Cellar/crow/1.2.0/include -I /opt/homebrew/Cellar/asio/1.30.2/include -I /opt/homebrew/Cellar/nlohmann-json/3.11.3/include -I /opt/homebrew/Cellar/cpp-httplib/0.18.3/include

SRC_DIR = src
OUT_DIR = output
TEST_DIR = tests

SOURCES = $(SRC_DIR)/server.cpp $(SRC_DIR)/orderbook/Orderbook.cpp $(SRC_DIR)/models/Event.cpp $(SRC_DIR)/models/User.cpp
MAIN_SOURCES = $(SRC_DIR)/main.cpp $(SOURCES)
TEST_SOURCES = $(TEST_DIR)/test.cpp	$(SOURCES)
# main target
build: 
	mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) $(MAIN_SOURCES) -o $(OUT_DIR)/server
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o $(OUT_DIR)/test

run:
	./$(OUT_DIR)/server

server: build run

# test target
test: build
	./$(OUT_DIR)/test

# valgrind target
valgrind_run:
	valgrind --leak-check=full ./$(OUT_DIR)/main

valgrind: build valgrind_run

# mac memory leak target
leak_run:
	ASAN_OPTIONS=detect_leaks=1 ./output/main

leak: build leak_run

# help target
help:
	@echo "make build - compile the project"
	@echo "make run - run the server"
	@echo "make test - run the tests"
	@echo "make valgrind - run the server with valgrind"
	@echo "make leak - run the server with memory leak detection"
	@echo "make help - print this message"
	@echo "make clean - remove all output files"

clean:
	rm -f $(OUT_DIR)/*
