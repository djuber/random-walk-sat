# CS408 Project 1
# Daniel Uber
headers = CNF.h
objects = CNF.o readfile.o
winobjects = CNF.obj readfile.obj
# the native unix compiler
CXX = g++
CXXFLAGS = -Wall -Werror -g
# the windows cross compiler 
CXXWIN = i586-mingw32msvc-g++ 
LIBS = -lm
# dummy target
all: unix windows

unix : walksat
windows: walksat.exe
# Unix Targets
# the cnf executable
walksat: $(objects)
	$(CXX) $(CXXFLAGS) -o walksat $(objects) $(LIBS)

# remove temp files and objects
clean: clean-objects
	rm -f  *~ walksat walksat.exe;

clean-objects:
	rm -f *.o *.obj;

# test: all
# 	sh test.sh

# Windows Targets
# use distinct object extension for PE objects
# add explicit rules since make only knows how to build .o
CNF.obj: $(headers) CNF.cc
	$(CXXWIN) $(CXXFLAGS) -o CNF.obj -c CNF.cc

readfile.obj: $(headers) readfile.cc
	$(CXXWIN) $(CXXFLAGS) -o readfile.obj -c readfile.cc

# the cnf executable for windows cnf.exe
walksat.exe: $(winobjects)
	$(CXXWIN) $(CXXFLAGS) -o walksat.exe $(winobjects) $(LIBS)


# windows-test: windows
# 	sh test-windows.sh	

# zipfile: 
# 	sh package.sh
