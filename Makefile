#for dependency issues libPAM.a need to be statically linked BEFORE libm.so is dynamically linked
PAM_INCLUDE=-I./include
PAM_LIBS=-L./bin
PAM_LDFLAGS=$(PAM_LIBS) -lPAM

TMW_ROOT=/opt/Matlab
TMW_LIBS=-Wl,-rpath-link,$(TMW_ROOT)/bin/glnxa64 -L$(TMW_ROOT)/bin/glnxa64 -lmx -lmex -lmat -lm
TMW_LDFLAGS=$(PAM_LDFLAGS) -Lbin -lPAM -pthread -shared -Wl,--version-script,$(TMW_ROOT)/extern/lib/glnxa64/mexFunction.map -Wl,--no-undefined $(TMW_LIBS) 

#-------------------------------------------------------------------------------

CC=gcc
AR=ar

TMW_INCLUDE=-I$(TMW_ROOT)/extern/include -I$(TMW_ROOT)/simulink/include $(PAM_INCLUDE)
TMW_CFLAGS=-c -DMATLAB_MEX_FILE -D_GNU_SOURCE  -fexceptions -fPIC -fno-omit-frame-pointer -pthread  -DMX_COMPAT_32 -O -DNDEBUG $(TMW_INCLUDE)

all: pamCLI pam.mexa64

pamCLI: bin/libPAM.a build/pamCLI.o
	$(CC) build/pamCLI.o -o bin/pamCLI $(PAM_LDFLAGS) -lm

pam.mexa64: build/mexFunction.o bin/libPAM.a
	mkdir -p bin/Matlab
	$(CC) -o bin/Matlab/pam.mexa64 build/mexFunction.o $(TMW_LDFLAGS)

bin/libPAM.a: build/pam.o build/internals.o build/correlation.o build/dataIO.o
	mkdir -p bin
	$(AR) rcs bin/libPAM.a $^

build/mexFunction.o: src/wrappers/Matlab/mexFunction.c	
	$(CC) $(TMW_CFLAGS) $^ -o $@

build/pamCLI.o: src/pamCLI.c
	$(CC) -c $^ -o $@ $(PAM_INCLUDE)

build/%.o: src/%.c
	$(CC) -c $^ -o $@ $(PAM_INCLUDE)

clean:
	rm -rf bin/* build/*
