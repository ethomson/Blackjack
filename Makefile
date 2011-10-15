# Blackjack Simulation
# Copyright (c) 2004, Ed Thomson <ethomson@ravecomm.com>
#
# Makefile for GNU make or equivalent.

SRC=			Blackjack.cpp			\
				Assert.cpp				\
				Card.cpp				\
				Hand.cpp				\
				Player.cpp				\
				Shoe.cpp				\
				Statistics.cpp			\
				StrategyBasic.cpp		\
				StrategyCardCount.cpp	\
				StrategyInteractive.cpp	\
				StrategyLookup.cpp		\
				StrategyVegasDealer.cpp	\
				Table.cpp
OBJ=			$(SRC:.cpp=.o)

DEBUGFLAGS=		-g -DDEBUG
RELEASEFLAGS=	-O

CFLAGS=			-Wall -W -pedantic $(RELEASEFLAGS)
LFLAGS=			-Lutil
LIBS=			-lutil

DEPDIR=			.depend
DEP=			$(DEPDIR)/$(*F)

all: blackjack

# this uses the autodep strategy from Tom Tromey <tromey@cygnus.com>
# slightly difficult, but brilliant
%.o : %.cpp
	@mkdir -p $(DEPDIR)
	@gcc -M $(CFLAGS) -o $(DEP).P $< ; \
	 cp $(DEP).P $(DEP).d; \
	 sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	     -e '/^$$/ d' -e 's/$$/ :/' < $(DEP).P >> $(DEP).d; \
	 rm -f $(DEP).P
	g++ -c $< -o $@ $(CFLAGS)
	
-include $(SRC:%.cpp=$(DEPDIR)/%.d)

blackjack: $(OBJ)
	g++ -o $@ $(OBJ) $(LFLAGS) $(LIBS)

clean:
	@rm -rf $(DEPDIR)
	@rm -f $(OBJ) blackjack

