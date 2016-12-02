all:
	g++ colorhandler.cpp -lboost_system -lboost_context -lpthread --std=c++11 -o colorhandler
