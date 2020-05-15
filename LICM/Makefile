INC=-I/usr/local/include/
all: dominator_pass.so LICM.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) $(INC) -g -O0 -fPIC

dataflow.o: dataflow.cpp dataflow.h
dominance.o: dominance.cpp dominance.h

%.so: %.o dataflow.o dominance.o
	$(CXX) -dylib -shared $^ -o $@

clean:
	rm -f *.o *~ *.so

.PHONY: clean all
