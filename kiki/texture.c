#include "cub3d.h"
#include <string.h>

void load_texture(t_game *game, t_texture *texture, char *file_path)
{
    // Charger la texture d'origine
    texture->img = mlx_xpm_file_to_image(game->mlx, file_path, &texture->width, &texture->height);
    if (!texture->img)
    {
        fprintf(stderr, "Error: Failed to load texture from %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    // Allocate memory for texture data
    texture->data = (int *)malloc(texture->width * texture->height * sizeof(int));
    if (!texture->data)
    {
        fprintf(stderr, "Error: Failed to allocate memory for texture data\n");
        mlx_destroy_image(game->mlx, texture->img);
        exit(EXIT_FAILURE);
    }

    // Get the data address for the original texture
    int *original_data = (int *)mlx_get_data_addr(texture->img, &game->bpp, &game->size_line, &game->endian);
    if (!original_data)
    {
        fprintf(stderr, "Error: Failed to get data address for texture\n");
        free(texture->data);
        mlx_destroy_image(game->mlx, texture->img);
        exit(EXIT_FAILURE);
    }

    // Copy the original texture data to the allocated data
    memcpy(texture->data, original_data, texture->width * texture->height * sizeof(int));

    // Création de la nouvelle image avec les dimensions de la fenêtre
    t_texture *resized_texture = malloc(sizeof(t_texture));
    if (!resized_texture)
    {
        fprintf(stderr, "Error: Failed to allocate memory for resized texture\n");
        free(texture->data);
        mlx_destroy_image(game->mlx, texture->img);
        exit(EXIT_FAILURE);
    }

    resized_texture->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    resized_texture->data = (int *)mlx_get_data_addr(resized_texture->img, &game->bpp, &game->size_line, &game->endian);

    if (!resized_texture->img || !resized_texture->data)
    {
        fprintf(stderr, "Error: Failed to create resized image\n");
        free(resized_texture);
        free(texture->data);
        mlx_destroy_image(game->mlx, texture->img);
        exit(EXIT_FAILURE);
    }

    // Remplir la nouvelle image avec la texture redimensionnée
    int x, y;
    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            // Calcul des coordonnées de la texture d'origine
            int tex_x = x * texture->width / WIDTH;
            int tex_y = y * texture->height / HEIGHT;

            // Récupérer la couleur de la texture d'origine
            int color = texture->data[tex_y * texture->width + tex_x];

            // Placer la couleur dans la nouvelle image
            resized_texture->data[y * WIDTH + x] = color;
        }
    }

    // Libérer la texture d'origine (si nécessaire)
    mlx_destroy_image(game->mlx, texture->img);
    free(texture->data); // Free the original texture data

    // Assignation de l'image redimensionnée à la texture
    texture->img = resized_texture->img;
    texture->data = resized_texture->data;
    texture->width = WIDTH;
    texture->height = HEIGHT;

    // Libérer la structure resized_texture
    free(resized_texture);
}
