/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 22:01:47 by rpehkone          #+#    #+#             */
/*   Updated: 2020/08/24 19:53:30 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void	rotate(t_float_xy *direction, double angle)
{
	double	sin_angle;
	double	cos_angle;
	float	x;
	float	y;

	sin_angle = sin(angle);
	cos_angle = cos(angle);
	x = direction->x;
	y = direction->y;
	direction->x = x * cos_angle + y * sin_angle;
	direction->y = y * cos_angle - x * sin_angle;
}

void		collision(t_float_xy *location, t_float_xy *direction, int neg, char **map)
{
	int speed = 40;
	t_int_xy	loc_on_map_f;

	loc_on_map_f.y = map[(int)(location->y + neg * direction->y * speed)]
							[(int)location->x];
	loc_on_map_f.x = map[(int)location->y]
							[(int)(location->x + neg * direction->x * speed)];
	if ('1' != loc_on_map_f.x && '2' != loc_on_map_f.x && loc_on_map_f.x != '3')
		location->x += direction->x * speed * neg;
	if ('1' != loc_on_map_f.y && '2' != loc_on_map_f.y && loc_on_map_f.y != '3')
		location->y += direction->y * speed * neg;
}

static void	player_movement(t_settings *settings)
{
	static int	anti_jump = 0;
	t_float_xy	tangent;
	int32_t		deltax;
	int			fwd;
	int			bwd;

	CGGetLastMouseDelta(&deltax, &fwd);
	fwd = is_key_down(126) + is_key_down(13);
	bwd = is_key_down(125) + is_key_down(1);
	if (settings->menu)
	{
		anti_jump = 1;
		return ;
	}
	if (deltax && !anti_jump)
		rotate(&settings->direction, deltax * -0.005);
	if (deltax)
		anti_jump = 0;
	if (is_key_down(124))
		rotate(&settings->direction, -0.05);
	if (is_key_down(123))
		rotate(&settings->direction, 0.05);
	if (fwd)
		collision(&settings->location, &settings->direction, 1, settings->map);
	if (bwd)
		collision(&settings->location, &settings->direction, -1, settings->map);
	tangent = settings->direction;
	rotate(&tangent, 1.57079633);
	if (is_key_down(0))
		collision(&settings->location, &tangent, 1, settings->map);
	if (is_key_down(2))
		collision(&settings->location, &tangent, -1, settings->map);
	if (settings->map[(int)settings->location.y]
			[(int)(settings->location.x)] == '4')
	{
		printf("Victory\n");
		exit(0);
	}
}

void		capture_cursor(t_settings *settings)
{
	UInt32		dispid;
	CGPoint		cursor;
	static int	last_menu_state = 1;

	//if lose focus menu = 1;
	dispid = CGMainDisplayID();
	cursor.x = 700;
	cursor.y = 700;
	//move releative to window;
	if (!settings->menu)
		CGWarpMouseCursorPosition(cursor);
	if (last_menu_state != settings->menu)
	{
		if (!settings->menu)
			CGDisplayHideCursor(dispid);
		else
		{
			CGDisplayShowCursor(dispid);
			CGDisplayMoveCursorToPoint(dispid, cursor);
		}
	}
	last_menu_state = settings->menu;
}

int			wolf(void)
{
	static t_settings settings;

	if (is_key_down(53))
		exit(0);
	else if (!settings.map)
		read_map(NULL, &settings);
	player_movement(&settings);
	make_threads(&settings);
	if (settings.cs_mode)
		crosshair();
	if (settings.print_map)
		map_print(&settings);
	if (!settings.menu)
		update_image();
	buttons(&settings);
	put_gun(&settings);
	if (settings.fps)
		fps();
	capture_cursor(&settings);
	return (0);
}
