# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/30 20:09:24 by rpehkone          #+#    #+#              #
#    Updated: 2021/08/19 20:41:42 by rpehkone         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = wolf3d
LIB = libft/libft.a
FOLDERS = libft mlx_io src minilibx src/live_texture src/live_texture/fractal src/live_texture/fdf
MLX_IO_FILES = init_window.c input_state.c image.c
FRACTAL = fractal.c init_fractal.c julia.c
FDF = colors.c fdf.c height_color.c init_fdf.c line.c printer.c read_map.c set_map.c
LIVE_TEXTURE_FILES = live_texture.c frame_in_frame.c
LIVE_TEXTURE_FILES += $(addprefix fractal/, $(FRACTAL))
LIVE_TEXTURE_FILES += $(addprefix fdf/, $(FDF))
FILES = main.c wolf.c render.c
FILES += $(addprefix live_texture/, $(LIVE_TEXTURE_FILES))
FLAGS = -Wall -Wextra -Werror -O3
.PHONY: clean fclean re all

all: $(NAME)

$(NAME):
	@make -C libft
	gcc $(FLAGS) $(addprefix mlx_io/, $(MLX_IO_FILES)) $(addprefix src/, $(FILES)) $(LIB) $(addprefix -I , $(FOLDERS)) -L minilibx -lmlx -framework OpenGL -framework Appkit -o $(NAME)

clean:
	@make clean -C libft

fclean: clean
	@make fclean -C libft
	rm -f $(NAME)

re: fclean
	make all
