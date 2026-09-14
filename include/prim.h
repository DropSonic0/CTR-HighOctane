#ifndef PRIM_H
#define PRIM_H

#include <macros.h>

enum VertexCount
{
	VertexCount_Point = 1,
	VertexCount_Line = 2,
	VertexCount_Tri = 3,
	VertexCount_Quad = 4,
};

typedef union Tag
{
	struct
	{
#if defined(__PS3__) || defined(__CELLOS_LV2__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
		u32 size : 8;
		u32 addr : 24;
#else
		u32 addr : 24;
		u32 size : 8;
#endif
	};
	u32 self;
} Tag;

/* Primitives */

typedef union Texpage
{
	struct
	{
#if defined(__PS3__) || defined(__CELLOS_LV2__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
		u32 code : 8; /* 0xE1 */
		u32 unused : 10;
		u32 texFlipY : 1;         /* bool */
		u32 texFlipX : 1;         /* bool */
		u32 y_VRAM_EXP : 1;       /* ununsed in retail */
		u32 drawDisplayArea : 1;  /* bool */
		u32 dither : 1;           /* (0=Off/strip LSBs, 1=24bit to 15bit Dither Enabled) */
		u32 texpageColors : 2;    /* (0=4bit, 1=8bit, 2=15bit, 3=Reserved) */
		u32 semiTransparency : 2; /* (0=B/2+F/2, 1=B+F, 2=B-F, 3=B+F/4) */
		u32 y : 1;                /* y * 256 */
		u32 x : 4;                /* x * 64 */
#else
		u32 x : 4;                /* x * 64 */
		u32 y : 1;                /* y * 256 */
		u32 semiTransparency : 2; /* (0=B/2+F/2, 1=B+F, 2=B-F, 3=B+F/4) */
		u32 texpageColors : 2;    /* (0=4bit, 1=8bit, 2=15bit, 3=Reserved) */
		u32 dither : 1;           /* (0=Off/strip LSBs, 1=24bit to 15bit Dither Enabled) */
		u32 drawDisplayArea : 1;  /* bool */
		u32 y_VRAM_EXP : 1;       /* ununsed in retail */
		u32 texFlipX : 1;         /* bool */
		u32 texFlipY : 1;         /* bool */
		u32 unused : 10;
		u32 code : 8; /* 0xE1 */
#endif
	};
	u32 self;
} Texpage;

enum TexpageColorMode
{
	TEXPAGE_COLOR_4BIT = 0,
	TEXPAGE_COLOR_8BIT = 1,
	TEXPAGE_COLOR_15BIT = 2,
};

typedef struct
{
	Tag tag;
	Texpage texpage;
} TPage;

enum RenderCode
{
	RenderCode_Polygon = 1,
	RenderCode_Line = 2,
	RenderCode_Rectangle = 3,
};

typedef union PrimCode
{
	union
	{
		struct
		{
#if defined(__PS3__) || defined(__CELLOS_LV2__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
			u8 renderCode : 3;       /* enum RenderCode */
			u8 gouraud : 1;          /* gouraud / flat shading */
			u8 quad : 1;             /* 4 / 3 vertices */
			u8 textured : 1;         /* textured / untextured */
			u8 semiTransparency : 1; /* semi-transparent / opaque */
			u8 rawTex : 1;           /* raw texture / modulation */
#else
			u8 rawTex : 1;           /* raw texture / modulation */
			u8 semiTransparency : 1; /* semi-transparent / opaque */
			u8 textured : 1;         /* textured / untextured */
			u8 quad : 1;             /* 4 / 3 vertices */
			u8 gouraud : 1;          /* gouraud / flat shading */
			u8 renderCode : 3;       /* enum RenderCode */
#endif
		} poly;
		struct
		{
#if defined(__PS3__) || defined(__CELLOS_LV2__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
			u8 renderCode : 3;       /* enum RenderCode */
			u8 gouraud : 1;          /* gouraud / flat shading */
			u8 polyline : 1;         /* polyline / single line */
			u8 unused2 : 1;          /* textured / untextured */
			u8 semiTransparency : 1; /* semi-transparent / opaque */
			u8 unused : 1;           /* raw texture / modulation */
#else
			u8 unused : 1;           /* raw texture / modulation */
			u8 semiTransparency : 1; /* semi-transparent / opaque */
			u8 unused2 : 1;          /* textured / untextured */
			u8 polyline : 1;         /* polyline / single line */
			u8 gouraud : 1;          /* gouraud / flat shading */
			u8 renderCode : 3;       /* enum RenderCode */
#endif
		} line;
		struct
		{
#if defined(__PS3__) || defined(__CELLOS_LV2__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
			u8 renderCode : 3;       /* enum RenderCode */
			u8 rectSize : 2;         /* rect size */
			u8 textured : 1;         /* textured / untextured */
			u8 semiTransparency : 1; /* semi-transparent / opaque */
			u8 rawTex : 1;           /* raw texture / modulation */
#else
			u8 rawTex : 1;           /* raw texture / modulation */
			u8 semiTransparency : 1; /* semi-transparent / opaque */
			u8 textured : 1;         /* textured / untextured */
			u8 rectSize : 2;         /* rect size */
			u8 renderCode : 3;       /* enum RenderCode */
#endif
		} rect;
	};
	u8 code;
} PrimCode;

typedef union ColorCode
{
	struct
	{
		u8 r;
		u8 g;
		u8 b;
		PrimCode code;
	};
	u32 self;
} ColorCode;

typedef ColorCode Color;

typedef union Point
{
	struct
	{
		s16 x;
		s16 y;
	};
	s32 self;
} Point;

#if defined(__PPU__) || defined(__PS3__) || defined(__CELLOS_LV2__) || defined(__SNC__)
static inline ColorCode MakeColorCode(u8 red, u8 green, u8 blue, PrimCode renderCode)
{
	ColorCode c;
	c.r = red;
	c.g = green;
	c.b = blue;
	c.code = renderCode;
	return c;
}

static inline Color MakeColor(u8 red, u8 green, u8 blue)
{
	Color c;
	c.r = red;
	c.g = green;
	c.b = blue;
	c.code.code = 0;
	return c;
}

static inline Point MakePoint(s16 px, s16 py)
{
	Point p;
	p.x = px;
	p.y = py;
	return p;
}
#else
#define MakeColorCode(red, green, blue, renderCode)         \
	(ColorCode)                                             \
	{                                                       \
		.r = red, .g = green, .b = blue, .code = renderCode \
	}
#define MakeColor(red, green, blue)     \
	(Color)                             \
	{                                   \
		.r = red, .g = green, .b = blue \
	}
#define MakePoint(px, py) \
	(Point)               \
	{                     \
		.x = px, .y = py  \
	}
#endif

typedef union UV
{
	struct
	{
		u8 u;
		u8 v;
	};
	u16 self;
} UV;

typedef union PolyTexpage
{
	struct
	{
#if defined(__PS3__) || defined(__CELLOS_LV2__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
		u16 nop : 2;
		u16 unused2 : 2;
		u16 y_VRAM_EXP : 1; /* ununsed in retail */
		u16 unused : 2;
		u16 texpageColors : 2;    /* (0=4bit, 1=8bit, 2=15bit, 3=Reserved) */
		u16 semiTransparency : 2; /* (0=B/2+F/2, 1=B+F, 2=B-F, 3=B+F/4) */
		u16 y : 1;                /* y * 256 */
		u16 x : 4;                /* x * 64 */
#else
		u16 x : 4;                /* x * 64 */
		u16 y : 1;                /* y * 256 */
		u16 semiTransparency : 2; /* (0=B/2+F/2, 1=B+F, 2=B-F, 3=B+F/4) */
		u16 texpageColors : 2;    /* (0=4bit, 1=8bit, 2=15bit, 3=Reserved) */
		u16 unused : 2;
		u16 y_VRAM_EXP : 1; /* ununsed in retail */
		u16 unused2 : 2;
		u16 nop : 2;
#endif
	};
	u16 self;
} PolyTexpage;

typedef union CLUT
{
	struct
	{
#if defined(__PS3__) || defined(__CELLOS_LV2__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
		u16 nop : 1; /* Should be 0 */
		u16 y : 9;   /* 0-511 (ie. in 1-line steps) */
		u16 x : 6;   /* X/16  (ie. in 16-halfword steps) */
#else
		u16 x : 6;   /* X/16  (ie. in 16-halfword steps) */
		u16 y : 9;   /* 0-511 (ie. in 1-line steps) */
		u16 nop : 1; /* Should be 0 */
#endif
	};
	u16 self;
} CLUT;

/* Vertex types */

typedef struct FVertex
{
	Point pos;
} FVertex;

typedef struct GVertex
{
	ColorCode color;
	Point pos;
} GVertex;

typedef struct FTVertex
{
	Point pos;
	UV texCoords;
	union
	{
		CLUT clut;
		PolyTexpage tpage;
	};
} FTVertex;

typedef struct GTVertex
{
	ColorCode color;
	Point pos;
	UV texCoords;
	union
	{
		CLUT clut;
		PolyTexpage tpage;
	};
} GTVertex;

/* Primitive types */

typedef struct LineG2
{
	Tag tag;
	GVertex v[VertexCount_Line];
} LineG2;

typedef struct PolyG3
{
	Tag tag;
	GVertex v[VertexCount_Tri];
} PolyG3;

typedef struct PolyG4
{
	Tag tag;
	GVertex v[VertexCount_Quad];
} PolyG4;

typedef struct PolyGT3
{
	Tag tag;
	GTVertex v[VertexCount_Tri];
} PolyGT3;

typedef struct PolyGT4
{
	Tag tag;
	GTVertex v[VertexCount_Quad];
} PolyGT4;

typedef struct LineF2
{
	Tag tag;
	ColorCode colorCode;
	FVertex v[VertexCount_Line];
} LineF2;

typedef struct LineF3
{
	Tag tag;
	ColorCode colorCode;
	FVertex v[VertexCount_Tri];
	u32 end;
} LineF3;

typedef struct PolyF3
{
	Tag tag;
	ColorCode colorCode;
	FVertex v[VertexCount_Tri];
} PolyF3;

typedef struct PolyF4
{
	Tag tag;
	ColorCode colorCode;
	FVertex v[VertexCount_Quad];
} PolyF4;

typedef struct PolyFT3
{
	Tag tag;
	ColorCode colorCode;
	FTVertex v[VertexCount_Tri];
} PolyFT3;

typedef struct PolyFT4
{
	Tag tag;
	ColorCode colorCode;
	FTVertex v[VertexCount_Quad];
} PolyFT4;

#define fPolyCode colorCode.code
#define gPolyCode v[0].color.code
#define polyClut  v[0].clut
#define polyTpage v[1].tpage

void GetPrimitiveMem(void **ppPrim, size_t primSize);
void AddPrimitive(void *pPrim, void *pOt);

#define GetPrimMem(p)                            \
	{                                            \
		void *primMem;                           \
		GetPrimitiveMem(&primMem, sizeof(*(p))); \
		(p) = primMem;                           \
	}

#endif
