#ifndef TETRAGRAMMATRON_H
#define TETRAGRAMMATRON_H

#include <stdint.h>
#include <stddef.h>
#include "omi.h"

#define RING_SIZE 5040u
#define MAX_RECEIPT 4096

typedef struct {
    uint64_t cycle;
    char receipt[MAX_RECEIPT];
    uint64_t hash;
    uint16_t result;
} RingSlot;

typedef struct {
    uint64_t citation_hash;
    uint16_t result;
    uint32_t slot5040;
    uint8_t accepted;
} TetragrammatronAcceptedState;

extern RingSlot ring[RING_SIZE];
extern uint64_t g_cycle;
extern volatile int g_running;

extern const uint8_t FANO_LINES[7][3];
extern const uint8_t FANO_PT_LINES[7][3];
extern const uint8_t CONFIG_MATRIX[7];
#define CONFIG_FAMILY_SIZE 3

extern const unsigned char QUQUART_PHASE[4];

/* Polybius 5x5 */
int polybius_origin_row(void);
int polybius_origin_col(void);
int polybius_get_low_ququart(int idx, int *row, int *col);
int polybius_get_high_ququart(int idx, int *row, int *col);
int polybius_get_interior(int x, int y, int *row, int *col);
int polybius_is_rail(int row, int col);
int polybius_is_interior(int row, int col);

/* Slot computation */
uint32_t compute_slot5040(int fano7, int role3, int local240);

/* Ring logical operations (pure memory) */
size_t ring_idx(void);
void ring_store(uint64_t h, const char *receipt_str);
void ring_dump(void);
uint16_t ring_xor_fold(void);
uint16_t ring_sum_fold(void);
uint16_t ring_rot_fold(void);
int ring_has_receipt(uint16_t val);
int count_filled_ring_slots(void);

/* Export/import — pure memory, no file I/O */
size_t tetragrammatron_export(RingSlot *out, size_t max);
void tetragrammatron_import(const RingSlot *in, size_t count);

int tetragrammatron_validate_citation_candidate(const OmiCitationCandidate *candidate, TetragrammatronAcceptedState *out);
int tetragrammatron_store_accepted_state(const TetragrammatronAcceptedState *state);

/* Chiral diagonal law */
typedef enum { CHIRAL_BALANCED=0, CHIRAL_POSTING=1, CHIRAL_PULLING=-1, CHIRAL_INCOMPLETE=-2 } ChiralDir;
ChiralDir tetra_chiral_pi(uint64_t hash);

#endif
