/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 01:05:50 by kdustin           #+#    #+#             */
/*   Updated: 2020/09/19 22:59:02 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reflection.h"

int	check_shadow(t_scene scene, t_reflection_data data)
{
	t_ray3d		l;
	t_object	obj;
	double		t;
	t_list		*objects;

	l.origin = sum_v(data.point, mul_vs(data.normal, 0.01));
	//l.origin = data.point;
	l.direction = data.light.direction;
	objects = scene.objects;
	while (objects != NULL)
	{
		t = -1;
		obj = *(t_object*)(objects->content);
		if (apply_intersect(l, obj, &t, NULL))
			return (-1);
		if (t > 0 && t<data.max_t)//t > __FLT_EPSILON__ && t < data.max_t)
			return (1);
		objects = objects->next;
	}
	return (0);
}

void	reflect_specular(t_scene scene, t_reflection_data data, t_vector3d *color)
{
	t_vector3d	reflected_vec;
	t_vector3d	light_direction = normalize(data.light.direction);
	double		cos = dot_v(data.normal, light_direction);
	double		refview;

	if (cos > 0)
	{
		reflected_vec = sub_v(mul_vs(data.normal, 2 * dot_v(data.normal, mul_vs(light_direction, -1))), mul_vs(light_direction, -1));
		refview = dot_v(reflected_vec, scene.camera.ray.direction);
		if (refview < 0)
		 refview = 0;
		*color = sum_v(*color, mul_vs(mul_vs(data.light_color, 0.77), pow(refview, data.specular)));
	}
}

void	reflect_diffusion(t_scene scene, t_reflection_data data, t_vector3d *color)
{
	double	cos = dot_v(normalize(data.normal), normalize(data.light.direction));
	if (cos > 0)
	{
		*color = sum_v(*color, mul_vs(mul_vs(data.light_color, 0.4), cos));
	}
}

t_color3d	calculate_diffusion_specular(t_scene scene, t_reflection_data data)
{
	t_color3d		point_color;
	t_light_point	*light;
	t_list			*lights;

	data.max_t = MAXFLOAT;
	lights = scene.lights;
	point_color = mul_vs(div_vs(scene.environment_light.color, 255), scene.environment_light.brightness);
	while (lights != NULL)
	{
		light = (t_light_point*)lights->content;
		data.max_t = 1;
		data.light.direction = sub_v(light->position, data.point);
		data.light.brightness = light->brightness;
		data.light_position = light->position;
		data.light_color = mul_vs(div_vs(light->color, 255), data.light.brightness);
		point_color = sum_v(point_color, mul_vs(data.light_color, 0.25));
		if (check_shadow(scene, data) == 0)
		{
			reflect_diffusion(scene, data, &point_color);
			if (data.specular >= 0)
				reflect_specular(scene, data, &point_color);
		}
		lights = lights->next;
	}
	return ((t_color3d){point_color.x * data.obj_color.x, point_color.y * data.obj_color.y, point_color.z * data.obj_color.z});
}

t_color3d	calculate_reflection(t_scene scene, double nearest_root,
							t_object nearest_obj)
{
	t_reflection_data	data;
	t_color3d			reflection_result;
	t_point3d			int_point;

	data.point = ray_func(scene.camera.ray, nearest_root);
	if (ft_strcmp(nearest_obj.type, "Sphere") == 0)
		data.normal = calculate_sphere_normal(*(t_sphere*)(nearest_obj.container), data.point, scene.camera.ray.origin);
	else if (ft_strcmp(nearest_obj.type, "Plane") == 0)
		data.normal = normal_to_camera(((t_plane*)nearest_obj.container)->normal, scene.camera.ray.direction);
	else if (ft_strcmp(nearest_obj.type, "Square") == 0)
		data.normal = normal_to_camera(((t_square*)nearest_obj.container)->normal, scene.camera.ray.direction);
	else if (ft_strcmp(nearest_obj.type, "Triangle") == 0)
		data.normal = normal_to_camera(((t_triangle*)nearest_obj.container)->normal, scene.camera.ray.direction);
	else if (ft_strcmp(nearest_obj.type, "Cylinder") == 0)
		data.normal = calculate_cylinder_normal(*(t_cylinder*)nearest_obj.container, data.point, scene.camera.ray);
	data.specular = 1000;
	data.obj_color = nearest_obj.color;
	reflection_result = calculate_diffusion_specular(scene, data);
	return (reflection_result);
}
