#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tetragrammatron.h"

RingSlot ring[RING_SIZE];
uint64_t g_cycle = 0;
volatile int g_running = 1;

const uint8_t FANO_LINES[7][3] = {
    {0,1,2},{0,3,4},{0,5,6},{1,3,5},{1,4,6},{2,3,6},{2,4,5}
};
const uint8_t FANO_PT_LINES[7][3] = {
    {0,1,2},{0,3,4},{0,5,6},{1,3,5},{1,4,6},{2,3,6},{2,4,5}
};
const uint8_t CONFIG_MATRIX[7] = {0, 3, 6, 9, 12, 15, 18};
const unsigned char QUQUART_PHASE[4] = {OMI_US, OMI_GS, OMI_RS, OMI_FS};

int polybius_origin_row(void) { return 1; }
int polybius_origin_col(void) { return 1; }
int polybius_get_low_ququart(int idx, int *row, int *col) { if(idx<0||idx>3)return 0; *row=1; *col=idx+2; return 1; }
int polybius_get_high_ququart(int idx, int *row, int *col) { if(idx<0||idx>3)return 0; *row=idx+2; *col=1; return 1; }
int polybius_get_interior(int x, int y, int *row, int *col) { if(x<0||x>3||y<0||y>3)return 0; *row=y+2; *col=x+2; return 1; }
int polybius_is_rail(int row, int col) { if(row<1||row>5||col<1||col>5)return 0; if(row==1&&col==1)return 0; return row==1||col==1; }
int polybius_is_interior(int row, int col) { if(row<1||row>5||col<1||col>5)return 0; if(row==1&&col==1)return 0; return row>1&&col>1; }

uint32_t compute_slot5040(int fano7, int role3, int local240) {
    return (uint32_t)(((fano7%7)*720u) + ((role3%3)*240u) + (uint32_t)(local240%240));
}

size_t ring_idx(void) { return g_cycle % RING_SIZE; }

void ring_store(uint64_t h, const char *receipt_str) {
    size_t idx=ring_idx(); ring[idx].cycle=g_cycle; ring[idx].hash=h;
    memset(ring[idx].receipt,0,MAX_RECEIPT); strncpy(ring[idx].receipt,receipt_str,MAX_RECEIPT-1);
}

void ring_dump(void) {
    for(size_t i=0;i<RING_SIZE;i++){if(ring[i].hash!=0||ring[i].receipt[0])printf("RING[%zu] cyc=%llu h=0x%016llx %s\n",i,(unsigned long long)ring[i].cycle,(unsigned long long)ring[i].hash,ring[i].receipt);}
}

uint16_t ring_xor_fold(void) {
    uint16_t fold=0; for(size_t i=0;i<RING_SIZE;i++) fold=u16(fold ^ (uint16_t)(ring[i].hash & 0xFFFF)); return fold;
}
uint16_t ring_sum_fold(void) {
    uint16_t fold=0; for(size_t i=0;i<RING_SIZE;i++) fold=u16(fold + (uint16_t)(ring[i].hash & 0xFFFF)); return fold;
}
uint16_t ring_rot_fold(void) {
    uint16_t fold=0; for(size_t i=0;i<RING_SIZE;i++) fold=u16(fold ^ rotl16((uint16_t)(ring[i].hash&0xFFFF),(uint16_t)(i&15))); return fold;
}

int ring_has_receipt(uint16_t val) {
    for(size_t i=0;i<RING_SIZE;i++) if((uint16_t)(ring[i].hash&0xFFFF)==val && ring[i].hash!=0) return 1; return 0;
}

int count_filled_ring_slots(void) {
    int n = 0;
    for (size_t i = 0; i < RING_SIZE; i++) { if (ring[i].hash != 0 || ring[i].receipt[0]) n++; }
    return n;
}

size_t tetragrammatron_export(RingSlot *out, size_t max) {
    size_t n = RING_SIZE < max ? RING_SIZE : max;
    memcpy(out, ring, n * sizeof(RingSlot));
    return n;
}

void tetragrammatron_import(const RingSlot *in, size_t count) {
    size_t n = count < RING_SIZE ? count : RING_SIZE;
    memcpy(ring, in, n * sizeof(RingSlot));
}

int tetragrammatron_validate_citation_candidate(const OmiCitationCandidate *candidate, TetragrammatronAcceptedState *out) {
    uint16_t r;
    int fano7, role3, local240;
    if(!candidate||!out)return 0;
    memset(out,0,sizeof(*out));
    if(!candidate->candidate_only||candidate->citation_hash==0||!candidate->citation_text[0])return 0;
    if(!candidate->has_frame&&!candidate->has_cons_closure)return 0;
    r=(uint16_t)((candidate->citation_hash ^ (candidate->citation_hash >> 16) ^ (candidate->citation_hash >> 32) ^ (candidate->citation_hash >> 48)) & 0xffffu);
    fano7=(int)(candidate->citation_hash % 7u);
    role3=(int)((candidate->citation_hash >> 8) % 3u);
    local240=(int)(r % 240u);
    out->citation_hash=candidate->citation_hash;
    out->result=r;
    out->slot5040=compute_slot5040(fano7,role3,local240);
    out->accepted=1;
    return 1;
}

int tetragrammatron_store_accepted_state(const TetragrammatronAcceptedState *state) {
    char receipt[128];
    size_t idx;
    if(!state||!state->accepted||state->citation_hash==0)return 0;
    idx=ring_idx();
    snprintf(receipt,sizeof(receipt),"accepted-state;slot5040=%u;result=0x%04x",(unsigned)state->slot5040,(unsigned)state->result);
    ring_store(state->citation_hash,receipt);
    ring[idx].result=state->result;
    return 1;
}

ChiralDir tetra_chiral_pi(uint64_t hash) {
    int dplus = 0, dminus = 0, dp = -1, dm = -1;
    for (int i = 0; i < 16; i++) {
        int n = (int)((hash >> (60 - 4*i)) & 0xF);
        switch (n) {
            case 0x0: dplus += 0;  if (dp < 0 && dplus >= 30) dp = i; break;
            case 0x5: dplus += 5;  if (dp < 0 && dplus >= 30) dp = i; break;
            case 0xA: dplus += 10; if (dp < 0 && dplus >= 30) dp = i; break;
            case 0xF: dplus += 15; if (dp < 0 && dplus >= 30) dp = i; break;
            case 0x3: dminus += 3; if (dm < 0 && dminus >= 30) dm = i; break;
            case 0x6: dminus += 6; if (dm < 0 && dminus >= 30) dm = i; break;
            case 0x9: dminus += 9; if (dm < 0 && dminus >= 30) dm = i; break;
            case 0xC: dminus += 12; if (dm < 0 && dminus >= 30) dm = i; break;
        }
    }
    if (dp >= 0 && (dm < 0 || dp < dm)) return CHIRAL_POSTING;
    if (dm >= 0 && (dp < 0 || dm < dp)) return CHIRAL_PULLING;
    if (dp >= 0 && dm >= 0 && dp == dm) return CHIRAL_BALANCED;
    return CHIRAL_INCOMPLETE;
}
