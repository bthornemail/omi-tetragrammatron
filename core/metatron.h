#ifndef METATRON_H
#define METATRON_H

#include <stdint.h>
#include <stddef.h>
#include "tetragrammatron.h"

typedef struct { double x, y, z; } V3;
typedef struct { int v[3]; } F3;
typedef struct { double x, y, z, w; } V4;
typedef uint16_t Citation;
typedef struct { Citation a, b; } OmiRelEdge;

typedef struct {
    const char *name;
    int fano_root;
    int family_seq;
    int nverts, nedges, nfaces;
    const OmiRelEdge *edges;
    int schlafli_p, schlafli_q;
} ShapeDef;

extern const OmiRelEdge SEED_TETRA_E[6];
extern const F3 SEED_TETRA_F[4];
extern const OmiRelEdge SEED_STELLATED_E[12];
extern const OmiRelEdge SEED_OCTA_E[12];
extern const OmiRelEdge SEED_CUBE_E[12];
extern const OmiRelEdge SEED_ICOSA_E[30];
extern const OmiRelEdge SEED_DODEC_E[30];
extern const OmiRelEdge SEED_CUBOCT_E[24];
extern const OmiRelEdge SEED_TRUNCOCT_E[36];
extern const OmiRelEdge SEED_TRUNCCUBE_E[36];
extern const OmiRelEdge SEED_RHOMBICDODEC_E[24];
extern const OmiRelEdge SEED_ICOSIDODEC_E[60];
extern const OmiRelEdge SEED_TRUNCICOSA_E[90];
extern const OmiRelEdge SEED_TRUNCDODEC_E[90];
extern const OmiRelEdge SEED_TRUNCICOSIDODEC_E[180];
extern const OmiRelEdge SEED_TETRAKISHEX_E[36];
extern const OmiRelEdge SEED_TRIAKISOCTA_E[36];
extern const OmiRelEdge SEED_PENTAKISDODEC_E[90];
extern const OmiRelEdge SEED_TRIAKISICOSA_E[90];
extern const OmiRelEdge SEED_DISDYAKIS_E[180];

extern const int8_t T0[4][3];
extern const int8_t T1[4][3];
extern const int8_t OCTA[6][3];
extern const int8_t CUBOCT[12][3];

extern const ShapeDef SHAPE_DB[21];
#define SHAPE_DB_N 21

extern const char *INCIDENCE_WYTHOFF[21];
extern const char *INCIDENCE_CORE[21];
extern const int BRANCH_TERM[7];
extern const char *BQF_BRANCH[7];

typedef struct {
    int solid_idx;
    const ShapeDef *shape;
    int highlight_vertex;
    int highlight_edge;
    double rotate_x, rotate_y, rotate_z;
    double scale;
} SolidRenderState;

typedef struct {
    int chart11, baseQ, fiberQ, fano7, role3;
    uint32_t qxy;
    int local240, slot5040;
    int a, b, c;
    double fiberPhase;
    double qw, qx, qy, qz;
    int polybius_row, polybius_col;
    int frame_type;
    uint64_t cycle;
    uint16_t result;
    uint16_t xf, sf, rf;
    uint16_t opcode;
    SolidRenderState solid;
} TwinGeometry;

typedef struct {
    double gr, gi;
    double zr, zi;
    double yr, yi;
    double rho, theta;
} SmithState;

double tetra_golden_ratio(void);
double tetra_derive_constant(const char *name, int steps);
void resolve_vertex(int shape_idx, int cite, double *x, double *y, double *z);
void cite_to_sphere(int shape_idx, int cite, double *x, double *y, double *z);
const ShapeDef *solid_lookup(int fano7, int role3);
SolidRenderState resolve_solid_geometry(uint16_t xf, uint16_t sf, uint16_t rf, int fano7, int role3, int local240, uint64_t hash);
TwinGeometry resolve_hopf_ququart_route(int chart11, int baseQ, int fiberQ, int fano7, int role3, uint64_t hash);
TwinGeometry tetragrammatron_geometry_route(uint64_t cycle, uint16_t xf, uint16_t sf, uint16_t rf, uint16_t opcode, uint16_t result);
int mod_pos(int x, int m);
double clamp_d(double n, double lo, double hi);
SmithState resolve_smith(uint32_t slot5040, uint64_t hash);
void print_twin_geometry(const TwinGeometry *g);
void render_frame_json(void);
void render_smith_svg(void);
void render_ppm(void);
void render_obj(const TwinGeometry *g);
void render_gltf(void);
void quat_to_rot(double qw, double qx, double qy, double qz, double rot[3][3]);
const ShapeDef *find_solid_with_data(int fano7, int role3);
int check_incidence(void);

#endif
