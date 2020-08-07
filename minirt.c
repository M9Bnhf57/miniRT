/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/01 13:31:53 by kdustin           #+#    #+#             */
/*   Updated: 2020/08/07 19:32:58 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	calculate_diffuse_reflection(t_point3d point, t_vector3d norm, t_light_environment env, t_list *lights)
{
	double			temp;
	double			point_brightness;
	t_object		*light;
	t_light_point		light_p;
	t_light_directional	light_d;

	point_brightness = env.brightness;
	while (lights != NULL)
	{
		light = (t_object*)lights->content;
		if (ft_strcmp(light->type, "Light_directional"))
		{
			light_d = *(t_light_directional*)light->container;
		}
		else if (ft_strcmp(light->type, "Light_point"))
		{
			light_p = *(t_light_point*)light->container;
			light_d.direction = minus_vec(light_p.position, point);
			light_d.brightness = light_p.brightness;
		}
		if ((temp =
		(light_d.brightness * dot_vec(norm, light_d.direction)) /
		(module_vec(norm) * module_vec(light_d.direction))) > 0)
			point_brightness += temp;
	}
	return (point_brightness);
}

/*
**	apply intersect function choosen for sertain primitive.
*/

int	apply_intersect(t_ray3d r, t_object obj, double *nearest_root,
							t_object *nearest_obj)
{
	double	*roots;

	if (!(roots = obj.intersect_function(r, obj.container)))
		return (-1);
	if ((roots[0] > -1 || roots[1] > -1) &&
	(*nearest_root == -1 || (roots[0] < *nearest_root || roots[1] <
								*nearest_root)))
	{
		*nearest_obj = obj;
		*nearest_root = roots[0] < roots[1] ? roots[0] : roots[1];
	}
	free(roots);
	return (0);
}

/*
**	trace_ray function
**	if find ray go trought object than we get it's color.
**	if we find two object in one spote we get color of neares.
*/

int	trace_ray(t_ray3d r, t_list *objects)
{
	double		*crossing_point;
	t_object	obj;
	t_object	nearest_obj;
	double		nearest_root;

	nearest_root = -1;
	while (objects != NULL)
	{
		obj = *(t_object*)(objects->content);
		if (apply_intersect(r, obj, &nearest_root, &nearest_obj)
									< 0)
			return (-2);
		objects = objects->next;
	}
	if (nearest_root != -1)
		return (create_trgb(0, nearest_obj.color.x, nearest_obj.color.y,
							nearest_obj.color.z));
	return (-1);
}

int	render_return(int ret, t_list *objects)
{
	ft_lstclear(&objects, delete_object);
	return (ret);
}

t_list	*init_lights()
{
	t_list			*lights;
	t_list			*temp;
	t_object		*object;
	
	if (!(object = create_object("Light_point", create_light_point((t_point3d){5, 5, 5}, 0.6), (t_color3d){255, 255, 255})))
		return (NULL);
	if (!(lights = ft_lstnew(object)))
	{
		delete_object(object);
		return (NULL);
	}
	if (!(object = create_object("Light_directional", create_light_directional((t_vector3d){1, 4, 4}, 0.2), (t_color3d){255, 255, 255})))
	{
		ft_lstclear(&lights, delete_object);
		return (NULL);
	}
	if (!(temp = ft_lstnew(object)))
	{
		delete_object(object);
		ft_lstclear(&lights, delete_object);
		return (NULL);
	}
	ft_lstadd_back(&lights, temp);
	return (lights);
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
			if ((color = trace_ray(scene.camera.ray, scene.objects))
									>= 0)
				draw_pixel(img, canvas_to_screen(point, screen),
									color);
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

