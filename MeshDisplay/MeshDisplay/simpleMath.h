#ifndef SIMPLEMATH_H
#define SIMPLEMATH_H

#include <math.h>
#include <cstring>

#define M_PI 3.14159265

// Vector operations
float  dot2     (float* a, float* b);
float  dot3     (float* a, float* b);
float  dot4     (float* a, float* b);
float* cross3   (float* a, float* b);
float  getNorm  (float* v);
float* normalize(float* v);

// Transformations
void trans (float* m, float* v, float* r);
void mult  (float* m1, float* m2, float* r);
void mult  (float* m1, float* m2, float* r);
void multv (float *m, float *v, float *r);
void multp(float *m, float *p, float *r);
void scalar(float s, float* m, float *r);
void add   (float *s, float*m, float *r);

// Load ID Matrix
void loadIdentity(float *m);

//// Quaternions
void quatmult  (float* q1, float* q2, float* r);
void axisToQuat(float* a, float ang, float* q);
void quatToMat (float* q, float* m);

// Conversions to axis-angle
void matToAxis(float* mat, float* axis, float* angle);
#endif




