SOURCES += src/AdminComponent.cc
SOURCES += src/Agreement.cc
SOURCES += src/ChessTourney.cc
SOURCES += src/ComponentBase.cc
SOURCES += src/DatabaseInterface.cc
SOURCES += src/DatabaseManager.cc
SOURCES += src/Game.cc
SOURCES += src/GameChess.cc
SOURCES += src/ServerCore.cc
SOURCES += src/GameRoom.cc
SOURCES += src/GlickoSystem.cc
SOURCES += src/HistoryProcess.cc
SOURCES += src/I18n.cc
SOURCES += src/libchess/Board.cc
SOURCES += src/libchess/ChessBasedGame.cc
SOURCES += src/libchess/Chess.cc
SOURCES += src/libchess/ChessMove.cc
SOURCES += src/libchess/History.cc
SOURCES += src/libchess/Piece.cc
SOURCES += src/libchess/State.cc
SOURCES += src/main.cc
SOURCES += src/MatchChess.cc
SOURCES += src/MatchDatabase.cc
SOURCES += src/MatchManager.cc
SOURCES += src/Pairing/TourneyGame.cc
SOURCES += src/Pairing/Player.cc
SOURCES += src/Pairing/Tourney.cc
SOURCES += src/Pairing/TourneyPlayers.cc
SOURCES += src/ProfileManager.cc
SOURCES += src/Threads/Dispatcher.cc
SOURCES += src/Threads/Pool.cc
SOURCES += src/Threads/Task.cc
SOURCES += src/Util/Identifier.cc
SOURCES += src/Util/IDSet.cc
SOURCES += src/Util/Log.cc
SOURCES += src/Util/Timer.cc
SOURCES += src/XML/iksutil.cc
SOURCES += src/XML/Xml.cc
SOURCES += src/XMPP/Component.cc
SOURCES += src/XMPP/Disco.cc
SOURCES += src/XMPP/Jid.cc
SOURCES += src/XMPP/Muc.cc
SOURCES += src/XMPP/Node.cc
SOURCES += src/XMPP/PartialJid.cc
SOURCES += src/XMPP/RootNode.cc
SOURCES += src/XMPP/Roster.cc
SOURCES += src/XMPP/StanzaBase.cc
SOURCES += src/XMPP/Stanza.cc
SOURCES += src/XMPP/Stream.cc
SOURCES += src/MatchFactory.cc
SOURCES += src/Util/Date.cc
SOURCES += src/Threads/Thread.cc
SOURCES += src/TourneyManager.cc
SOURCES += src/ServerModule.cc
SOURCES += src/ChessMatchAnnouncement.cc
SOURCES += src/MatchAnnouncementFactory.cc
SOURCES += src/AnnouncementManager.cc


SRCDIR = src
OBJDIR = obj
DEPSDIR = .deps
CXXFLAGS+=-Wall -ggdb3
#CXXFLAGS+=-Wall -O2 -pg -march=native
#CXXFLAGS+=-Wall -O3 -fomit-frame-pointer
CXXFLAGS+=-I${SRCDIR} `pkg-config --cflags iksemel` `pkg-config --cflags libpqxx`
LDLIBS+=-lrt -lpthread `pkg-config --libs iksemel` `pkg-config --libs libpqxx` -lboost_date_time
TARGET=chessd
CC=gcc
CXX=g++

OBJECTS = $(patsubst ${SRCDIR}/%.cc,${OBJDIR}/%.o,${SOURCES})
DEPS = $(patsubst ${SRCDIR}/%.cc,${DEPSDIR}/%.d,${SOURCES})

all: ${TARGET}
	@echo "done"

-include ${DEPS}

${TARGET}: ${OBJECTS}
	@echo "LD $@..."
	@${CXX} -o ${TARGET} ${OBJECTS} ${CXXFLAGS} ${LDLIBS}

.deps/%.d:
	@mkdir -p $(dir $@)
	@touch $@

${OBJDIR}/%.o: ${SRCDIR}/%.cc
	@mkdir -p $(dir $@)
	@echo "CC $<..."
	@${CXX} ${CXXFLAGS} -o $@ -c $< -MMD -MP -MF $(patsubst ${OBJDIR}/%.o,${DEPSDIR}/%.d,$@)

clean: clean-target clean-obj

clean-all: clean-target clean-obj clean-deps

clean-target:
	@echo "Cleaning executable..."
	@rm -f ${TARGET}

clean-obj:
	@echo "Cleaning objects..."
	@rm -f ${OBJECTS}

clean-deps:
	@echo "Cleaning dependencies..."
	@rm -rf ${DEPSDIR}
