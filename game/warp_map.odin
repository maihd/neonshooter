package neonshooter_game

Warp_Map :: struct {

}

warp_map_init :: proc(warp_map: ^Warp_Map, allocator := context.allocator) {
    
}

warp_map_deinit :: proc(warp_map: ^Warp_Map) {
    if warp_map != nil {


        warp_map^ = {}
    }
}