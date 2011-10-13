CONFIG ?= makefile.config

-include ${CONFIG}

SOURCES += src/hash.c
SOURCES += src/http.c
SOURCES += src/http_server.c
SOURCES += src/jabber_bind.c
SOURCES += src/log.c
SOURCES += src/main.c
SOURCES += src/socket_monitor.c
SOURCES += src/time.c
SOURCES += src/list.c
SOURCES += src/socket.c

SRCDIR = src
OBJDIR = obj
DEPSDIR = .deps
CFLAGS += -Wall -D_GNU_SOURCE $(shell pkg-config iksemel --cflags)
CXXFLAGS += ${CFLAGS}
LDLIBS += -I${HOME}/.usr/lib -lrt $(shell pkg-config iksemel --libs)
TARGET ?= bosh

CC ?= gcc
CXX ?= g++

OBJECTS = $(patsubst ${SRCDIR}/%.c,${OBJDIR}/%.o,${SOURCES})
DEPS = $(patsubst ${SRCDIR}/%.c,${DEPSDIR}/%.d,${SOURCES})

all: ${TARGET}
	@echo "done"

-include ${DEPS}

${TARGET}: ${OBJECTS}
	@echo "LD $@..."
	@${CC} -o ${TARGET} ${OBJECTS} ${CXXFLAGS} ${LDLIBS}

.deps/%.d:
	@mkdir -p $(dir $@)
	@touch $@

${OBJDIR}/%.o: ${SRCDIR}/%.c
	@mkdir -p $(dir $@)
	@echo "CC $<..."
	@${CC} ${CFLAGS} -o $@ -c $< -MMD -MP -MF $(patsubst ${OBJDIR}/%.o,${DEPSDIR}/%.d,$@)

clean: clean-target clean-obj

dist-clean: clean-target clean-obj clean-deps

clean-target:
	@echo "Cleaning executable..."
	@rm -f ${TARGET}

clean-obj:
	@echo "Cleaning objects..."
	@rm -f ${OBJECTS}

clean-deps:
	@echo "Cleaning dependencies..."
	@rm -rf ${DEPSDIR}
