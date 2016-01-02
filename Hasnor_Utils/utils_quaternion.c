#include "utils_quaternion.h"

#define _USE_MATH_DEFINES
#include <math.h>

static float _workQuat[4];

void quaternion_fromEuler(float out[4], float pitch, float yaw, float roll)
{ // Yaw and roll are swapped in our axis
	float cp, cy, cr;
	float sp, sy, sr;

	cp = cosf(pitch/2.0f);
	cy = cosf(roll/2.0f);
	cr = cosf(yaw/2.0f);

	sp = sinf(pitch/2.0f);
	sy = sinf(roll/2.0f);
	sr = sinf(yaw/2.0f);

	out[0] = cp * cy * cr + sp * sy * sr;
	out[1] = sp * cy * cr - cp * sy * sr;
	out[2] = cp * sy * cr + sp * cy * sr;
	out[3] = cp * cy * sr - sp * sy * cr;
}

void quaternion_identity(float out[4])
{
	quaternion_fromEuler(out, 0.0f, 0.0f, 0.0f);
}

void quaternion_multiply(float out[4], const float a[4], const float b[4])
{
	_workQuat[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
	_workQuat[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
	_workQuat[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1];
	_workQuat[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0];

	out[0] = _workQuat[0];
	out[1] = _workQuat[1];
	out[2] = _workQuat[2];
	out[3] = _workQuat[3];
}

void quaternion_inverse(float out[4], const float src[4])
{
	const float norm = sqrtf(src[0] * src[0] + src[1] * src[1] + src[2] * src[2] + src[3] * src[3]);
	out[0] = -src[0] / norm;
	out[1] = -src[1] / norm;
	out[2] = -src[2] / norm;
	out[3] =  src[3] / norm;
}

void initQuaternionFunctions(void)
{
	Quaternion.fromEuler = quaternion_fromEuler;
	Quaternion.identity = quaternion_identity;
	Quaternion.multiply = quaternion_multiply;
	Quaternion.inverse = quaternion_inverse;
}
