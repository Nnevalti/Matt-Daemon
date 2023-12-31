NAME = Matt_daemon

SRCS =		main.cpp			\
			$(_CLASS)			\
			$(_UTILS)			\
			$(_SETUP)			\
			$(_SERVER)			\

_CLASS =	tintin_reporter.cpp	\
			socket.cpp			\
			epoll.cpp			\
			ssl.cpp				\

_UTILS =	signals.cpp			\
			lock.cpp			\
			daemonize.cpp		\
			check_root.cpp		\

_SETUP =	cleanup.cpp			\
			init_server.cpp		\

_SERVER =	run_server.cpp		\
			read_client.cpp		\
			broadcast.cpp		\
			accept_client.cpp	\
			remove_client.cpp	\
			do_command.cpp		\


OBJS = $(SRCS:%.cpp=$(OBJS_DIR)/%.o)
DEPENDENCIES = $(OBJS:%.o=%.d)

SRCS_DIR = sources
LIBS_DIR = libs
OBJS_DIR = .objs
INCLUDES_DIR = includes $(LIBS:%=lib%/includes) $(LIBS:%=lib%)

LIBS =

MAKE = make
CXX = c++
RM = rm -f
MKDIR = mkdir -p
DEBUG = off

CXXFLAGS = $(INCLUDES_DIR:%=-I %) -MMD -Wall -Wextra -Werror -std=c++17
ifeq ($(DEBUG), on)
	CXXFLAGS += -g3
endif
LDFLAGS = $(LIBS:%=-L lib%) $(LIBS:%=-l%) -lssl -lcrypto

vpath %.cpp	$(addprefix $(SRCS_DIR), /. /class /utils /setup /server)

all:
	$(foreach LIB, ${LIBS}, ${MAKE} -C lib${LIB} ;)
	$(MAKE) $(NAME)
	openssl req -x509 -sha256 -days 356 -nodes -newkey rsa:2048 -subj "/CN=/C=FR/L=Paris" -keyout /etc/ssl/private/matt-daemon.key -out /etc/ssl/certs/matt-daemon.crt


scan:
	scan-build $(MAKE)

$(NAME): $(OBJS) | $(LIBS:%=lib%.a)
	$(CXX) $(CXXFLAGS) $^ -o $(NAME) $(LDFLAGS)

-include $(DEPENDENCIES)
$(OBJS_DIR)/%.o: %.cpp $(OBJS_DIR)/debug$(DEBUG) | $(OBJS_DIR) 
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(OBJS_DIR):
	$(MKDIR) $(OBJS_DIR)

lib%.a:
	$(MAKE) -C $(@:%.a=%)

$(OBJS_DIR)/debug$(DEBUG): | $(OBJS_DIR)
	$(RM) $(OBJS_DIR)/debug*
	touch $@

clean:
	$(foreach LIB, $(LIBS), $(MAKE) $@ -C lib$(LIB);)
	$(RM) -r $(OBJS_DIR)
	$(RM) matt-daemon.key matt-daemon.crt

fclean: clean
	$(RM) $(NAME) $(foreach LIB, $(LIBS), lib$(LIB)/lib$(LIB).a)

re: fclean all

.PHONY: all clean fclean re scan
