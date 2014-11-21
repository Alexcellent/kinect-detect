
#ifndef MESH_H
#define MESH_H
#include "plyModel.h"
#include <string>

// TODO:
// - Add center of mass
// - Add boolean isPicked
// - Transform all points on mouse-movement

class DefMesh
{
public:
    PLYmodel *pmodel;
    GLuint    mode;

    DefMesh(std::string filename);
    ~DefMesh() { delete pmodel; }
    void glDraw(int type);
};
#endif
