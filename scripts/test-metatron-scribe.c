#include <stdio.h>
#include <string.h>
#include "metatron.h"

static int fail(const char *m) {
    fprintf(stderr, "FAIL: %s\n", m);
    return 1;
}

static int has(const char *s, const char *p) {
    return strstr(s, p) != NULL;
}

int main(void) {
    static const MetatronSurfaceKind kinds[] = {
        METATRON_SURFACE_CONS,
        METATRON_SURFACE_OMI_LISP,
        METATRON_SURFACE_GEOMETRY,
        METATRON_SURFACE_BARCODE,
        METATRON_SURFACE_DOM,
        METATRON_SURFACE_GPIO,
        METATRON_SURFACE_SYMBOLIC,
        METATRON_SURFACE_PROJECTIVE
    };
    RingSlot s;
    MetatronScribeRecord r;
    char expect[512];

    for (size_t i = 0; i < sizeof(kinds)/sizeof(kinds[0]); i++) {
        const char *n = metatron_surface_name(kinds[i]);
        if (metatron_surface_parse(n) != kinds[i]) return fail("surface parse round trip");
    }
    if (metatron_surface_parse("missing") != METATRON_SURFACE_UNKNOWN) return fail("unknown parse");

    memset(&s, 0, sizeof(s));
    if (metatron_scribe_receipt(&s, METATRON_SURFACE_CONS, &r)) return fail("empty slot return");
    if (r.accepted || r.scribable || r.notation[0]) return fail("empty slot state");

    memset(&s, 0, sizeof(s));
    s.cycle = 7;
    s.hash = 0x0123456789abcdefull;
    if (metatron_scribe_receipt(&s, METATRON_SURFACE_CONS, &r)) return fail("hash-only slot return");
    if (r.accepted || r.scribable || r.notation[0]) return fail("hash-only slot state");

    memset(&s, 0, sizeof(s));
    strcpy(s.receipt, "{\"ok\":true}");
    if (metatron_scribe_receipt(&s, METATRON_SURFACE_CONS, &r)) return fail("receipt-only slot return");
    if (r.accepted || r.scribable || r.notation[0]) return fail("receipt-only slot state");

    memset(&s, 0, sizeof(s));
    s.cycle = 7;
    s.hash = 0x0123456789abcdefull;
    strcpy(s.receipt, "{\"ok\":true}");

    if (!metatron_scribe_accepted_slot(&s, METATRON_SURFACE_CONS, &r)) return fail("accepted slot scribe");
    if (!r.accepted || !r.scribable) return fail("accepted slot flags");

    if (metatron_scribe_receipt(&s, METATRON_SURFACE_UNKNOWN, &r)) return fail("unknown surface return");
    if (!r.accepted || r.scribable || r.notation[0]) return fail("unknown surface state");

    if (!metatron_scribe_receipt(&s, METATRON_SURFACE_CONS, &r)) return fail("cons scribe");
    if (!r.accepted || !r.scribable) return fail("cons flags");
    if (strcmp(r.notation, "(receipt . (7 . 0x0123456789abcdef))") != 0) return fail("cons notation");

    if (!metatron_scribe_receipt(&s, METATRON_SURFACE_OMI_LISP, &r)) return fail("omi-lisp scribe");
    snprintf(expect, sizeof(expect),
        "(metatron.scribe (surface \"omi-lisp\") (cycle 7) (hash \"0x0123456789abcdef\") (slot5040 %u) (frame %u) (fiber %u))",
        r.slot5040, r.frame_type, r.fiber_q);
    if (strcmp(r.notation, expect) != 0) return fail("omi-lisp notation");

    if (!metatron_scribe_receipt(&s, METATRON_SURFACE_GEOMETRY, &r)) return fail("geometry scribe");
    if (!has(r.notation, "(slot5040 ") || !has(r.notation, "(frame ") || !has(r.notation, "(fiber ")) return fail("geometry route fields");
    if (!has(r.notation, "(fano ") || !has(r.notation, "(role ") || !has(r.notation, "(local ")) return fail("geometry incidence fields");

    if (!metatron_scribe_receipt(&s, METATRON_SURFACE_DOM, &r)) return fail("dom scribe");
    if (!has(r.notation, "(metatron.declare") || !has(r.notation, "(surface \"dom\")") || !has(r.notation, "(effect none)")) return fail("dom declaration only");
    if (!metatron_scribe_receipt(&s, METATRON_SURFACE_GPIO, &r)) return fail("gpio scribe");
    if (!has(r.notation, "(surface \"gpio\")") || !has(r.notation, "(effect none)")) return fail("gpio declaration only");
    if (!metatron_scribe_receipt(&s, METATRON_SURFACE_BARCODE, &r)) return fail("barcode scribe");
    if (!has(r.notation, "(surface \"barcode\")") || !has(r.notation, "(effect none)")) return fail("barcode declaration only");

    memset(ring, 0, sizeof(ring));
    if (metatron_scribe_ring_latest(METATRON_SURFACE_CONS, &r)) return fail("empty ring latest return");
    if (r.accepted || r.scribable) return fail("empty ring latest state");
    ring[1] = s;
    ring[1].receipt[0] = 0;
    if (metatron_scribe_ring_latest(METATRON_SURFACE_CONS, &r)) return fail("candidate ring latest return");
    if (r.accepted || r.scribable) return fail("candidate ring latest state");
    ring[2] = s;
    ring[3] = s;
    ring[3].cycle = 9;
    ring[3].hash = 0x1111111111111111ull;
    if (!metatron_scribe_ring_latest(METATRON_SURFACE_CONS, &r)) return fail("latest scribe");
    if (r.cycle != 9 || r.hash != 0x1111111111111111ull) return fail("latest selected");

    printf("PASS metatron scribe\n");
    return 0;
}
