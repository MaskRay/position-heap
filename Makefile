CXXFLAGS = -g
main: main.cpp
a.out: main.cpp
	$(CXX) $(CXXFLAGS) -D LINEAR_BUILD $<
check: a.out main
	tr -cd a-z < /dev/urandom | head -c 1000 > /tmp/in
	bash -c 'diff <(./main < /tmp/in) <(./a.out < /tmp/in)'
