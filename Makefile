# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/15 15:12:01 by tkempf-e          #+#    #+#              #
#    Updated: 2022/10/15 15:29:50 by tkempf-e         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	=	main.c actions.c utils.c

OBJS	=	$(SRCS:.c=.o)

GCC 	=	gcc

RM		=	rm -f

FLAGS	=	-pthread

NAME	=	philo

.c.o:
	$(GCC) $(FLAGS) -c -I. $< -o ${<:.c=.o}

$(NAME):	$(OBJS)
					@$(GCC) $(FLAGS) $(OBJS) -pthread -o $(NAME)

all:		$(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME) philo

re:			fclean all

.PHONY:		all clean fclean re