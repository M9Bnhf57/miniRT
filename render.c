/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 02:15:29 by kdustin           #+#    #+#             */
/*   Updated: 2020/09/20 19:24:15 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

int	trace_ray(t_scene scene)
{
	t_object	obj;
	t_object	near_obj;
	double		near_root;
	t_list		*objects;

	objects = scene.objects;
	near_root = -1;
	while (objects != NULL)
	{
		obj = *(t_object*)(objects->content);
		if (apply_intersect(scene.camera.ray, obj, &near_root, &near_obj) < 0)
			return (-2);
		objects = objects->next;
	}
	if (near_root != -1)
		return (to_trgb(calculate_reflection(scene, near_root, near_obj)));
	return (-1);
}

int	render_return(int ret, t_list *objects)
{
	//ft_lstclear(&objects, delete_object);
	return (ret);
}

int	render(t_screen screen, t_data *img, t_scene scene, int camera_number)
{
	const t_canvas	canvas = create_canvas(screen);
	t_point2d		point;
	int				color;
	t_list			*lights;
	int				test;

	scene.camera = *(t_camera*)ft_lstget(scene.cameras,
									abs(camera_number % scene.cameras_counter));
	point.y = canvas.top_border + 1;
	while (--point.y > canvas.bottom_border)
	{
		point.x = canvas.left_border - 1;
		while (++point.x < canvas.right_border)
		{
			draw_background(img, point, screen, canvas);
			scene.camera.ray.direction =
				normalize(apply_matrix(scene.camera.rotation_matrix,
				canvas_to_viewport(point, canvas, scene.camera.viewport)));
			if ((color = trace_ray(scene)) >= 0)
				draw_pixel(img, canvas_to_screen(point, screen), color);
			else if (color == -2)
				return (render_return(-2, scene.objects));
		}
	}
	return (render_return(0, scene.objects));
}
