
#ifndef MESH_H
#define MESH_H
#include "bbox.h"
#include <string>
#include <limits>
#include <iostream>
#include <vector>

// TODO:
// - Add center of mass
// - Add boolean isPicked
// - Transform all points on mouse-movement

class DefMesh
{
public:
    PLYmodel *pmodel;
    float     t_matrix[16];
    GLuint    mode;
    BBox      bbox;
    Status    status;

    DefMesh (std::string filename);
    ~DefMesh() 
    { 
        delete pmodel; 
    }

    void Save(std::string filename);
    
    //void updateMesh();
    void glDraw(int meshModel);

};
#endif
