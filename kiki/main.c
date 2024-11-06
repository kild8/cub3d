# include "mlx.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>


# define WIDTH 1600
# define HEIGHT 1500
# define EVENT_CLOSE_BTN 17
# define KEY_LEFT  65361
# define KEY_RIGHT 65363
# define KEY_W 119
# define KEY_S 115
# define KEY_A 97
# define KEY_D 100
# define KEY_ESC 65307

# define PI 3.1415926575
# define P2 PI / 2
# define P3 3 * PI / 2
# define DEG 0.0174533

typedef struct s_cub3d
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*data;
	float	px;//player x position
	float	py;//player y position
	float	pdx;//delta x position
	float	pdy;//delta y position
	float	pa;//player angle

	int		r;
	int		mx;
	int		my;
	int		mp;
	int		dof;
	float	rx;
	float	ry;
	float	ra;
	float	xo;
	float	yo;

	int		C;//ceiling color
	int		F;//floor color
}	t_cub3d;

////////////UTILS

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
	write(fd, "\n", 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}




///////////EXIT

void	clean_exit(int exit_code, t_cub3d *c)
{
	if (!c)
		exit(exit_code);
	if (c->img)
		mlx_destroy_image(c->mlx, c->img);
	if (c->win && c->mlx)
		mlx_destroy_window(c->mlx, c->win);
	exit(exit_code);
}

int	esc_event(int keycode, t_cub3d *c)
{
	if (keycode == KEY_ESC)
		clean_exit(0, c);
	return (0);
}


int	msg_display(char *str1, char *str2, int errno)
{
	ft_putstr_fd("cub3d: ", 2);
	ft_putstr_fd(str1, 2);
	ft_putendl_fd(str2, 2);
	return (errno);
}

int	end_cub3d(t_cub3d *c)
{
	clean_exit(0, c);
	return (0);
}


////////////INIT


void	clean_init(t_cub3d *c)
{
	c->mlx = NULL;
	c->win = NULL;
	c->img = NULL;
	c->data = NULL;
	c->px = 0;
	c->py = 0;
	c->pdx = 0;
	c->pdy = 0;
	c->pa = 0;
	c->C = 0;
	c->F = 0;
	c->r = 0;
	c->mx = 0;
	c->my = 0;
	c->mp = 0;
	c->dof = 0;
	c->rx = 0;
	c->ry = 0;
	c->ra = 0;
	c->xo = 0;
	c->yo = 0;
}

void	init_img(t_cub3d *c)
{
	int		pixel_bits;
	int		line_bytes;
	int		endian;
	char	*data;

	c->img = mlx_new_image(c->mlx, WIDTH, HEIGHT);
	if (!(c->img))
		clean_exit(msg_display("error creating the image.", "", 1), c);
	data = mlx_get_data_addr(c->img, &pixel_bits, &line_bytes, &endian);
	c->data = data;
}

void	init(t_cub3d *c)
{
	c->mlx = mlx_init();
	if (!c->mlx)
		clean_exit(msg_display("MLX: error connecting to mlx.", "", 1), c);
	c->win = mlx_new_window(c->mlx, WIDTH, HEIGHT, "fractol");
	if (!c->win)
		clean_exit(msg_display("MLX : error creating window", "", 1), c);
	init_img(c);
	c->px = 300;
	c->py = 300;
	c->pdx = cos(c->pa) * 5;
	c->pdy = sin(c->pa) * 5;
	c->C = 0xFF0000;
	c->F = 0x0000FF;
}


/*int	mapx = 33;
int	mapy = 14;
int	mapS = 462;
int map[]=
{
0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1
0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1
0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1
1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1
1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,0,1,0,0,1,0,0,0,0
1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,0,1,0,0,1,0,0,0
1,1,0,0,0,0,0,1,1,0,1,0,1,0,1,1,1,0,0,0,0,1,0,0,1,0,0,0,0
1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0
1,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0
1,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,1,1,1,0,1,1,1,0,0,1,1,1,0
1,1,1,1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,0,1,0,0,1,0,0,0
1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0
};*/
int	mapx = 8;
int	mapy = 8;
int mapS = 64;
int map[]=
{
	1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,
};

///////////////RAYCAST


float	dist(float ax, float ay, float bx, float by)
{
	return (sqrt ((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void	draw_line(t_cub3d *c, int x, float lineH, int color)
{
	int	y;

	(void)lineH;
	y = 0;
	printf("%d", y);

	while (y < 20)
	{
		mlx_pixel_put(c->mlx, c->win, x, y, color);
		y++;
	}
}


void	raycaster(t_cub3d *c)
{
	float	aTan;
	float	disH;
	float	hx;
	float	hy;
	float 	nTan;
	float	disV;
	float	vx;
	float	vy;
	float	disT;
	float	lineH;

	c->ra = c->pa - DEG * 30;
	if (c->ra < 0)
		c->ra += 2 * PI;
	if (c->ra > 2 * PI)
		c->ra -= 2 * PI;
	while (c->r < 60)
	{
		c->dof = 0;
		disH = 1000000;
		hx = c->px;
		hy = c->py;
		aTan = -1/tan(c->ra);
		if (c->ra > PI)
		{
			c->ry = (((int)c->py / 64) * 64) - 0.0001;
			c->rx = (c->py - c->ry) * aTan + c->px;
			c->yo = - 64;
			c->xo = -c->yo * aTan;
		}
		if (c->ra < PI)
		{
			c->ry = (((int)c->py / 64) * 64) + 64;
			c->rx = (c->py - c->ry) * aTan + c->px;
			c->yo = 64;
			c->xo = -c->yo * aTan;
		}
		if (c->ra == 0 || c->ra == PI)
		{
			c->rx = c->px;
			c->ry = c->py;
			c->dof = 8;
		}
		while (c->dof < 8)
		{
			c->mx = (int)(c->rx) / 64;
			c->my = (int)(c->ry) / 64;
			c->mp = c->my * mapx + c->mx;
			if (c->mp > 0 && c->mp < mapx * mapy && map[c->mp] == 1)
			{
				hx = c->rx;
				hy = c->ry;
				disH = dist(c->px, c->py, hx, hy);
				c->dof = 8;
			}
			else
			{
				c->rx += c->xo;
				c->ry += c->yo;
				c->dof += 1;
			}

			
		}	
		c->dof = 0;
		disV = 1000000;
		vx = c->px;
		vy = c->py;
		nTan = -tan(c->ra);
		if (c->ra > P2 && c->ra < P3)
		{
			c->rx = (((int)c->px / 64) * 64) - 0.0001;
			c->ry = (c->px - c->rx) * nTan + c->py;
			c->xo -= -64;
			c->yo = - c->xo * nTan;
		}
		if (c->ra < P2 || c->ra > P3)
		{
			c->rx = (((int)c->px / 64) * 64) + 64;
			c->ry = (c->px - c->rx) * nTan + c->py;
			c->xo = 64;
			c->yo = -c->xo * nTan;
		}
		if (c->ra == 0 || c->ra == PI)
		{
			c->rx = c->px;
			c->ry = c->py;
			c->dof = 8;
		}
		while (c->dof < 8)
		{
			c->mx = (int)(c->rx) / 64;
			c->my = (int)(c->ry) / 64;
			c->mp = c->my * mapx + c->mx;
			if (c->mp > 0 && c->mp < mapx * mapy && map[c->mp] == 1)
			{
				vx = c->rx;
				vy = c->ry;
				disV = dist(c->px, c->py, vx, vy);
				c->dof = 8;
			}
			else
			{
				c->rx += c->xo;
				c->ry += c->yo;
				c->dof += 1;
			}

		}
		if (disV < disH)
		{
			c->rx = vx;
			c->ry = vy;
			disT = disV;
		}
		if (disH < disV)
		{
			c->rx = hx;
			c->ry = hy;
			disT = disH;
		}
		lineH = (mapS * 320) / disT;
		if (lineH > 320)
			lineH = 320;
		draw_line();
		c->ra += DEG;
		if (c->ra < 0)
			c->ra += 2 * PI;
		if (c->ra > 2 * PI)
			c->ra -= 2 * PI;
		c->r++;
	}
}

//////////RENDER


void	render(t_cub3d *c)
{
	int color = c->C; // Blanc
	int color2 = c->F;
	int total_pixels = WIDTH * HEIGHT;
	int i;

	// Remplir chaque pixel de l'image avec du blanc
	for (i = 0; i < total_pixels / 2; i++)
	{
		((int *)c->data)[i] = color;
	}
	for (i = total_pixels / 2; i < total_pixels; i++)
	{
		((int *)c->data)[i] = color2;
	}
	mlx_put_image_to_window(c->mlx, c->win, c->img, 0, 0);
	raycaster(c);
}



//////////KEYHOOK



int	key_event(int keycode, t_cub3d *c)
{
	if (keycode == KEY_ESC)
	{
		end_cub3d(c);
		return (0);
	}
	else if (keycode == KEY_W)
	{
		c->px += c->pdx;
		c->py += c->pdy;
		printf("ceci est px %f\n", c->px);
		printf("ceci est py %f\n", c->py);
	}
	else if (keycode == KEY_S)
	{
		c->px -= c->pdx;
		c->py -= c->pdy;
		printf("ceci est px %f\n", c->px);
		printf("ceci est py %f\n", c->py);
	}
	else if (keycode == KEY_A)
	{
		c->px -= c->pdx;
		c->py += c->pdy;
		printf("ceci est px %f\n", c->px);
		printf("ceci est py %f\n", c->py);
	}
	else if (keycode == KEY_D)
	{
		c->px += c->pdx;
		c->py -= c->pdy;
		printf("ceci est px %f\n", c->px);
		printf("ceci est py %f\n", c->py);
	}
	else if (keycode == KEY_LEFT)
	{
		c->pa -= 0.1;
		if (c->pa < 0)
			c->pa += 2 * PI;
		c->pdx = cos(c->pa) * 5;
		c->pdy = sin(c->pa) * 5;
		printf("ceci est pdx %f\n", c->pdx);
		printf("ceci est pdy %f\n", c->pdy);
	}
	else if (keycode == KEY_RIGHT)
	{
		c->pa += 0.1;
		if (c->pa > 2 * PI)
			c->pa -= 2 * PI;
		c->pdx = cos(c->pa) * 5;
		c->pdy = sin(c->pa) * 5;
		printf("ceci est pdx %f\n", c->pdx);
		printf("ceci est pdy %f\n", c->pdy);
	}
	else
		return (0);
	render(c);
	return (0);
}


int	main(void)
{
	t_cub3d c;

	write(1, "initialize\n", 11);
	clean_init(&c);
	init(&c);
	render(&c);
	mlx_hook(c.win, EVENT_CLOSE_BTN, 0, end_cub3d, &c);
	mlx_key_hook(c.win, key_event, &c);
	mlx_loop(c.mlx);
	write(1, "salut\n", 6);
	return (0);
}