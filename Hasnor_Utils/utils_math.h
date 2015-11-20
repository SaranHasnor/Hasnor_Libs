#ifndef HASNOR_MATH_IMPORTED
#define HASNOR_MATH_IMPORTED

typedef struct {
	/* Constants */
	float		pi;

	/* Trigonometry */
	float		(*cos)(float);
	float		(*sin)(float);
	float		(*tan)(float);

	float		(*acos)(float);
	float		(*asin)(float);
	float		(*atan)(float);

	float		(*deg2rad)(float degrees);
	float		(*rad2deg)(float radians);

	/* RNG */
	float		(*random)(void);
	float		(*randomInterval)(float min, float max);
	int			(*randomInteger)(int min, int max);

	/* Utility */
	int			(*ceil)(float);
	int			(*floor)(float);
	int			(*round)(float);

	float		(*clamp)(float min, float value, float max);
	int			(*sign)(float);
} _math_functions;

_math_functions Math;

#ifdef HASNOR_INIT
void initMathFunctions(void);
#endif

#endif
