/* Demonstrates how to load PLY files
*
* http://talkera.org/opengl/
* Model needs to be triangulated
* Use blender
*
* Just the class for loading PLY files.
*
*/

#ifndef PLYMODEL_H
#define PLYMODEL_H

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <string>


class PLYmodel
{
public:
    
    PLYmodel() {}
    ~PLYmodel()
    {
        free(Faces_Triangles);
        free(Faces_Quads);
        free(Vertex_Buffer);
        free(Normals);
    }
    
    int PLYmodel::Load(char *filename);
    void PLYmodel::Draw();
    float* PLYmodel::calculateNormal(float *coord1, float *coord2, float *coord3);
    

    float* Faces_Triangles;
    float* Faces_Quads;
    float* Vertex_Buffer;
    float* Normals;

    int TotalConnectedTriangles;
    int TotalConnectedQuads;
    int TotalConnectedPoints;
    int TotalFaces;
};

#endif