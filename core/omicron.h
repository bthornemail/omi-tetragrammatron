#ifndef OMICRON_H
#define OMICRON_H

#include <stddef.h>
#include <stdint.h>

#define OMICRON_PREHEADER_LEN 8u
#define OMICRON_FLAG_PREHEADER_STAGED 0x00000001u
#define OMICRON_FLAG_PRELANGUAGE_INDUCED 0x00000002u
#define OMICRON_FLAG_OBJECTS_BOUND 0x00000004u

typedef enum {
    OMICRON_MODE_CLI,
    OMICRON_MODE_SOFTWARE_BOOT,
    OMICRON_MODE_HARDWARE_BOOT,
    OMICRON_MODE_EMBEDDED,
    OMICRON_MODE_MODULE,
    OMICRON_MODE_BITBOARD
} OmicronMode;

typedef enum {
    OMICRON_COMMAND_DEFAULT,
    OMICRON_COMMAND_EVAL,
    OMICRON_COMMAND_REPL,
    OMICRON_COMMAND_RING,
    OMICRON_COMMAND_BOOT,
    OMICRON_COMMAND_AUTO,
    OMICRON_COMMAND_RUN,
    OMICRON_COMMAND_CPU,
    OMICRON_COMMAND_GEOM,
    OMICRON_COMMAND_RENDER_FRAME,
    OMICRON_COMMAND_RENDER_PPM,
    OMICRON_COMMAND_SCRIBE,
    OMICRON_COMMAND_RENDER_OBJ,
    OMICRON_COMMAND_RENDER_GLTF,
    OMICRON_COMMAND_SMITH,
    OMICRON_COMMAND_TWIN,
    OMICRON_COMMAND_SEED,
    OMICRON_COMMAND_SERVE,
    OMICRON_COMMAND_CHECK,
    OMICRON_COMMAND_HELP,
    OMICRON_COMMAND_WATCH,
    OMICRON_COMMAND_EXPR
} OmicronCommand;

typedef enum {
    OMICRON_DIALECT_OMICRON = 0xf0u,
    OMICRON_DIALECT_CANONICAL = 0xffu
} OmicronDialect;

typedef struct {
    OmicronMode mode;
    OmicronCommand command;
    OmicronDialect dialect;
    uint32_t flags;
    uint64_t bitboard;
    uint8_t preheader[OMICRON_PREHEADER_LEN];
    int argc;
    char **argv;
    const char *command_arg;
    const char *command_arg2;
    const void *omi_object;
    size_t omi_object_len;
    const void *imo_object;
    size_t imo_object_len;
    const void *tetragrammatron_object;
    size_t tetragrammatron_object_len;
    const void *metatron_object;
    size_t metatron_object_len;
} OmicronConfig;

void omicron_config_init(OmicronConfig *cfg);
int omicron_config_from_cli(OmicronConfig *cfg, int argc, char **argv);
int omicron_boot(OmicronConfig *cfg);
int omicron_stage_preheader(OmicronDialect dialect, uint8_t out[OMICRON_PREHEADER_LEN]);
int omicron_induce_omi_lisp(const OmicronConfig *cfg);
int omicron_load_system_objects(const OmicronConfig *cfg);
const char *omicron_mode_name(OmicronMode mode);
const char *omicron_command_name(OmicronCommand command);
const char *omicron_dialect_name(OmicronDialect dialect);

#endif
