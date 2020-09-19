/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/01 13:34:01 by kdustin           #+#    #+#             */
/*   Updated: 2020/09/19 22:01:00 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include "point.h"

typedef	t_point3d	t_vector3d;
typedef t_point4d	t_vector4d;

t_vector3d		sum_v(t_vector3d a, t_vector3d b);
t_vector3d		sub_v(t_vector3d a, t_vector3d b);
double			dot_v(t_vector3d a, t_vector3d b);
t_vector3d		cross_v(t_vector3d a, t_vector3d b);
t_vector3d		mul_vs(t_vector3d a, double n);
t_vector3d		div_vs(t_vector3d a, double n);
double			module_v(t_vector3d a);
t_vector3d		normalize(t_vector3d a);

t_vector4d		sum_vec4d(t_vector4d a, t_vector4d b);
t_vector4d		sub_vec4d(t_vector4d a, t_vector4d b);
double			dot_vec4d(t_vector4d a, t_vector4d b);
t_vector4d		mul_vec4d_scalar(t_vector4d a, double n);
t_vector4d		div_vec4d_scalar(t_vector4d a, double n);
double			module_vec4d(t_vector4d a);
t_vector4d		unit_vec4d(t_vector4d a);

t_vector4d		v3_to_v4(t_vector3d v);
t_vector3d		v4_to_v3(t_vector4d v);
#endif
