#***************************************************************
#                                                              
# makefile fuer frog
#
#                                                              *
#***************************************************************

#***************
# Programmname *
#***************
PROGRAM=frog
#DEBUG=-g                                       
DEBUG=

LDFLAGS =  -Wall  
CPPFLAGS=  -std=c++0x -pthread \



	
$(PROGRAM) : version main.o frog.o  
			 c++  -o $(PROGRAM) main.o frog.o   $(DEBUG) 

# server
main.o : main.cpp frog.o
	c++ -c main.cpp   -o main.o  $(DEBUG)    $(LDFLAGS)  $(CPPFLAGS)

# frog(lib)
frog.o : frog.cpp
	c++ -c frog.cpp   -o frog.o  $(DEBUG) $(LDFLAGS)  $(CPPFLAGS)

  
version: 
	@echo "-------------------------------------------------"
	@echo "Frog - simplified programm interpreter..."	 
	@echo "Try:"
	@echo "./frog hello.frog "	 
	@echo "or"
	@echo "./frog sort.frog "	 
	@echo "-------------------------------------------------"
	

clean: 
	rm -f *.o	
	rm -f frog
	 
	