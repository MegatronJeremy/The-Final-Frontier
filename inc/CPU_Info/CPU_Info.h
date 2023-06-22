#pragma once

#define L1_CACHE_ID (0)

int get_cache_line_size(int id) {
    // Variables to hold the contents of the 4 i386 legacy registers
    uint32_t eax = 4; // get cache info
    uint32_t ebx;
    uint32_t ecx = id; // cache id
    uint32_t edx;

    // generates output in 4 registers eax, ebx, ecx and edx
    __asm__ (
            "cpuid" // call i386 cpuid instruction
            :   "+a" (eax), // contains the cpuid command code, 4 for cache query
    "=b" (ebx),
    "+c" (ecx), // contains the cache id
    "=d" (edx)
            );

    // See the page 3-191 of the manual.
    int cache_type = eax & 0x1F;

    // end of valid cache identifiers
    if (cache_type == 0) {
        return 64; // failsafe
    }

    return (ebx & 0xFFF) + 1;
}