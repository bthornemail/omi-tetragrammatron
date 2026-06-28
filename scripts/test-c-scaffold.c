#include <stdio.h>
#include <string.h>
#include "omi.h"

static unsigned char mem[OMI_ARENA_SIZE];

static int fail(const char *m) {
    fprintf(stderr, "FAIL: %s\n", m);
    return 1;
}

int main(void) {
    omi_arena_t a;
    omi_bridge_result_t br;
    omi_gauge_entry_t *e;
    omi_u64 b;

    if (sizeof(omi_gauge_entry_t) != 32) return fail("gauge entry size");
    if (sizeof(omi_cons_cell_t) != 16) return fail("cons cell size");
    if (sizeof(omi_receipt_slot_t) != 64) return fail("receipt slot size");

    memset(mem, 0xff, sizeof(mem));
    omi_arena_init(&a, mem, sizeof(mem));
    if (a.base != mem) return fail("arena base");
    if (a.heap_free != OMI_OFFSET_HEAP) return fail("arena heap offset");
    if (OMI_GET64(&a, OMI_OFFSET_BITBOARD) != 0) return fail("arena zeroes bitboard");

    e = omi_gauge_get(&a, 0x78);
    if (!e) return fail("gauge 0x78 exists");
    if (!(e->flags & OMI_GAUGE_FLAG_ACTIVE)) return fail("gauge active flag");
    if (e->bridge != OMI_BRIDGE_SYSTEM_WITNESS) return fail("gauge 0x78 bridge");

    br = omi_bridge_resolve(&a, OMI_BRIDGE_EXTERNAL, OMI_HANDLE_NIL);
    if (!br.recognized || !br.staged || br.action != 5) return fail("external bridge result");
    if (!omi_bridge_is_staged(&a, OMI_BRIDGE_EXTERNAL)) return fail("external staged");
    if (OMI_GET16(&a, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_EXTERNAL) != OMI_BRIDGE_EXTERNAL) return fail("external bridge slot");
    if (OMI_GET32(&a, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_BOOT_PAGE) != 0) return fail("bridge slots independent");

    br = omi_bridge_resolve(&a, OMI_BRIDGE_BOOT_PAGE, OMI_HANDLE_NIL);
    if (!br.recognized || !br.staged || br.action != 3) return fail("boot bridge result");
    if (!omi_bridge_is_staged(&a, OMI_BRIDGE_BOOT_PAGE)) return fail("boot staged");

    b = omi_gauge_process(&a, 0x78, OMI_HANDLE_NIL);
    if ((b & OMI_BB_GAUGE_MASK) != 0x78) return fail("bitboard gauge byte");
    if (!(b & (1ull << OMI_BB_BRIDGE_78_BIT))) return fail("bitboard 0x78 bridge bit");
    if (!(b & (1ull << OMI_BB_BOOT_7C00_BIT))) return fail("bitboard boot bridge bit");
    if (!(b & (1ull << OMI_BB_BRIDGE_AA55_BIT))) return fail("bitboard external bridge bit");

    b = omi_gauge_process(&a, 0x21, OMI_HANDLE_NIL);
    if (OMI_GET8(&a, OMI_OFFSET_REGISTER_INJECT + 1) != 0x21) return fail("register stage");

    b = omi_gauge_process(&a, 0x7f, OMI_HANDLE_NIL);
    if (!(b & (1ull << OMI_BB_SEAL_7F_BIT))) return fail("seal bit");

    if (omi_projection_allowed(&a, OMI_MAKE_HANDLE(OMI_HANDLE_TAG_RECEIPT, 1), OMI_EFFECT_HARDWARE)) return fail("projection must deny unaccepted receipt");
    if (!omi_symbol_intern(&a, "effect")) return fail("symbol intern");
    if (omi_symbol_intern(&a, "effect") != omi_symbol_intern(&a, "effect")) return fail("symbol intern deterministic");

    printf("PASS c scaffold\n");
    return 0;
}
