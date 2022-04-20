CXX := g++
CXXFLAGS := -g -Wall --std=c++11 -DDEBUG
GTESTINCL := -I /usr/include/gtest/  
GTESTLIBS := -lgtest -lgtest_main  -lpthread
VALGRIND := valgrind --tool=memcheck --leak-check=yes

all: schedwork-driver

# generic rule for .cpp files
schedwork.o : schedwork.cpp schedwork.h
	$(CXX) $(CXXFLAGS) $(DEFS) -c $< -o $@ 
schedwork-driver : schedwork-driver.cpp schedwork.o
	$(CXX) $(CXXFLAGS) $(DEFS)  $^ -o $@ 

clean:
	rm -f *.o schedwork-driver *~

.PHONY: clean 