CXX      := g++
CXXFLAGS := -std=c++20 -Isrc
LDLIBS   := $(shell pkg-config --libs ftxui) $(shell pkg-config --libs libpq)

OBJS := src/window/main_screen.o src/window/screens.o src/atm.o src/account.o src/card.o src/db.o

atm.out: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDLIBS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f atm.out $(OBJS)