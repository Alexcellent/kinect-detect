
#include "defMesh.h"

DefMesh::DefMesh(std::string filename)
{
    pmodel = new PLYmodel;
    pmodel->Load((char*)filename.c_str());
    
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
    pmodel->Draw();
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