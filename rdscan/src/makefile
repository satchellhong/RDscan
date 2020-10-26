
.SUFFIXS 	: .cc .o
CC              = g++ 
LIB             = -L../lib/ -lAlgLib -lbiofiles -lutil -pthread -L/usr/local/lib/ -lz 
CFLAGS          = -O2 -g -Wall -std=c++0x 

SRCS		= main.cc	preprocess.cc	calcDist.cc	report.cc	util.cc	
OBJECTS     = main.o	preprocess.o	calcDist.o	report.o	util.o
TARGET 		= rd_scan_v1.0.1 



all: $(TARGET)

$(TARGET) : $(OBJECTS)
		$(CC) -o ../bin/$(TARGET) $(CFLAGS) $(OBJECTS) $(LIB)


dep :
		$(CC) -M $(SRCS) > .depend

clean:
	rm -f $(OBJECTS) .depend
cleanall: clean
	rm -f $(OBJECTS) ../bin/$(TARGET) .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif



