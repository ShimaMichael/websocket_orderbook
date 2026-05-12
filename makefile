CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic 
CXX = g++
SRCS = main.cpp


LIBS = -lboost_system -pthread -lssl -lcrypto 

all: streamer
streamer: $(SRCS)
	$(CXX) $(CXXFLAGS) -o streamer $(SRCS) $(LIBS)