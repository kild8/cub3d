#include "cub3d.h"


void put_pixel(int x, int y, int color, t_game *game)
{
	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	if (color < 0 || color > 0xFFFFFF)
	{
    	fprintf(stderr, "Error: Invalid color value: %d\n", color);
	}
	int index = y * game->size_line + x * game->bpp / 8;
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;
}

void	draw_square(int x, int y, int size, int color, t_game *game)
{
	int i;

	i = 0;
	while (i++ < size)
		put_pixel(x + i, y, color, game);
	i = 0;
	while (i++ < size)
		put_pixel(x, y + i, color, game);
	i = 0;
	while (i++ < size)
		put_pixel(x + size, y + i, color, game);
	i = 0;
	while (i++ < size)
		put_pixel(x + i, y + size, color, game);
}

void	draw_map(t_game *game)
{
	char **map;
	int color;
	int x;
	int y;
	
	map = game->map;
	color = 0x0000FF;
	x = 0;
	y = 0;
	while (map[y] != NULL)
	{
		x = 0;
		while (map[y][x] != 0)
		{
			if (map[y][x] == '1')
				draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
			x++;
		}
		y++;
	}
	
}

char **get_map(void)
{
	char **map = malloc(sizeof(char *) * 11);
	map[0] = "111111111111111";
	map[1] = "100000000000001";
	map[2] = "100000000000001";
	map[3] = "100000000000001";
	map[4] = "100000000000001";
	map[5] = "100100000000001";
	map[6] = "100000000000001";
	map[7] = "100001000100001";
	map[8] = "101100000110001";
	map[9] = "111111111111111";
	map[10] = NULL;
	return (map);
}

void	clear_image(t_game *game)
{
	int x;
	int y;
	int ceiling_color = (255 << 16 | 145 << 8 | 23);
	int floor_color = (23 << 16 | 123 << 8 | 2);
	//RGB ---> (R << 16 | G << 8 | B)

	x = 0;
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			if (y < HEIGHT / 2)
				put_pixel(x, y, ceiling_color, game);
			else
				put_pixel(x, y, floor_color, game);
			x++;
		}
		y++;
	}
}

void init_game(t_game *game)
{
	init_player(&game->player);
	game->map = get_map();
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Game");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
	load_texture(game, &game->north_texture, "./kiki/textures/north_texture.xpm");
	load_texture(game, &game->south_texture, "./kiki/textures/south_texture.xpm");
	load_texture(game, &game->west_texture, "./kiki/textures/west_texture.xpm");
	load_texture(game, &game->east_texture, "./kiki/textures/east_texture.xpm");

	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);

}

bool	touch(float px, float py, t_game *game)
{
	int x = px / BLOCK;
	int y = py / BLOCK;
	if (game->map[y][x] == '1')
		return true;
	else
		return false;
}

float distance(float x, float y)
{
	return sqrt(x * x + y * y);
}

float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
	float delta_x = x2 - x1;
	float delta_y = y2 - y1;
	float angle = atan2(delta_y, delta_x) - game->player.angle;
	float fix_dist = distance(delta_x, delta_y) * cos(angle);
	return (fix_dist);
}

void draw_wall_line(t_game *game, int x, int start_y, int end_y, t_texture *texture, int tex_x) {
    int wall_height = end_y - start_y; 
    float texture_ratio = (float)texture->height / (float)wall_height; // Calcul de la proportion de la texture pour le mur

    // Clamp tex_x pour s'assurer qu'il reste dans les limites de la texture
    tex_x = tex_x < 0 ? 0 : (tex_x >= texture->width ? texture->width - 1 : tex_x);

    // Dessiner chaque ligne du mur
    for (int y = start_y; y < end_y; y++) {
        if (y < 0 || y >= HEIGHT) continue; // Vérifier les limites de l'écran
        int tex_y = (y - start_y) * texture_ratio;  // Calcul de la coordonnée Y de la texture

        // Clamp tex_y pour s'assurer qu'il reste dans les limites de la texture
        tex_y = tex_y < 0 ? 0 : (tex_y >= texture->height ? texture->height - 1 : tex_y);

        // Récupérer la couleur de la texture
        int color = texture->data[tex_y * texture->width + tex_x];

        // Dessiner le pixel sur l'écran
        put_pixel(x, y, color, game);
    }
}


void draw_line(t_player *player, t_game *game, float start_x, int i) {
    float cos_angle = cos(start_x);
    float sin_angle = sin(start_x);
    float ray_x = player->x;
    float ray_y = player->y;
    bool is_vertical = false;
    t_texture *texture;

    // Boucle de raycasting
    while (!touch(ray_x, ray_y, game)) {
        if (DEBUG == 1)
            put_pixel(ray_x, ray_y, 0xFF0000, game);

        // Réduire la tolérance pour une meilleure précision
        float tolerance = 0.999999999999999999999999;  // Tolérance réduite pour une meilleure précision
        is_vertical = (fmod(ray_x, BLOCK) < tolerance || fmod(ray_x, BLOCK) > (BLOCK - tolerance));

        // Incrémentation du rayon
        ray_x += cos_angle;
        ray_y += sin_angle;
    }

    // Calcul de la distance et de la hauteur du mur
    float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
    float height = (BLOCK / dist) * (WIDTH / 2);
    int start_y = (HEIGHT - height) / 2;
    int end_y = start_y + height;

    // Dessin de la ligne
    while (start_y < end_y) {
        if (DEBUG == 0)
            put_pixel(i, start_y, 255, game);
        start_y++;
    }

    start_y = (HEIGHT - height) / 2;
    int tex_x;
	
    // Vérification de la direction du rayon pour déterminer la texture
    if (is_vertical) {
        if (cos(start_x) > 0) {
            texture = &game->east_texture;
        } else if (cos(start_x) < 0) {
            texture = &game->west_texture;
        }
    } else {
        if (sin(start_x) > 0) {
            texture = &game->south_texture;
        } else if (sin(start_x) < 0) {
            texture = &game->north_texture;
        }
    }

    // Calcul de tex_x en fonction de la position du rayon
    if (is_vertical) {
        tex_x = (int)(fmod(ray_y, BLOCK) / BLOCK * texture->width);
    } else {
        tex_x = (int)(fmod(ray_x, BLOCK) / BLOCK * texture->width);
    }

    draw_wall_line(game, i, start_y, end_y, texture, tex_x);
}

int draw_loop(t_game *game)
{
	t_player *player = &game->player;
	move_player(player);
	clear_image(game);
	if (DEBUG == 1)
	{
		draw_square(player->x, player->y, 10, 0x00FF00, game);
		draw_map(game);
	}

	float fraction = PI / 3 / WIDTH;
	float start_x = player->angle - PI / 6;
	int i = 0;
	while (i < WIDTH)
	{
		draw_line(player, game, start_x, i);
		start_x += fraction;
		i++;
	}

	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return 0;
}

int main(void)
{
	t_game game;

	init_game(&game);

	mlx_hook(game.win, 2, 1L<<0, key_press, &game.player);
	mlx_hook(game.win, 3, 1L<<1, key_release, &game.player);
	//mlx_hook(game.win, EVENT_CLOSE_BTN, 0, mlx_loop_end, game.mlx);

	mlx_loop_hook(game.mlx, draw_loop, &game);
	mlx_loop(game.mlx);

	return (0);
}
