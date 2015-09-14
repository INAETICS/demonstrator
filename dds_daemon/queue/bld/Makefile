
IDL_SOURCE_DIR=../idl
TARGET=x86_64.linux
OPENSPLICE_DIR=../../opensplice/${TARGET}
OUTPUT_DIR=../exe
CFLAGS=-fPIC -O0 -g -DNDEBUG -D_REENTRANT -Wall -I../include -I${OPENSPLICE_DIR}/include -I${OPENSPLICE_DIR}/include/sys -I${OPENSPLICE_DIR}/include/dcps/C/SAC -I.

all: ${OUTPUT_DIR}/dds_daemon
	
${OUTPUT_DIR}/dds_daemon: .obj/dds_daemon.o libsac_sample_types.so
	@test -d ${OUTPUT_DIR} || mkdir ${OUTPUT_DIR}
	gcc -L. -L${OPENSPLICE_DIR}/lib -Wl,-rpath,./ -Wl,-rpath,${OPENSPLICE_DIR}/lib .obj/dds_daemon.o -lsac_sample_types -ldcpssac -lddskernel -ldl $(subst lib,-l,$(sort $(basename $(notdir $(wildcard /usr/lib/librt.so /lib/librt.so))))) -lpthread -o $@

.obj/dds_daemon.o: ../src/dds_daemon.c
	@test -d ".obj/" || mkdir ".obj/"
	gcc ${CFLAGS} -c -o $@ ../src/dds_daemon.c

libsac_sample_types.so: .obj/sampleSacDcps.o .obj/sampleSplDcps.o
	test -d ./ || mkdir ./
	gcc -L. -L${OPENSPLICE_DIR}/lib -Wl,-rpath,./ -Wl,-rpath,${OPENSPLICE_DIR}/lib -shared .obj/sampleSacDcps.o .obj/sampleSplDcps.o -ldcpssac -lddskernel -ldl $(subst lib,-l,$(sort $(basename $(notdir $(wildcard /usr/lib/librt.so /lib/librt.so))))) -lpthread -o $@
	
.obj/sampleSacDcps.o: ../src/sampleSacDcps.c
	@test -d ".obj/" || mkdir ".obj/"
	gcc ${CFLAGS} -c -o $@ ../src/sampleSacDcps.c

.obj/sampleSplDcps.o: ../src/sampleSplDcps.c
	@test -d ".obj/" || mkdir ".obj/"
	gcc ${CFLAGS} -c -o $@ ../src/sampleSplDcps.c

	
	

clean:
	rm -rf ./.obj
	rm -f libsac_sample_types.so
	rm -rf ${OUTPUT_DIR}
