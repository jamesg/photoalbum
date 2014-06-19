first:	all

CPPFLAGS= \
		  --std=c++0x \
		  -O3 \
		  -Isrc/ \
		  -ggdb \
		  `pkg-config --cflags ImageMagick++` \
		  -Icommandline/include/ \
		  -Ijson/include/ \
		  -Isqlite/include/
LIBS=\
	 sqlite/libsqlite.a \
	 json/libjson.a \
	 -lboost_date_time \
	 -lboost_filesystem \
	 -lboost_program_options \
	 -lboost_random \
	 -lboost_regex \
	 -lboost_system \
	 -lboost_thread \
	 -lpthread \
	 -lsqlite3 \
	 `pkg-config --libs ImageMagick++` \
	 -lexiv2

OBJDIR=objs

OBJS=\
	${OBJDIR}/api/album.o \
	${OBJDIR}/api/auth.o \
	${OBJDIR}/api/features.o \
	${OBJDIR}/api/jpeg_data.o \
	${OBJDIR}/api/api.o \
	${OBJDIR}/api/map.o \
	${OBJDIR}/api/note.o \
	${OBJDIR}/api/photograph.o \
	${OBJDIR}/api/statistics.o \
	${OBJDIR}/base64/base64.o \
	${OBJDIR}/db/album.o \
	${OBJDIR}/db/auth.o \
	${OBJDIR}/db/get_database.o \
	${OBJDIR}/db/jpeg_data.o \
	${OBJDIR}/db/map.o \
	${OBJDIR}/db/note.o \
	${OBJDIR}/db/photograph.o \
	${OBJDIR}/db/statistics.o \
	${OBJDIR}/jsonrpc/auth.o \
	${OBJDIR}/jsonrpc/server.o \
	${OBJDIR}/mongoose.o \
	${OBJDIR}/server.o \
	${OBJDIR}/uri/http_date.o \
	${OBJDIR}/uri/insert_photograph.o \
	${OBJDIR}/uri/jpeg_image.o \
	${OBJDIR}/uri/jpeg_image_fullsize.o \
	${OBJDIR}/uri/map_tile_km.o \
	${OBJDIR}/util/scale.o

clean:
	rm -f ${OBJS} objs/main.o objs/os/gazetteer.o objs/os/streetmap.o

all:	bin/main

json/libjson.a:
	cd json && make libjson.a

sqlite/libsqlite.a:
	# Build libsqlite with our copy of the JSON library.
	cd sqlite && make JSON_LIBRARY=../json libsqlite.a

bin/main:	${OBJS} ${OBJDIR}/main.o json/libjson.a sqlite/libsqlite.a
	${CXX} ${OBJS} ${OBJDIR}/main.o ${LIBS} -o $@

bin/exports: ${OBJS} ${OBJDIR}/exports/exports.o json/libjson.a sqlite/libsqlite.a
	${CXX} ${OBJS} ${OBJDIR}/exports/exports.o ${LIBS} -o $@

bin/gazetteer: ${OBJS} ${OBJDIR}/os/gazetteer.o json/libjson.a sqlite/libsqlite.a
	${CXX} ${OBJS} ${OBJDIR}/os/gazetteer.o ${LIBS} -o $@

bin/streetmap: ${OBJS} ${OBJDIR}/os/streetmap.o json/libjson.a sqlite/libsqlite.a
	${CXX} ${OBJS} ${OBJDIR}/os/streetmap.o ${LIBS} -o $@

${OBJDIR}/%.o : src/%.cpp
	${CXX} ${CPPFLAGS} $< -c -o $@

${OBJDIR}/mongoose.o: src/mongoose.c
	${CC} ${CFLAGS} -c -o ${OBJDIR}/mongoose.o src/mongoose.c -lpthread

