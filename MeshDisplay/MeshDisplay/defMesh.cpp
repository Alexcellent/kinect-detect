
#include "defMesh.h"

DefMesh::DefMesh(std::string filename)
{
    pmodel = new PLYmodel;
    pmodel->Load((char*)filename.c_str());
    makeBBox();
    
    //pmodel = NULL;
    //if (!pmodel) {	/* load up the model */

    //char* meshFile = (char*)filename.c_str();
    //pmodel = glmReadOBJ(meshFile);
    //if (!pmodel) {
    //    return;
    //}
    //    glmFacetNormals(pmodel);
    //    glmVertexNormals(pmodel, 0);
    //    glmFacetNormals(pmodel);

    //    // save initial values for vertices
    //    for (int i = 0; i <= pmodel->numvertices; i++){

    //        int x = i * 3;
    //        int y = i * 3 + 1;
    //        int z = i * 3 + 2;

    //        pmodel->initVertices[x] = pmodel->vertices[x];
    //        pmodel->initVertices[y] = pmodel->vertices[y];
    //        pmodel->initVertices[z] = pmodel->vertices[z];
    //    }

    //    // save initial values for normals
    //    for (int i = 0; i <= pmodel->numnormals; i++){
    //        
    //        int x = i * 3;
    //        int y = i * 3 + 1;
    //        int z = i * 3 + 2;

    //        pmodel->initNormals[x] = pmodel->normals[x];
    //        pmodel->initNormals[y] = pmodel->normals[y];
    //        pmodel->initNormals[z] = pmodel->normals[z];
    //    }

    //}
   
}

void DefMesh::glDraw(int type)
{
    glColor3f(0.5, 0.5, 0.5);
    pmodel->Draw();
    glColor3f(0.5, 0.0, 0.0);
    bbox.draw();
    
    //switch(type){
    //case 0:
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    //case 1:
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
    //case 2:
    //    glPolygonMode(GL_POINT, GL_FILL); break;
    //}
    //
    //glColor3f(0.5, 0.5, 0.5);
    //mode = GLM_NONE;
    //mode = mode | GLM_SMOOTH;
    //glPushMatrix();
    //glScalef(2,2,2);
    //glTranslatef(-0.5, -0.5, -0.5);
    //glmDraw(pmodel, mode);
    //glPopMatrix();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void DefMesh::makeBBox()
{
    // Find meshes bounding box
    bbox.xMin = bbox.yMin = bbox.zMin =  FLT_MAX;
    bbox.xMax = bbox.yMax = bbox.zMax = -FLT_MAX;

    std::cout << "xMin: " << bbox.xMin << " yMin: " << bbox.yMin << " zMin: " << bbox.zMin << std::endl
              << "xMax: " << bbox.xMax << " yMax: " << bbox.yMax << " zMax: " << bbox.zMax << std::endl
              << "xCen: " << bbox.xCen << " yCen: " << bbox.yCen << " zCen: " << bbox.zCen << std::endl;
        
    for (unsigned i = 0; i < pmodel->TotalConnectedPoints; i++){
        float v_x = pmodel->Vertex_Buffer[0 + (i * 3)];
        float v_y = pmodel->Vertex_Buffer[1 + (i * 3)];
        float v_z = pmodel->Vertex_Buffer[2 + (i * 3)];

        if (v_x < bbox.xMin) bbox.xMin = v_x;
        if (v_y < bbox.yMin) bbox.yMin = v_y;
        if (v_z < bbox.zMin) bbox.zMin = v_z;

        if (v_x > bbox.xMax) bbox.xMax = v_x;
        if (v_y > bbox.yMax) bbox.yMax = v_y;
        if (v_z > bbox.zMax) bbox.zMax = v_z;
    }

    bbox.xCen = (bbox.xMin + bbox.xMax) / 2.0;
    bbox.yCen = (bbox.yMin + bbox.yMax) / 2.0;
    bbox.zCen = (bbox.zMin + bbox.zMax) / 2.0;

    std::cout << "xMin: " << bbox.xMin << " yMin: " << bbox.yMin << " zMin: " << bbox.zMin << std::endl
              << "xMax: " << bbox.xMax << " yMax: " << bbox.yMax << " zMax: " << bbox.zMax << std::endl
              << "xCen: " << bbox.xCen << " yCen: " << bbox.yCen << " zCen: " << bbox.zCen << std::endl;
}