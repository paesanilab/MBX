CXX = icpc
CXXFLAGS = -std=c++11 -lmbnrglib -fopenmp -lfftw3

LIBS = -L../../install/lib/static/
INCLUDE = -I../../install/include/

all: ttm-nrg_mbx 

ttm-nrg_mbx: ttm-nrg_mbx.cpp
	$(CXX) ttm-nrg_mbx.cpp $(CXXFLAGS) $(INCLUDE) $(LIBS) -o ttm-nrg_mbx

clean: 
	rm -f ttm-nrg_mbx

.PHONY: all clean
