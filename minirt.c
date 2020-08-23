/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/01 13:31:53 by kdustin           #+#    #+#             */
/*   Updated: 2020/08/23 17:10:03 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
**	trace_ray function
**	if find ray go trought object than we get it's color.
**	if we find two object in one spote we get color of neares.
*/

int	trace_ray(t_scene scene)//t_ray3d r, t_list *objects, t_light_environment env, t_list *lights)
{
	double		*crossing_point;
	t_object	obj;
	t_object	nearest_obj;
	double		nearest_root;
	t_list		*objects;

	objects = scene.objects;
	nearest_root = -1;
	while (objects != NULL)
	{
		obj = *(t_object*)(objects->content);
		if (apply_intersect(scene.camera.ray, obj, &nearest_root, &nearest_obj) < 0)
			return (-2);
		objects = objects->next;
	}
	if (nearest_root != -1)
	{
		return (color3d_to_trgb(mul_vec_scalar(nearest_obj.color,
		calculate_reflection(scene, nearest_root, nearest_obj))));
	}
	return (-1);
}

int	render_return(int ret, t_list *objects)
{
	ft_lstclear(&objects, delete_object);
	return (ret);
}
                                                                                //  разобратся с t_data, разобратся с цветами  initobj initlight malloc
int	render(t_screen screen, t_data *img)
{
	const t_canvas	canvas = create_canvas(screen);
	t_scene		scene;
	t_point2d	point;
	int		color;

	t_list	*lights;
	scene = init_scene(init_objects(), init_lights(), (t_light_environment){0.2},
				(t_viewport){1, 1, 1}, (t_point3d){0, 0, 0});
	point.y = canvas.top_border + 1;
	while (--point.y > canvas.bottom_border)
	{
		point.x = canvas.left_border - 1;
		while (++point.x < canvas.right_border)
		{
			draw_background(img, point, screen, canvas);
			scene.camera.ray.direction = canvas_to_viewport(point,
						canvas, scene.camera.viewport);
			if ((color = trace_ray(scene)) >= 0)
				draw_pixel(img, canvas_to_screen(point, screen), color);
			else if (color == -2)
				return (render_return(-2, scene.objects));
		}
	}
	return (render_return(0, scene.objects));
}

int	main(void)
{
	const t_screen	screen = (t_screen){800, 600};
	void		*mlx;
	void		*mlx_win;
	t_data		img;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, screen.width, screen.height, "MLX!");
	img.img = mlx_new_image(mlx, screen.width, screen.height);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
						&img.line_length, &img.endian);
	if (render(screen, &img) < 0)
		return (-1);
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
	return (0);
}

