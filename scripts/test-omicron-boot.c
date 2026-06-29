#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "omicron.h"

static int fail(const char *m) {
    fprintf(stderr, "FAIL: %s\n", m);
    return 1;
}

int main(void) {
    OmicronConfig cfg;
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
    if (!omicron_induce_omi_lisp(&cfg)) return fail("induce omi-lisp");
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
    if (memcmp(cfg.preheader, "\xf0\x00\x1c\x1d\x1e\x1f\x20\xf0", OMICRON_PREHEADER_LEN) != 0) return fail("boot config preheader");
    cfg.omi_object = NULL;
    if (omicron_load_system_objects(&cfg)) return fail("mismatched object pair");
    if (omicron_boot(&cfg) != 4) return fail("boot object failure code");
    if ((cfg.flags & OMICRON_FLAG_OBJECTS_BOUND) != 0) return fail("failed boot object flag cleared");

    printf("PASS omicron boot\n");
    return 0;
}
