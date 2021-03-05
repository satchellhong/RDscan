CC			=	gcc
CXX			=	g++
CFLAGS		=	-g -Wall -O2
DFLAGS		=	-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_CURSES_LIB=1 -DBGZF_CACHE
LIBS		=	-lcurses
BAMDIR		=	./bam/
SYNDIR		=	./synteka/
UTILDIR		=	./util/
OBJDIR		=	./objs/
BAMFILE		=	bam_file.
BAMFILES	=	bgzf. bam. kstring. bam_import. bam_aux. sam_header. bam_index.
SYNFILES	=	key_dp_file. key_file. dp_file. list_file. fa_file. iupac_file.
UTILFILES	=	suffix_array. util.
TARGET_BIO	=	../lib/libbiofiles.a 
TARGET_UTIL	=	../lib/libutil.a

all: $(OBJDIR) $(BAMFILE) $(BAMFILES) $(SYNFILES) $(UTILFILES) MAKELIB

$(OBJDIR):
		mkdir -p $(OBJDIR)

$(BAMFILE):
		$(CXX) $(CFLAGS) -std=c++0x -c $(BAMDIR)$@cc -o $(OBJDIR)$@o

$(BAMFILES):
		$(CC) $(CFLAGS) $(DFLAGS) -c $(BAMDIR)$@c -o $(OBJDIR)$@o

$(SYNFILES):
		$(CXX) $(CFLAGS) -std=c++0x -c $(SYNDIR)$@cc -o $(OBJDIR)$@o

$(UTILFILES):
		$(CXX) $(CFLAGS) -std=c++0x -c $(UTILDIR)$@cc -o $(UTILDIR)$@o

MAKELIB:
		ar -csruU $(TARGET_BIO) $(shell find objs -type f \( -name *.o -not -name *-checkpoint.o \))
		ar -csruU $(TARGET_UTIL) $(shell find util -type f \( -name *.o -not -name *-checkpoint.o \))

clean:
	rm -rf $(OBJDIR)
	rm -rf $(TARGET_BIO)
	rm -rf $(TARGET_UTIL)
	find ./ -name ".ipynb_checkpoints" -exec rm -rf {} \;
