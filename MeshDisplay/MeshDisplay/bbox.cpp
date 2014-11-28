#include "bbox.h"
#include <math.h>

void BBox::makeBBox(PLYmodel* pmodel)
{
    // Find meshes bounding box
    xMin = yMin = zMin =  FLT_MAX;
    xMax = yMax = zMax = -FLT_MAX;

    for (unsigned i = 0; i < pmodel->TotalConnectedPoints; i++){
        float v_x = pmodel->Vertex_Buffer[0 + (i * 3)];
        float v_y = pmodel->Vertex_Buffer[1 + (i * 3)];
        float v_z = pmodel->Vertex_Buffer[2 + (i * 3)];

        if (v_x < xMin) xMin = v_x;
        if (v_y < yMin) yMin = v_y;
        if (v_z < zMin) zMin = v_z;

        if (v_x > xMax) xMax = v_x;
        if (v_y > yMax) yMax = v_y;
        if (v_z > zMax) zMax = v_z;
    }

    width  = abs(xMax - xMin);
    height = abs(yMax - yMin);
    depth  = abs(zMax - zMin);

    xCen = (xMin + xMax) / 2.0;
    yCen = (yMin + yMax) / 2.0;
    zCen = (zMin + zMax) / 2.0;

    //std::cout << "Bounding box:\n"
    //std::cout << "xMin: " << xMin << " yMin: " << yMin << " zMin: " << zMin << std::endl
    //          << "xMax: " << xMax << " yMax: " << yMax << " zMax: " << zMax << std::endl
    //          << "xCen: " << xCen << " yCen: " << yCen << " zCen: " << zCen << std::endl;
}


float BBox::getRadius()
{
    float radius = sqrt(((xMax - xMin)/2.0)*((xMax - xMin)/2.0) + 
                        ((yMax - yMin)/2.0)*((yMax - yMin)/2.0) + 
                        ((zMax - zMin)/2.0)*((zMax - zMin)/2.0));
    return radius;
}
void BBox::getScreenSphere(GLdouble* x, GLdouble* y, GLdouble* z, GLdouble* radius)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLdouble winX, winY, winZ;

    

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    gluProject((GLdouble)xCen, (GLdouble)yCen, (GLdouble)zCen,
               modelview, projection, viewport, x, y, z);
}

void BBox::draw(){
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.8, 0.8, 0.8, 0.4); // should be completely invisible

    glTranslated(xCen, yCen, zCen);
    glutSolidSphere(getRadius(), 30, 30);
    glTranslated(-xCen, -yCen, -zCen);


    glDisable(GL_BLEND);
}