#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "omicron.h"

static int fail(const char *m) {
    fprintf(stderr, "FAIL: %s\n", m);
    return 1;
}

static int eq_frame(const uint16_t got[8], const uint16_t want[8]) {
    for (int i = 0; i < 8; i++) if (got[i] != want[i]) return 0;
    return 1;
}

int main(void) {
    OmicronConfig cfg;
    OmicronAddressCandidate ac;
    OmiLogCandidate lc;
    uint8_t h[OMICRON_PREHEADER_LEN];
    char *boot_argv[] = {"omicron.bin", "--boot"};
    char *eval_argv[] = {"omicron.bin", "--eval", "(cons 1 2)"};
    char *eval_missing_argv[] = {"omicron.bin", "--eval"};
    char *serve_argv[] = {"omicron.bin", "--serve", "8081"};
    char *scribe_argv[] = {"omicron.bin", "--scribe", "cons"};
    char *bitboard_argv[] = {"omicron.bin", "--bitboard"};
    char *expr_argv[] = {"omicron.bin", "(cons 1 2)"};
    static const unsigned char obj[] = {1,2,3,4};

    omicron_config_init(&cfg);
    if (cfg.mode != OMICRON_MODE_CLI) return fail("default mode");
    if (cfg.command != OMICRON_COMMAND_DEFAULT) return fail("default command");
    if (cfg.dialect != OMICRON_DIALECT_OMICRON) return fail("default dialect");
    if (strcmp(omicron_mode_name(cfg.mode), "cli") != 0) return fail("mode name");
    if (strcmp(omicron_command_name(cfg.command), "default") != 0) return fail("command name");
    if (strcmp(omicron_dialect_name(cfg.dialect), "omicron") != 0) return fail("dialect name");

    if (!omicron_stage_preheader(OMICRON_DIALECT_OMICRON, h)) return fail("omicron preheader");
    if (memcmp(h, "\xf0\x00\x1c\x1d\x1e\x1f\x20\xf0", OMICRON_PREHEADER_LEN) != 0) return fail("omicron preheader bytes");
    if (!omicron_stage_preheader(OMICRON_DIALECT_CANONICAL, h)) return fail("canonical preheader");
    if (memcmp(h, "\xff\x00\x1c\x1d\x1e\x1f\x20\xff", OMICRON_PREHEADER_LEN) != 0) return fail("canonical preheader bytes");
    if (omicron_stage_preheader((OmicronDialect)0xefu, h)) return fail("invalid preheader");

    if (!omicron_config_from_cli(&cfg, 2, boot_argv)) return fail("config boot cli");
    if (cfg.mode != OMICRON_MODE_SOFTWARE_BOOT) return fail("boot mode");
    if (cfg.command != OMICRON_COMMAND_BOOT) return fail("boot command");
    if (!omicron_config_from_cli(&cfg, 3, eval_argv)) return fail("config eval cli");
    if (cfg.command != OMICRON_COMMAND_EVAL) return fail("eval command");
    if (strcmp(cfg.command_arg, "(cons 1 2)") != 0) return fail("eval arg");
    if (!omicron_config_from_cli(&cfg, 2, eval_missing_argv)) return fail("config eval missing cli");
    if (cfg.command != OMICRON_COMMAND_EXPR) return fail("eval missing falls through");
    if (strcmp(cfg.command_arg, "--eval") != 0) return fail("eval missing expr arg");
    if (!omicron_config_from_cli(&cfg, 3, serve_argv)) return fail("config serve cli");
    if (cfg.command != OMICRON_COMMAND_SERVE) return fail("serve command");
    if (strcmp(cfg.command_arg, "8081") != 0) return fail("serve arg");
    if (!omicron_config_from_cli(&cfg, 3, scribe_argv)) return fail("config scribe cli");
    if (cfg.command != OMICRON_COMMAND_SCRIBE) return fail("scribe command");
    if (strcmp(cfg.command_arg, "cons") != 0) return fail("scribe arg");
    if (!omicron_config_from_cli(&cfg, 2, bitboard_argv)) return fail("config bitboard cli");
    if (cfg.mode != OMICRON_MODE_BITBOARD) return fail("bitboard mode");
    if (cfg.command != OMICRON_COMMAND_DEFAULT) return fail("bitboard command remains default");
    if (!omicron_config_from_cli(&cfg, 2, expr_argv)) return fail("config expr cli");
    if (cfg.command != OMICRON_COMMAND_EXPR) return fail("expr command");
    if (strcmp(cfg.command_arg, "(cons 1 2)") != 0) return fail("expr arg");

    omicron_config_init(&cfg);
    if (omicron_induce_omi_lisp(&cfg)) return fail("induce without preheader");
    if (!omicron_stage_preheader(cfg.dialect, cfg.preheader)) return fail("stage config preheader");
    cfg.flags |= OMICRON_FLAG_PREHEADER_STAGED;
    if (!omicron_induce_omi_lisp(&cfg)) return fail("induce omi-lisp");
    if (cfg.readable_boundary != OMICRON_READABLE_BOUNDARY) return fail("readable boundary");
    if (cfg.dot_operator != OMICRON_DOT_OPERATOR) return fail("dot operator");
    if (!omicron_load_system_objects(&cfg)) return fail("empty system objects ok");
    cfg.omi_object = obj;
    cfg.omi_object_len = sizeof(obj);
    cfg.imo_object = obj;
    cfg.imo_object_len = sizeof(obj);
    cfg.tetragrammatron_object = obj;
    cfg.tetragrammatron_object_len = sizeof(obj);
    cfg.metatron_object = obj;
    cfg.metatron_object_len = sizeof(obj);
    if (!omicron_load_system_objects(&cfg)) return fail("system objects bound");
    if (omicron_boot(&cfg) != 0) return fail("boot scaffold");
    if ((cfg.flags & OMICRON_FLAG_PREHEADER_STAGED) == 0) return fail("boot preheader flag");
    if ((cfg.flags & OMICRON_FLAG_PRELANGUAGE_INDUCED) == 0) return fail("boot prelanguage flag");
    if ((cfg.flags & OMICRON_FLAG_OBJECTS_BOUND) == 0) return fail("boot objects flag");
    if (cfg.readable_boundary != OMICRON_READABLE_BOUNDARY) return fail("boot readable boundary");
    if (cfg.dot_operator != OMICRON_DOT_OPERATOR) return fail("boot dot operator");
    if (memcmp(cfg.preheader, "\xf0\x00\x1c\x1d\x1e\x1f\x20\xf0", OMICRON_PREHEADER_LEN) != 0) return fail("boot config preheader");
    cfg.omi_object = NULL;
    if (omicron_load_system_objects(&cfg)) return fail("mismatched object pair");
    if (omicron_boot(&cfg) != 4) return fail("boot object failure code");
    if ((cfg.flags & OMICRON_FLAG_OBJECTS_BOUND) != 0) return fail("failed boot object flag cleared");

    /*
     * Omicron parser vocabulary guardrail:
     *
     * These tests cover candidate syntax, normalized notation, lowered frames,
     * dialect surfaces, prefix/path parsing, and CAR/CDR closure decoding only.
     *
     * Parser fields must not imply validation, acceptance, receipt creation,
     * proof generation, projection, routing, or receipt-ring storage.
     *
     * Allowed example:
     *   lowered_candidate
     *
     * Forbidden examples:
     *   lowered_valid_candidate
     *   accepted_candidate
     *   receipt_candidate
     *   proof_candidate
     *   projection_candidate
     *
     * Omicron lowers notation candidates.
     * Tetragrammatron validates and accepts.
     * Metatron projects accepted state.
     */
    if (!omicron_parse_address_candidate("omi---imo", &ac)) return fail("parse omi ring");
    if (!ac.has_open || !ac.has_close) return fail("omi ring bounds");
    if (ac.has_frame || ac.has_prefix || ac.path_count != 0) return fail("omi ring empty surface");
    if (!ac.lowered_candidate) return fail("omi ring candidate");

    {
        const uint16_t f[8] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0036,0x0072};
        if (!omicron_parse_address_candidate("omi-0000-0000-0000-0000-0000-0000-0036-0072-imo", &ac)) return fail("parse bounded frame");
        if (!ac.has_open || !ac.has_close || !ac.has_frame) return fail("bounded frame flags");
        if (!eq_frame(ac.frame, f) || !eq_frame(ac.lowered_frame, f)) return fail("bounded frame values");
        if (ac.has_prefix || ac.path_count != 0) return fail("bounded frame no path");
    }

    {
        const uint16_t f[8] = {0x0500,0x03bf,0x000c,0x2b05,0x2f05,0x0002,0x039f,0x05ff};
        if (!omicron_parse_address_candidate("omi-0500-03bf-000c-2b05-2f05-0002-039f-05ff/128/hardware/i2c:0x28/euler-imo", &ac)) return fail("parse derived frame");
        if (!ac.has_prefix || ac.prefix != 128) return fail("derived prefix");
        if (ac.path_count != 4) return fail("derived path count");
        if (strcmp(ac.path[1], "hardware") != 0 || strcmp(ac.path[2], "i2c:0x28") != 0 || strcmp(ac.path[3], "euler") != 0) return fail("derived path values");
        if (!eq_frame(ac.frame, f)) return fail("derived frame values");
    }

    {
        const uint16_t f[8] = {0,0,0,0,0,0,0x7f00,0x0001};
        if (!omicron_parse_address_candidate("ip4:127.0.0.1/32", &ac)) return fail("parse ip4");
        if (!ac.has_frame || !ac.has_prefix || ac.prefix != 32) return fail("ip4 flags");
        if (!eq_frame(ac.lowered_frame, f)) return fail("ip4 lowered");
    }

    {
        const uint16_t f[8] = {0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,0x7f00,0x0001};
        if (!omicron_parse_address_candidate("ip6:0000:0000:0000:0000:0000:ffff:7f00:0001/128", &ac)) return fail("parse ip6");
        if (!ac.has_frame || !ac.has_prefix || ac.prefix != 128) return fail("ip6 flags");
        if (!eq_frame(ac.lowered_frame, f)) return fail("ip6 lowered");
    }

    if (!omicron_parse_address_candidate("omi-0000-0000-0000-0000-0000-0000-0036-0072/128?0?0@3VZ@T01J", &ac)) return fail("parse closure");
    if (!ac.has_payload_mask || ac.payload != 0 || ac.mask != 0) return fail("payload mask");
    if (!ac.has_cons_closure || ac.car36_value != 5039) return fail("car36");
    if (ac.cdr64_len != 3 || ac.cdr64_bytes[0] != 0x4f || ac.cdr64_bytes[1] != 0x4d || ac.cdr64_bytes[2] != 0x49) return fail("cdr64");
    if (omicron_parse_address_candidate("omi-0000-0000-0000-0000-0000-0000-0036-0072/128?0?0@3vz@T01J", &ac)) return fail("invalid base36");
    if (omicron_parse_address_candidate("omi-0000-0000-0000-0000-0000-0000-0036-0072/128?0?0@3VZ@T01!", &ac)) return fail("invalid base64url");
    if (omicron_parse_address_candidate("ip4:ip6:127.0.0.1/32", &ac)) return fail("ambiguous family");

    if (!omilog_parse_candidate("omi-0000-0000-0000-0000-0000-0000-0036-0072/128 MUST project-base36-orbital-symbols", &lc)) return fail("parse omilog head");
    if (!lc.candidate_only) return fail("omilog candidate only");
    if (strcmp(lc.keyword, "MUST") != 0) return fail("omilog keyword");
    if (strcmp(lc.assignment, "project-base36-orbital-symbols") != 0) return fail("omilog assignment");
    if (!lc.address.has_prefix || lc.address.prefix != 128) return fail("omilog prefix");
    if (lc.has_source_block || lc.has_o_expression_body) return fail("omilog no source block");

    {
        const char *log =
            "omi-0000-0000-0000-0000-0000-0000-0036-0072/128?0?0@3VZ@T01J FACT base36-car-cdr-closure-declared\n"
            "\n"
            "omi-\n"
            "  (\n"
            "    (car36 . \"3VZ\")\n"
            "    (cdr64 . \"T01J\")\n"
            "    (closure . \"@3VZ@T01J\")\n"
            "  )\n"
            "-imo";
        if (!omilog_parse_candidate(log, &lc)) return fail("parse omilog block");
        if (!lc.address.has_cons_closure || lc.address.car36_value != 5039) return fail("omilog closure");
        if (!lc.has_source_block || !lc.source_block_start || lc.source_block_len == 0) return fail("omilog block flags");
        if (strncmp(lc.source_block_start, "omi-", 4) != 0) return fail("omilog block start");
        if (!lc.has_o_expression_body) return fail("omilog oexpr");
    }

    if (!omilog_parse_candidate("ip4:127.0.0.1/32 FACT local-surface-present", &lc)) return fail("parse omilog ip4");
    if (!lc.address.has_prefix || lc.address.prefix != 32) return fail("omilog ip4 prefix");
    if (omilog_parse_candidate("omi-0000-0000-0000-0000-0000-0000-0036-0072/128 must lower-case-keyword", &lc)) return fail("omilog lowercase keyword");
    if (omilog_parse_candidate("omi-0000-0000-0000-0000-0000-0000-0036-0072/128 MUST", &lc)) return fail("omilog missing assignment");
    if (omilog_parse_candidate("not-an-address MUST x", &lc)) return fail("omilog invalid address");

    printf("PASS omicron boot\n");
    return 0;
}
