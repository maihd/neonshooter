package neonshooter_game

import "core:math"
import "base:runtime"
import "core:fmt"

import "../gxm"
import rl "vendor:raylib"


MAX_NUM_WARP_POINTS :: 8192


Warp_Point :: struct {
	pos: 		gxm.Vec3,
	vel: 		gxm.Vec3,
	accel:		gxm.Vec3,

	inv_mass: 	f32,
	damping:	f32,
}


Warp_Spring :: struct {
	end_point1:	^Warp_Point,
	end_point2:	^Warp_Point,

	target_len:	f32,
	stiffness:	f32,
	damping:	f32,
}


Warp_Grid :: struct {
	points: 		[MAX_NUM_WARP_POINTS]Warp_Point,
	fixed_points: 	[MAX_NUM_WARP_POINTS]Warp_Point,
	springs: 		[MAX_NUM_WARP_POINTS * 2]Warp_Spring,
    num_cols: 		i32,
    num_rows: 		i32,
    num_springs:	i32,
}


WARP_GRID_POINT_DAMPING :: 0.1

warp_point_apply_force :: proc "contextless" (point: ^Warp_Point, force: gxm.Vec3) {
	point.accel += force * point.inv_mass
}


warp_point_increase_damping :: proc "contextless" (point: ^Warp_Point, factor: f32) {
	point.damping *= factor
}


warp_grid_init :: proc "contextless" (grid: ^Warp_Grid, world_bbox: gxm.BBox2, spacing: gxm.Vec2) {
	counting := gxm.ivec2(gxm.round((world_bbox.max - world_bbox.min) / spacing))
	num_cols := counting.x + 2
	num_rows := counting.y + 2

	grid.num_cols = num_cols
	grid.num_rows = num_rows

	// Set point masses values
	c, r 	:= i32(0), i32(0)
	max_x 	:= world_bbox.min.x + f32(num_cols) * spacing.x
	max_y 	:= world_bbox.min.y + f32(num_rows) * spacing.y
	for y := world_bbox.min.y - spacing.y; y <= max_y; y += spacing.y {
		for x := world_bbox.min.x - spacing.x; x <= max_x; x += spacing.x {
			idx := r * num_cols + c
			
			grid.points[idx] = {
				pos 		= gxm.vec3(x, y, 0),
				vel 		= 0,
				accel 		= 0,
				inv_mass 	= 1,
				damping		= WARP_GRID_POINT_DAMPING,
			}

			grid.fixed_points[idx] = {
				pos 		= gxm.vec3(x, y, 0),
				vel 		= 0,
				accel 		= 0,
				inv_mass 	= 0,
				damping		= WARP_GRID_POINT_DAMPING,
			}

			c += 1
		}

		r += 1
		c  = 0
	}

	// Link the points together
	spring_len_scale := f32(0.95)
	anchor_to_fixed_x := i32(3)
	anchor_to_fixed_y := i32(3)
	for y in 0..<num_rows {
		for x in 0..<num_cols {
			point_idx := y * num_cols + x
		
			if  x == 0 ||
				y == 0 ||
				x == num_cols - 1 ||
				y == num_rows - 1 {
				end_point1 := &grid.fixed_points[point_idx]
				end_point2 := &grid.points[point_idx]
				target_len := gxm.dist(end_point1.pos, end_point2.pos) * 0.95
				grid.springs[grid.num_springs] = {
					end_point1 		= end_point1,
					end_point2 		= end_point2,
					stiffness 		= 12.0,
					damping 		= 0.1,
					target_len	 	= target_len,
				}
				
				grid.num_springs += 1
			} else if x % anchor_to_fixed_x == 0 && y % anchor_to_fixed_y == 0 {
				end_point1 := &grid.fixed_points[point_idx]
				end_point2 := &grid.points[point_idx]
				target_len := gxm.dist(end_point1.pos, end_point2.pos) * 0.95
				grid.springs[grid.num_springs] = {
					end_point1 		= end_point1,
					end_point2 		= end_point2,
					stiffness 		= 0.5,
					damping 		= 0.005,
					target_len	 	= target_len,
				}
				
				grid.num_springs += 1
			}
			
			// common data
			stiffness 	:= f32(10.2)
			damping		:= f32(0.06)

			// horizontal anchor
			if x > 0 {
				end_point1 := &grid.points[point_idx - 1]
				end_point2 := &grid.points[point_idx]
				target_len := gxm.dist(end_point1.pos, end_point2.pos) * 0.95
				grid.springs[grid.num_springs] = {
					end_point1 		= end_point1,
					end_point2 		= end_point2,
					stiffness 		= stiffness,
					damping 		= damping,
					target_len 		= target_len,
				}
				
				grid.num_springs += 1
			}

			// vertical anchor
			if y > 0 {
				end_point1 := &grid.fixed_points[(y - 1) * num_cols + x]
				end_point2 := &grid.points[point_idx]
				target_len := gxm.dist(end_point1.pos, end_point2.pos) * 0.98
				grid.springs[grid.num_springs] = {
					end_point1 		= end_point1,
					end_point2 		= end_point2,
					stiffness 		= stiffness,
					damping 		= damping,
					target_len 		= target_len,
				}
				
				grid.num_springs += 1
			}
		}
	}
}

warp_grid_deinit :: proc "contextless" (grid: ^Warp_Grid) {
    assert_contextless(grid != nil)
    grid^ = {}
}

warp_grid_update :: proc "contextless" (grid: ^Warp_Grid, dt: f32) {
	// update springs
	for idx in 0..<grid.num_springs {
		spr := &grid.springs[idx]
		dir := spr.end_point1.pos - spr.end_point2.pos
		len := gxm.len(dir)
		if len > spr.target_len {
			dis 	:= (dir / len) * (len - spr.target_len)
			dvel  	:= spr.end_point2.vel - spr.end_point1.vel
			force 	:= spr.stiffness * dis - dvel * spr.damping 
			// force 	:= spr.stiffness * dis - dvel * math.pow(spr.damping, dt)
			// @todo(maihd): add gxm.damping
			
			warp_point_apply_force(spr.end_point1, -force)
			warp_point_apply_force(spr.end_point2, +force)
		}
	}

	// context = runtime.default_context()
	// fmt.println("Update points")
	
	// update points
	for idx in 0..<(grid.num_cols * grid.num_rows) {
		point := &grid.points[idx]
		// gxm.integrate(&point.pos, &point.vel, point.accel, dt)

		point.vel 	 += point.accel
		point.pos    += point.vel * dt
		
		point.vel 	 *= math.pow(0.5, dt / point.damping)

		point.accel   = 0
		// point.accel  *= math.pow(point.damping, dt)
		// gxm.decay(&point.vel, point.damping, dt)
		// gxm.decay(&point.accel, point.damping, dt)
		point.damping = WARP_GRID_POINT_DAMPING

		if gxm.lensq(point.vel) <= 0.0001 {
			point.vel = {}
		}
	}
}

xna_catmull_rom :: #force_inline proc "contextless" (v1, v2, v3, v4: $T, t: f32) -> T {
	t2 := t * t
	t3 := t2 * t
	
	return 0.5 * (
		(2.0 * v2) +
		(-v1 + v3) * t +
		(2.0 * v1 - 5.0 * v2 + 4.0 * v3 - v4) * t2 +
		(-v1 + 3.0 * v2 - 3.0 * v3 + v4) * t3
	)
}

warp_grid_render :: proc "contextless" (grid: ^Warp_Grid, world_bbox: gxm.BBox2, dt: f32) {
	to_vec2 :: proc "contextless" (v3: gxm.Vec3) -> gxm.Vec2 {
		factor := (v3.z + 2000) / 2000
		return { v3.x, v3.y } * factor
	}
	
	num_cols := grid.num_cols
	num_rows := grid.num_rows
	
	color := rl.Color{30, 30, 139, 85} // DARK BLUE

	rl.BeginBlendMode(.ADD_COLORS)
	
	for y in 1..<num_rows {
		for x in 1..<num_cols {
			left, up := to_vec2(0), to_vec2(0)
			p := to_vec2(grid.points[y * num_cols + x].pos)

			if x > 1 {
				left = to_vec2(grid.points[y * num_cols + x - 1].pos)
				thickness := (y % 6) == 1 ? f32(3) : f32(1)

				// use Catmull-Rom interpolation to help smooth bends in the grid 
				clamped_x := min(x + 1, grid.num_cols - 1)
				mid := gxm.catmull_rom(
					to_vec2(grid.points[y * grid.num_cols + x - 2].pos), 
					left, 
					p, 
					to_vec2(grid.points[y * grid.num_cols + clamped_x].pos), 
					0.5
				)
				
				// If the grid is very straight here, draw a single straight line. Otherwise, draw lines to our 
				// new interpolated midpoint 
				if (gxm.distsq(mid, (left + p) / 2) > 1)
				{
					rl.DrawLineEx(left, mid, thickness, color);
					rl.DrawLineEx(mid, p, thickness, color);
				}
				else
				{
					rl.DrawLineEx(left, p, thickness, color)
				}
			}
			
			if y > 1 {
				up = to_vec2(grid.points[(y - 1) * num_cols + x].pos)
				thickness := (x % 6) == 1 ? f32(3) : f32(1)
				// rl.DrawLineEx(up, p, thickness, color)
				
				// use Catmull-Rom interpolation to help smooth bends in the grid 
				clamped_y := min(y + 1, grid.num_cols - 1)
				mid := gxm.catmull_rom(
					to_vec2(grid.points[(y - 2) * grid.num_cols + x].pos), 
					up, 
					p, 
					to_vec2(grid.points[clamped_y * grid.num_cols + x].pos), 
					0.5
				)
				
				// If the grid is very straight here, draw a single straight line. Otherwise, draw lines to our 
				// new interpolated midpoint 
				if (gxm.distsq(mid, (up + p) / 2) > 1)
				{
					rl.DrawLineEx(up, mid, thickness, color);
					rl.DrawLineEx(mid, p, thickness, color);
				}
				else
				{
					rl.DrawLineEx(up, p, thickness, color)
				}
			}

			if x > 1 && y > 1 {
				up_left := to_vec2(grid.points[(y - 1) * num_cols + (x - 1)].pos)
				thickness := f32(1)

				rl.DrawLineEx(0.5 * (up_left + up), 0.5 * (left + p), thickness, color)
				rl.DrawLineEx(0.5 * (up_left + left), 0.5 * (up + p), thickness, color)
			}
		}
	}

	rl.EndBlendMode()
}


warp_grid_apply_force :: proc "contextless" (grid: ^Warp_Grid, force, pos: gxm.Vec3, radius: f32) {
	for idx in 0..<(grid.num_cols * grid.num_rows) {
		point 	:= &grid.points[idx]
		distsq 	:= gxm.distsq(pos, point.pos)
		if distsq < radius * radius {
			warp_point_apply_force(point, 10 * force / (10 + gxm.sqrt(distsq)))
		}
	}
}


warp_grid_implosive_force :: proc "contextless" (grid: ^Warp_Grid, force, pos: gxm.Vec3, radius: f32) {
	for idx in 0..<(grid.num_cols * grid.num_rows) {
		point 	:= &grid.points[idx]
		distsq 	:= gxm.distsq(pos, point.pos)
		if distsq < radius * radius {
			warp_point_apply_force(point, 10 * force / (10 + gxm.sqrt(distsq)))
			warp_point_increase_damping(point, 0.6)
		}
	}
}


warp_grid_explosive_force :: proc "contextless" (grid: ^Warp_Grid, force, pos: gxm.Vec3, radius: f32) {
	for idx in 0..<(grid.num_cols * grid.num_rows) {
		point 	:= &grid.points[idx]
		distsq 	:= gxm.distsq(pos, point.pos)
		if distsq < radius * radius {
			warp_point_apply_force(point, 100 * force * (point.pos - pos) / (10000 + distsq))
			warp_point_increase_damping(point, 0.6)
		}
	}
}