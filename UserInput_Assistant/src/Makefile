INC_DIR:= ./
SRC_DIR:= ./src/./
SRCS:=$(wildcard *.cc)
OBJS:= $(patsubst %.cc,%.o, $(SRCS))
LIBS:= -lpthread -ljson

CXX:=g++

CXXFLAGS:= -w -g  $(addprefix -I, $(INC_DIR)) $(LIBS)

EXE:=./server.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)

