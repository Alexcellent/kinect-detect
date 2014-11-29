#include <cstdlib>
#include <iostream>
#include "simpleMath.h"

float dot2(float* a, float* b)
{
    return a[0]*b[0] + a[1]*b[1];
};

float dot3(float* a, float* b)
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
};

float dot4(float* a, float* b)
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
};

float* cross3(float* a, float* b)
{
    float* ret = new float[3];
    ret[0] = a[1] * b[2] - a[2] * b[1];
    ret[1] = a[2] * b[0] - a[0] * b[2];
    ret[2] = a[0] * b[1] - a[1] * b[0];
    return ret;
}

float getNorm(float* v)
{
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}


float* normalize(float* v)
{
    float norm = getNorm(v);
    float* ret = new float[3];
    for (int i = 0; i < 3; i++)
        ret[i] = v[i] / norm;

    return ret;
}


void quatmult(float* q1, float* q2, float* r)
{
    float p[4];
    
    p[0]=q1[0]*q2[0]-q1[1]*q2[1]-q1[2]*q2[2]-q1[3]*q2[3];
    p[1]=q1[0]*q2[1]+q1[1]*q2[0]+q1[2]*q2[3]-q1[3]*q2[2];
    p[2]=q1[0]*q2[2]-q1[1]*q2[3]+q1[2]*q2[0]+q1[3]*q2[1];
    p[3]=q1[0]*q2[3]+q1[1]*q2[2]-q1[2]*q2[1]+q1[3]*q2[0];
    
    memcpy(r, p, sizeof(float)*4);
}

void axisToQuat(float* a, float ang, float* q)
{
    q[0]=cos(ang/2.0);
    q[1]=a[0]*sin(ang/2.0);
    q[2]=a[1]*sin(ang/2.0);
    q[3]=a[2]*sin(ang/2.0);
}

void quatToMat(float* q, float* m)
{
    m[0]=1-2*q[2]*q[2]-2*q[3]*q[3];
    m[1]=2*q[1]*q[2]+2*q[3]*q[0];
    m[2]=2*q[1]*q[3]-2*q[2]*q[0];
    m[3]=0.0;
    m[4]=2*q[1]*q[2]-2*q[3]*q[0];
    m[5]=1-2*q[1]*q[1]-2*q[3]*q[3];
    m[6]=2*q[2]*q[3]+2*q[1]*q[0];
    m[7]=0.0;
    m[8]=2*q[1]*q[3]+2*q[2]*q[0];
    m[9]=2*q[2]*q[3]-2*q[1]*q[0];
    m[10]=1-2*q[1]*q[1]-2*q[2]*q[2];
    m[11]=0.0;
    m[12]=m[13]=m[14]=0.0;
    m[15]=1.0;
}

void trans(float* m, float* v, float* r)
{
    float p[16];
    
    for(int i=0; i<16; ++i)
        p[i]=0.0;
    p[0]=p[5]=p[10]=p[15]=1.0;
    
    p[12]=v[0];
    p[13]=v[1];
    p[14]=v[2];
    
    mult(m, p, r);
}

void mult(float* m1, float* m2, float* r)
{
    float p[16];
    
    for(int i=0; i<4; ++i)
    {
        for(int j=0; j<4; ++j)
        {
            p[i*4+j]=0;
            for(int x=0; x<4; ++x)
                p[i*4+j]+=m1[j+4*x]*m2[i*4+x];
        }
    }
    
    memcpy(r, p, sizeof(float)*16);
}

void multv(float *m, float *v, float *r)
{
    float t[3];
    
    t[0]=m[0]*v[0]+m[4]*v[1]+m[8]*v[2]+m[12];
    t[1]=m[1]*v[0]+m[5]*v[1]+m[9]*v[2]+m[13];
    t[2]=m[2]*v[0]+m[6]*v[1]+m[10]*v[2]+m[14];
    
    memcpy(r, t, sizeof(float)*3);
}

void scalar(float s,float* m, float *r)
{
    for(int i=0; i<16; ++i)
        r[i]=m[i]*s;
}

void add(float* s,float* m, float *r)
{
    for(int i=0; i<16; ++i)
        r[i]=m[i]+s[i];
}

void loadIdentity(float *m)
{
    for(int i = 0; i < 16; i++){
        if (i % 5 == 0)
            m[i] = 1.0;
        else
            m[i] = 0.0;
    }
}

void matToAxis(float* mat, float* axis, float* angle)
{   
    // Algorithm adapted from:
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/
    
    // Check if special case 0deg or 180deg
    if ((abs(mat[4] - mat[1]) < 0.01) &&
        (abs(mat[8] - mat[2]) < 0.01) &&
        (abs(mat[9] - mat[6]) < 0.01)) {
        
        // If identity, angle is 0, axis doesn't matter
        if ((abs(mat[4] + mat[1]) == 0) &&
            (abs(mat[8] + mat[2]) == 0) &&
            (abs(mat[9] + mat[6]) == 0) &&
            (mat[0] + mat[5] + mat[10]) == 3) {
            *angle = 0.0;
            axis[0] = 1.0;
            axis[1] = 0.0;
            axis[2] = 0.0;
            return;
        }
        // Else angle is 180
        *angle = M_PI;
        float xx = (mat[0] + 1) / 2.0;
        float yy = (mat[5] + 1) / 2.0;
        float zz = (mat[10] + 1) / 2.0;
        float xy = (mat[4] + mat[1]) / 4.0;
        float xz = (mat[8] + mat[2]) / 4.0;
        float yz = (mat[9] + mat[6]) / 4.0;
        if ((xx > yy) && (xx > zz)) { // m[0] is the largest diagonal term
            if (xx< 0.01) {
                axis[0] = 0;
                axis[1] = 0.7071;
                axis[2] = 0.7071;
            }
            else {
                axis[0] = sqrt(xx);
                axis[1] = xy / axis[0];
                axis[2] = xz / axis[0];
            }
        }
        else if (yy > zz) { // m[5] is the largest diagonal term
            if (yy< 0.01) {
                axis[0] = 0.7071;
                axis[1] = 0;
                axis[2] = 0.7071; 
 
            }
            else {
                axis[1] = sqrt(yy);
                axis[0] = xy / axis[1];
                axis[2] = yz / axis[1];
            }
        }
        else { // m[9] is the largest diagonal term so base result on this
            if (zz< 0.01) {
                axis[0] = 0.7071;
                axis[1] = 0.7071;
                axis[2] = 0; 
            }
            else {
                axis[2] = sqrt(zz);
                axis[0] = xz / axis[2];
                axis[1] = yz / axis[2];
            }
        }
        return;
    }
    
    // No special cases:
    *angle = acos((mat[0] + mat[5] + mat[10] - 1) / 2.0);
    float h = sqrt((mat[6] - mat[9]) * (mat[6] - mat[9]) + (mat[8] - mat[2]) * (mat[8] - mat[2]) + (mat[1] - mat[4]) * (mat[1] - mat[4]));
    axis[0] = (mat[6] - mat[9]) / h;
    axis[1] = (mat[8] - mat[2]) / h;
    axis[2] = (mat[1] - mat[4]) / h;
}
