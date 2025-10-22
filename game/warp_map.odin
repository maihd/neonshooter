package neonshooter_game

Warp_Cell :: struct {
    
}

Warp_Map :: struct {
    cols: int,
    rows: int,
    cell_size: f32,
    cells: []Warp_Cell,
}

warp_map_init :: proc(warp_map: ^Warp_Map, allocator := context.allocator) {
    
}

warp_map_deinit :: proc(warp_map: ^Warp_Map) {
    if warp_map != nil {


        warp_map^ = {}
    }
}