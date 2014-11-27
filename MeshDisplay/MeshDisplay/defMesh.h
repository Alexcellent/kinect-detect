
#ifndef MESH_H
#define MESH_H
#include "plyModel.h"
#include "GL/glut.h"
#include <string>
#include <limits>
#include <iostream>

// TODO:
// - Add center of mass
// - Add boolean isPicked
// - Transform all points on mouse-movement

struct BBox{
    float xMin, yMin, zMin, 
          xMax, yMax, zMax,
          xCen, yCen, zCen;
    
    void draw(){
        glTranslated(xMin, yMin, zMin);
        glutSolidSphere(0.01, 15, 15);
        glTranslated(-xMin, -yMin, -zMin);
        
        glTranslated(xMax, yMin, zMin);
        glutSolidSphere(0.01, 15, 15);
        glTranslated(-xMax, -yMin, -zMin);

        glTranslated(xMax, yMin, zMax);
        glutSolidSphere(0.01, 15, 15);
        glTranslated(-xMax, -yMin, -zMax);

        glTranslated(xMin, yMin, zMax);
        glutSolidSphere(0.01, 15, 15);
        glTranslated(-xMin, -yMin, -zMax);
        
        glTranslated(xMin, yMax, zMin);
        glutSolidSphere(0.01, 15, 15);
        glTranslated(-xMin, -yMax, -zMin);

        glTranslated(xMax, yMax, zMin);
        glutSolidSphere(0.01, 15, 15);
        glTranslated(-xMax, -yMax, -zMin);

        glTranslated(xMax, yMax, zMax);
        glutSolidSphere(0.01, 15, 15);
        glTranslated(-xMax, -yMax, -zMax);

        glTranslated(xMin, yMax, zMax);
        glutSolidSphere(0.01, 15, 15);
        glTranslated(-xMin, -yMax, -zMax);

        glTranslated(xCen, yCen, zCen);
        glutSolidSphere(0.01, 15, 15);
        glTranslated(-xCen, -yCen, -zCen);


    }
};

class DefMesh
{
public:
    PLYmodel *pmodel;
    GLuint    mode;
    BBox      bbox;
    bool      isSelected;

    DefMesh(std::string filename);
    ~DefMesh() { delete pmodel; }

    void glDraw(int type);

    void makeBBox();
};
#endif
