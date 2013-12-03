PKG_CONFIG_PATH := /usr/local/lib/pkgconfig:$(PKG_CONFIG_PATH)
export PKG_CONFIG_PATH

NAME		=	exec `pkg-config --libs allegro-5.0 allegro_main-5.0`

CC		=	g++

RM		=	rm -f

CPPFLAGS	=	-std=c++0x -W -Wextra -Wall -Werror -O2

CPPFLAGS	+=	-I. -I/usr/include -I/usr/local/include -I./mac/
LDFLAGS		=	-L./mac/lib -L./vendors/ -lgphoto2.6 -lgphoto2_port -lallegro -lallegro_dialog -lallegro_primitives -lallegro_ttf -lallegro_font -lallegro_image

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
