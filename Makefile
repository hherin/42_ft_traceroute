NAME=	ft_traceroute

SRC=	./src/main.c \
		./src/options.c \
		./src/send.c \
		./src/recv.c \
		./src/utils.c
		

OBJ=	${SRC:.c=.o}

HDRS=	./inc/traceroute.h

CFLAG=	-g3 -fsanitize=address -Wall -Werror -Wextra

all: ${NAME}

${OBJ} : ${HDRS}

%.o:%.c
	gcc ${CFLAG} -o $@ -c $<

${NAME}:	${OBJ}
			make -C lib
			gcc ${CFLAG} -o ${NAME} ${OBJ} lib/libft.a

clean:
			@make clean -C lib
			@rm -rf src/*.o

fclean:		clean
			@make fclean -C lib
			@rm -rf ${NAME}

re:			fclean all

.PHONY: all clean fclean re