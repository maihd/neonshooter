package neonshooter_game

Hash_Indexor_Num_Items          :: 1024
Hash_Indexor_Num_Buckets        :: 64
Hash_Indexor_Bucket_Size        :: Hash_Indexor_Num_Items / Hash_Indexor_Num_Buckets

#assert(Hash_Indexor_Bucket_Size * Hash_Indexor_Num_Buckets == Hash_Indexor_Num_Items)

Hash_Indexor :: struct {
    hashes:             [Hash_Indexor_Num_Items]u64,
    indices:            [Hash_Indexor_Num_Items]i32,
    bucket_num_items:   [Hash_Indexor_Bucket_Size]i32,
}

@(require_results)
hi_bucket_idx :: proc "contextless" (hash: u64) -> int {
    return int(hash % Hash_Indexor_Bucket_Size)
}

@(require_results)
hi_storage_idx_to_bucket_idx :: proc "contextless" (#any_int storage_idx: int) -> int {
    return storage_idx % Hash_Indexor_Bucket_Size
}

@(require_results)
hi_storage_idx :: proc "contextless" (#any_int bucket_idx, offset: int) -> int {
    return int(bucket_idx * Hash_Indexor_Bucket_Size + offset)
}

@(require_results)
hi_find_slot :: proc "contextless" (hi: ^Hash_Indexor, hash: u64) -> (int, bool) #optional_ok {
    bucket_idx := hi_bucket_idx(hash)
    for i in 0..<hi.bucket_num_items[bucket_idx] {
        idx := hi_storage_idx(bucket_idx, i)

        if hi.hashes[idx] == hash {
            return idx, true
        }
    }

    return -1, false
}

hi_add :: proc "contextless" (hi: ^Hash_Indexor, hash: u64, index: i32) -> bool {
    bucket_idx := hi_bucket_idx(hash)
    assert_contextless(hi.bucket_num_items[bucket_idx] < Hash_Indexor_Bucket_Size)

    idx                := hi_storage_idx(bucket_idx, hi.bucket_num_items[bucket_idx])
    hi.hashes[idx]      = hash
    hi.indices[idx]     = index

    hi.bucket_num_items[bucket_idx] += 1

    return true
}

hi_set :: proc "contextless" (hi: ^Hash_Indexor, hash: u64, index: i32) -> bool {
    idx, found := hi_find_slot(hi, hash)
    if found {
        hi.indices[idx] = index
        return true
    }

    return false
}

hi_get :: proc "contextless" (hi: ^Hash_Indexor, hash: u64) -> (i32, bool) {
    idx, found := hi_find_slot(hi, hash)
    if found {
        return hi.indices[idx], true
    }

    return -1, false
}

hi_remove :: proc "contextless" (hi: ^Hash_Indexor, hash: u64) -> (i32, bool) {
    slot, found := hi_find_slot(hi, hash)

    if found {
        index               := hi.indices[slot]
        bucket_idx          := hi_bucket_idx(hash)
        last_idx_in_bucket  := hi_storage_idx(bucket_idx, hi.bucket_num_items[bucket_idx] - 1)

        hi.hashes[slot]  = hi.hashes[last_idx_in_bucket]
        hi.indices[slot] = hi.indices[last_idx_in_bucket]

        hi.bucket_num_items[bucket_idx] -= 1

        return index, true
    }

    return -1, false
}
