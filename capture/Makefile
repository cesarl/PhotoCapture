NAME		=	exec

CC		=	g++-4.8

RM		=	rm -f

CPPFLAGS	=	-std=c++0x -W -Wextra -Wall -Werror

CPPFLAGS	+=	-I.
LDFLAGS		=	-L/usr/lib -L./vendors/ -lgphoto2 -lallegro -lallegro_dialog -lallegro_primitives -lallegro_ttf -lallegro_font -lallegro_image -lGLU

INC		=	-I/usr/include/allegro5

SRCS		=	main.cpp \
			./vendors/cJSON.cpp

OBJS		=	$(SRCS:.cpp=.o)

ECHO		=	echo -e

all		:	$(NAME)

$(NAME)		:	$(OBJS)
			@$(CC) $(OBJS) -o $(NAME) $(INC) $(LDFLAGS)
			@$(ECHO) '\033[1;37;44m> Compiled {\033[4m$(NAME)}\033[0m'

clean		:
			@$(RM) $(OBJS)
			@$(ECHO) '\033[1;37;41m> Directory cleaned\033[0m'

fclean		:	clean
			@$(RM) $(NAME)
			@$(ECHO) '\033[1;37;41m> Remove executable\033[0m'

re		:	fclean all

.PHONY		:	all clean re
