#ifndef CR_MATH_H
#define CR_MATH_H

typedef struct {

	unsigned int up;
	unsigned int down;
	unsigned int left;
	unsigned int right;

} CR_BOUNDS;

//FUNCTION DECLARATIONS
int CR_SIGN(int var);
int CR_SIGN_NEG(int var);

bool cr_coord_is_within(float pos_x, float pos_y, float x1, float x2, float y1, float y2, bool based_on_camera);
bool cr_coord_is_within_3d(float pos_x, float pos_y, float pos_z, float x1, float x2, float y1, float y2, float z1, float z2, bool based_on_camera);

// VECTOR 2's

typedef struct {
	float x;
	float y;
} vec2f;

typedef struct {
	int x;
	int y;
} vec2i;

typedef struct {
	unsigned int x;
	unsigned int y;
} vec2ui;

typedef struct {
	short x;
	short y;
} vec2s;

typedef struct {
	unsigned short x;
	unsigned short y;
} vec2us;

typedef struct {
	bool x;
	bool y;
} vec2b;

typedef struct {
	char x;
	char y;
} vec2c;

typedef struct {
	unsigned char x;
	unsigned char y;
} vec2uc;

// VECTOR 3's

typedef struct {
	float x;
	float y;
	float z;
} vec3f;

typedef struct {
	int x;
	int y;
	int z;
} vec3i;

typedef struct {
	unsigned int x;
	unsigned int y;
	unsigned int z;
} vec3ui;

typedef struct {
	short x;
	short y;
	short z;
} vec3s;

typedef struct {
	unsigned short x;
	unsigned short y;
	unsigned short z;
} vec3us;

typedef struct {
	bool x;
	bool y;
	bool z;
} vec3b;

typedef struct {
	char x;
	char y;
	char z;
} vec3c;

typedef struct {
	unsigned char x;
	unsigned char y;
	unsigned char z;
} vec3uc;

//PREPROCCESSOR FUNCTIONS

#define CR_ZERO(X)			X ^= X

#define CR_PI				(3.1415927)

#define CR_PI_2				(1.5707963)

#define CR_MOD(A, B)		(A - B * (A / B))

#define CR_FLOOR(A)			(A - (CR_MOD(A, 1)))

#define CR_DIV(A, B)		((A) / (B))

#define CR_ABS(X)			((X) < 0 ? -(X) : (X))

#define CR_FABS(X)			((X) < 0.0f ? -(X) : (X))

#define CR_MIN(A, B)		(((A) < (B)) ? (A) : (B))

#define CR_MAX(A, B)		(((A) > (B)) ? (A) : (B))

#define CR_SWAP_SIGN(X)		(-(X))

#define CR_RAD_TO_DEG(A)	(180.0 * (A) / CR_PI)

#define CR_DEG_TO_RAD(A)	(CR_PI * (A) / 180.0)

#define CR_IS_ODD(A)		((A) & 1)

#define CR_SWAP(A, B)		{ (A) ^= (B); (B) ^= (B); (A) ^= (B); }

#define CR_ARR_SIZE(A)		(sizeof(A) / sizeof(*A))

#define CR_BNEG(A)			((A != 0) ? (1) : (-1))
#define CR_BPOS(A)			((A != 0) ? (-1) : (1))

#define CR_RANGE(T, L, H)	(T <= L) || (T >= H) ? 1 : 0

#define CR_THREEPOINTONEFOURFONEIVENINETWOSIXFIVETHREEFIVEEIGHTNINESEVENNINE	(3.1415927)

//TRIG

extern int cr_cos_lookup_table[360];

static int cr_cos(const int angle) { return (cr_cos_lookup_table[CR_ABS(angle) % 360]); }

static float cr_cosf(const int angle) { return (cr_cos(angle) / 32768.0f); }

extern int cr_sin_lookup_table[360];

static int cr_sin(const int angle)
{
	return (cr_cos(angle - 90));
}

static float cr_sinf(const int angle) { return (cr_sin(angle) / 32768.0f); }

static int cr_tan(const int angle)
{
	return (cr_sin(angle) / cr_cos(angle));
}

static float cr_tanf(const int angle)
{
	return (cr_sinf(angle) / cr_cosf(angle));
}

unsigned long cr_rngseed;
int cr_rand(int lower, int upper);

#endif // !CR_MATH_H
