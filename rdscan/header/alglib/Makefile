.SUFFIXES : .cpp .o
CC=			g++
CFLAGS=		-g -O2
LIBS=		
CFILE=		$(shell find ./ -type f \( -name "*.cpp" -not -name "*-checkpoint.cpp" \))
OBJS=		$(shell for file in `find ./ -type f \( -name "*.cpp" -not -name "*-checkpoint.cpp" \)`; do echo $$(basename $$file .cpp).o; done)
TARGET=		libAlgLib.a
TARGETDIR=	../../lib/

.cpp.o:
		$(CC) -c $(CFLAGS) $< -o $@
		
all : $(TARGETDIR) $(TARGET)

$(TARGETDIR):
		mkdir -p $(TARGETDIR)

$(TARGET) : $(OBJS)
		$(AR) -csruU $(TARGETDIR)$@ $(OBJS)

clean:
		rm -rf *.o
		rm -rf $(TARGETDIR)$(TARGET)
		find . -name ".ipynb_checkpoints" -exec rm -rf {} \;