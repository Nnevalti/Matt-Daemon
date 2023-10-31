NAME = Matt_daemon

SRCS =	main.cpp				\
		lock.cpp				\
		$(_CLASS)				\

_CLASS =	tintin_reporter.cpp	\


OBJS = $(SRCS:%.cpp=$(OBJS_DIR)/%.o)
DEPENDENCIES = $(OBJS:%.o=%.d)

SRCS_DIR = sources
LIBS_DIR = libs
OBJS_DIR = .objs
INCLUDES_DIR = includes $(LIBS:%=lib%/includes) $(LIBS:%=lib%)

LIBS =

MAKE = make
CC = g++
RM = rm -f
MKDIR = mkdir -p
DEBUG = off

CFLAGS = -MMD -Wall -Wextra -Werror
CXXFLAGS = $(INCLUDES_DIR:%=-I %)
ifeq ($(DEBUG), on)
	CXXFLAGS += -g3
endif
LDFLAGS = $(LIBS:%=-L lib%) $(LIBS:%=-l%)

vpath %.cpp	$(addprefix $(SRCS_DIR), /. /class)

all:
	$(foreach LIB, ${LIBS}, ${MAKE} -C lib${LIB} ;)
	$(MAKE) $(NAME)

scan:
	scan-build $(MAKE)

$(NAME): $(OBJS) | $(LIBS:%=lib%.a)
	$(CC) $(CXXFLAGS) $^ -o $(NAME) $(LDFLAGS)

-include $(DEPENDENCIES)
$(OBJS_DIR)/%.o: %.cpp $(OBJS_DIR)/debug$(DEBUG) | $(OBJS_DIR) 
	$(CC) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@ 

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
