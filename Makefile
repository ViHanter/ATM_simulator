CXX      := g++
CXXFLAGS := -std=c++20 -Isrc
LDLIBS   := $(shell pkg-config --libs ftxui)

OBJS := src/window/main_screen.o src/bank.o src/atm.o src/account.o src/card.o

atm.out: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDLIBS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f atm.out $(OBJS)