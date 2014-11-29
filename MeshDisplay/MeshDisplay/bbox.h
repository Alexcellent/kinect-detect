#ifndef BBOX_H
#define BBOX_H

#include <GL/glut.h>
#include "simpleMath.h"
#include "plyModel.h"

// Mesh bounding box used for mesh selection
struct BBox{
    
    float xMin, yMin, zMin, // Min Point
          xMax, yMax, zMax, // Max Point
          xCen, yCen, zCen; // Box Center

    float width, height, depth;

    void  makeBBox(PLYmodel* pmodel);
    float getRadius();
    void  getScreenSphere(GLdouble* x, GLdouble* y, GLdouble* z, GLdouble* radius);
    void  draw();
};

#endif