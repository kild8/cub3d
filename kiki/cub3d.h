#ifndef CUB3D_H
#define CUB3D_H

#include "../mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

# define DEBUG 0
# define WIDTH 1280
# define HEIGHT 720
# define BLOCK 64
# define EVENT_CLOSE_BTN 17
# define KEY_UP 65362
# define KEY_DOWN 65364
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_W 119
# define KEY_S 115
# define KEY_A 97
# define KEY_D 100
# define KEY_ESC 65307
# define KEY_Q 113
# define KEY_E 101
# define PI 3.141592653589

typedef struct s_player
{
	float	x;
	float	y;
	float	angle;

	bool	key_left;
	bool	key_right;
	bool	key_up;
	bool	key_down;
	bool	left_rotate;
	bool	right_rotate;

} t_player;

typedef struct s_texture
{
	void	*img;
	int		*data;
	int		width;
	int		height;
	int		bpp;
	int		size_line;
	int		endian;
} t_texture;

typedef struct s_game
{
	void	*mlx;
	void	*win;
	void	*img;

	char	*data;
	int		bpp;
	int		size_line;
	int		endian;

	char**	map;

	t_player	player;
	t_texture	north_texture;
	t_texture	south_texture;
	t_texture	west_texture;
	t_texture	east_texture;


} t_game;

void	init_player(t_player *player);
int		key_press(int key, t_player *player);
int		key_release(int key, t_player *player);
void	move_player(t_player *player);
void	load_texture(t_game *game, t_texture *texture, char *file_path);

#endif