#include "cub3d.h"

void	init_player(t_player *player)
{
	player->x = WIDTH / 2;
	player->y = HEIGHT / 2;
	player->angle = 3 * PI / 2;


	player->key_left = false;
	player->key_right = false;
	player->key_up = false;
	player->key_down = false;
	player->left_rotate = false;
	player->right_rotate = false;

}

int	key_press(int key, t_player *player)
{
	if (key == KEY_A)
	{
		player->key_left = true;
		printf("keyleft\n");
	}
	if (key == KEY_D)
	{
		player->key_right = true;
		printf("keyright\n");
	}
	if (key == KEY_W)
	{
		player->key_up = true;
		printf("keyup\n");
	}
	if (key == KEY_S)
	{
		player->key_down = true;
		printf("keydown\n");
	}
	if (key == KEY_LEFT)
	{
		player->left_rotate = true;
		printf("leftrotate\n");
	}
	if (key == KEY_RIGHT)
	{
		player->right_rotate = true;
		printf("rightrotate\n");
	}
	return 0;
}

int	key_release(int key, t_player *player)
{
	if (key == KEY_A)
	{
		player->key_left = false;
		printf("keyleft\n");
	}
	if (key == KEY_D)
	{
		player->key_right = false;
		printf("keyright\n");
	}
	if (key == KEY_W)
	{
		player->key_up = false;
		printf("keyup\n");
	}
	if (key == KEY_S)
	{
		player->key_down = false;
		printf("keydown\n");
	}
	if (key == KEY_LEFT)
	{
		player->left_rotate = false;
		printf("leftrotate\n");
	}
	if (key == KEY_RIGHT)
	{
		player->right_rotate = false;
		printf("rightrotate\n");
	}
	return 0;
}

void	move_player(t_player *player)
{
	int speed = 1;
	float angle_speed = 0.1;

	// Calcul des cosinus et sinus pour l'angle
	float cos_angle = cos(player->angle);
	float sin_angle = sin(player->angle);

	// Rotation
	if (player->left_rotate)
		player->angle -= angle_speed;
	if (player->right_rotate)
		player->angle += angle_speed;

	// Maintenir l'angle dans l'intervalle [0, 2*PI]
	if (player->angle > 2 * PI)
		player->angle -= 2 * PI;
	if (player->angle < 0)
		player->angle += 2 * PI;

	// Déplacement avant/arrière (up/down)
	if (player->key_up)
	{
		player->x += cos_angle * speed;
		player->y += sin_angle * speed;
		printf("up\n");
	}
	if (player->key_down)
	{
		player->x -= cos_angle * speed;
		player->y -= sin_angle * speed;
		printf("down\n");
	}

	// Déplacement latéral (left/right)
	if (player->key_left)
	{
		player->x += sin_angle * speed; // Utilisation du sinus pour gauche
		player->y -= cos_angle * speed; // Perpendiculaire à l'angle
		printf("left\n");
	}
	if (player->key_right)
	{
		player->x -= sin_angle * speed; // Utilisation du sinus pour droite
		player->y += cos_angle * speed; // Perpendiculaire opposé à l'angle
		printf("right\n");
	}
}