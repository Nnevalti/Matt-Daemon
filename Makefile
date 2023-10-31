NAME = Matt_daemon

SRCS =	main.cpp				\
		lock.cpp				\
		daemonize.cpp			\
		cleanup.cpp				\
		$(_CLASS)				\
		$(_UTILS)				\

_CLASS =	tintin_reporter.cpp	\
			socket.cpp			\

_UTILS =	signals.cpp			\


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
LDFLAGS = $(LIBS:%=-L lib%) $(LIBS:%=-l%)

vpath %.cpp	$(addprefix $(SRCS_DIR), /. /class /utils)

all:
	$(foreach LIB, ${LIBS}, ${MAKE} -C lib${LIB} ;)
	$(MAKE) $(NAME)

scan:
	scan-build $(MAKE)

$(NAME): $(OBJS) | $(LIBS:%=lib%.a)
	$(CXX) $(CXXFLAGS) $^ -o $(NAME) $(LDFLAGS)

-include $(DEPENDENCIES)
$(OBJS_DIR)/%.o: %.cpp $(OBJS_DIR)/debug$(DEBUG) | $(OBJS_DIR) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@ 

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

fclean: clean
	$(RM) $(NAME) $(foreach LIB, $(LIBS), lib$(LIB)/lib$(LIB).a)

re: fclean all

.PHONY: all clean fclean re scan
