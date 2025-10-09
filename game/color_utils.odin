package neonshooter_game

import "core:math"
import rl "vendor:raylib"

vec4_hsv :: proc(h, s, v: f32) -> Vec4 {
    if (h == 0 && s == 0) {
        return { v, v, v, 1.0 };
    }

    c := s * v;
    x := c * (1 - math.abs(math.mod(h, 2) - 1));
    m := v - c;

    if (h < 1)      do return { c + m, x + m, m    , 1.0 }
    else if (h < 2) do return { x + m, c + m, m    , 1.0 }
    else if (h < 3) do return { m    , c + m, x + m, 1.0 }
    else if (h < 4) do return { m    , x + m, c + m, 1.0 }
    else if (h < 5) do return { x + m, m    , c + m, 1.0 }
    else            do return { c + m, m    , x + m, 1.0 }
}

color_from_vec4 :: proc(v: Vec4) -> rl.Color {
    return { u8(v.x * 255), u8(v.y * 255), u8(v.z * 255), u8(v.w * 255) }
}