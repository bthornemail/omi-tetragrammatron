#ifndef IMO_H
#define IMO_H

#include <stdint.h>
#include <stddef.h>
#include "tetragrammatron.h"

/* ─── Ring Serialization (IMO owns disk) ─── */
void ring_save(void);
void ring_load(void);
int write_ring_omi(const char *path);

/* ─── S-expression types ─── */
typedef enum { NODE_ATOM, NODE_STRING, NODE_LIST, NODE_PAIR } NodeKind;

typedef struct Node Node;
struct Node {
    NodeKind kind;
    char *text;
    Node **items;
    size_t count, cap;
    Node *car, *cdr;
};

typedef struct { char *data; size_t len, cap; } Buffer;
typedef struct { unsigned char lane; char *value; } Event;
typedef struct { Event *items; size_t len, cap; } EventList;

typedef struct {
    const char *src;
    size_t len, pos;
    int line, col;
    char error[256];
} Parser;

typedef struct {
    Node *root;
    char *normalized;
    char shape[16];
    unsigned char state;
    EventList events;
    Buffer tape;
    uint64_t source_hash, normalized_hash, tape_hash;
} Compilation;

typedef struct { char *receipt; char *result_canon; char *normalized; char shape[16]; size_t events; int accepted; } SxResult;

/* ─── Node operations ─── */
Node *nnew(NodeKind k);
Node *natom(const char *t);
Node *nstring(const char *t);
Node *npair(Node *a, Node *d);
void npush(Node *l, Node *item);
void nfree(Node *n);
Node *ncopy(Node *n);

/* ─── Parser ─── */
Node *parse_sexpr(const char *src, char *err, size_t errlen);

/* ─── Buffer operations ─── */
void breserve(Buffer *b, size_t extra);
void bputc(Buffer *b, unsigned char c);
void bappend(Buffer *b, const char *s);
void bappend_bytes(Buffer *b, const unsigned char *d, size_t n);
void bfree(Buffer *b);

/* ─── S-expression / OMI compilation ─── */
void jesc(Buffer *b, const char *s);
void ncanon(Node *n, Buffer *b);
int compile_omi(const char *src, Compilation *c, char *err, size_t errlen);
void cfree(Compilation *c);
Node *reduce(Node *expr);
char *generate_receipt(const char *src, const char *result, uint64_t cycle);
SxResult process_sexpr(const char *line, uint64_t cycle);
void sx_free(SxResult *pr);

/* ─── Utility ─── */
void die(const char *msg);
void *xmalloc(size_t n);
void *xrealloc(void *p, size_t n);
char *xstrdup(const char *s);
char *xstrndup(const char *s, size_t n);
void handle_signal(int sig);

/* ─── SHA-1 / base64 ─── */
void sha1(const unsigned char *msg, size_t len, unsigned char out[20]);
void b64_encode(const unsigned char *in, size_t len, char *out);

/* ─── HTTP server ─── */
void serve_http(int port);

/* ─── Boot ROM ─── */
extern const char *BOOT_ROM[];
extern const int BOOT_COUNT;

#endif
