#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//A FAIRE
//QUAND le 0 est en position max ou min !!!
//MAX = INDEX X AVEC DES 1 ET NOMBRE DELEMENT Y - 1

/*char **get_map(void)
{
	char **map = malloc(sizeof(char *) * 11);
	map[0] = " 1 1 ";
	map[1] = "11111";
	map[2] = "10101";
	map[3] = "10N11";
	map[4] = " 1 1 ";
	map[5] = NULL;
	return (map);
}*/
char **get_map(void)
{
	char **map = malloc(sizeof(char *) * 15);
	map[0]	= "1111111111111111111111111";
	map[1]	= "1000000000110000000000001";
	map[2]	= "1011000001110000000000001";
	map[3]	= "1001000000000000000000001";
	map[4]	= "1111111110110000011100001";
	map[5]	= "1000000000110000011101111";
	map[6]	= "1111011111111101110000001";
	map[7] 	= "1111011111111111 10101001";
	map[8]	= "1100000011010101110000001";
	map[9]	= "1000000000000000110000001";
	map[10] = "1000000000000000110101001";
	map[11] = "1100000111010101111101110";
	map[12] = "1111011111101011011110101";
	map[13] = "1111111111111111111111111";
	map[14] = NULL;
	return (map);
}

bool	check_map3(char **tab, int x, int y, int i, int max_x, int max_y)
{
	bool flag;
	int y2;

	flag = false;
	y2 = y;
	while (y < max_y && y > 0)
	{
		y = y + i;
		printf("x is %d, y is $%d\n", x, y);
		if (tab[x][y] == '1')
		{
			flag = true;
			break ;
		}
		if (tab[x][y] == ' ')
			break ;
		flag = false;
	}
	if (!flag)
		return (flag);
	y = y2;
	while (x < max_x && x > 0)
	{
		x = x + i;
		printf("x is %d, y is $%d\n", x, y);
		if (tab[x][y] == '1')
		{
			flag = true;
			break ;
		}
		if (tab[x][y] == ' ')
			break ;
		flag = false;
	}
	printf("end\n");
	return flag;
}

bool	check_map2(char **tab, int x, int y, int max_x, int max_y)
{
	if (x == max_x || x == 0)
		return (false);
	if (y == max_y || y == 0)
		return (false);
	if (check_map3(tab, x, y, 1, max_x, max_y) == false)
		return (false);
	if (check_map3(tab, x, y, -1, max_x, max_y) == false)
		return (false);
	return (true);
}

void	check_map(char **tab, int max_x, int max_y)
{
	int y;
	int x;

	y = 0;
	x = 0;
	while (x <= max_x)
	{
		while (y <= max_y)
		{
			if (tab[x][y] == '0')
			{
				if (check_map2(tab, x, y, max_x, max_y) == false)
				{
					printf("error\n");
					free(tab);
					exit(1);
				}
			}
			y++;
		}
		x++;
		y = 0;
	}
	return ;
}

int main(void)
{
	char **map = get_map();

	//check_map(map, 4, 4);
	check_map(map, 13, 24);
	free(map);
	printf("you did it!\n");
	return 0;
}