
#include "defMesh.h"

DefMesh::DefMesh(std::string filename) : status(IDLE)
{
    pmodel = new PLYmodel;
    pmodel->Load((char*)filename.c_str());
    bbox.makeBBox(pmodel);
    loadIdentity(t_matrix);
    
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

//void DefMesh::updateMesh(){
//
//    for (int i = 0; i < pmodel->TotalConnectedPoints; i++){
//
//        // Simplify getting coordinate of vertex
//        // Note: vertices start at i = 1
//        int x = (i + 1) * 3;
//        int y = (i + 1) * 3 + 1;
//        int z = (i + 1) * 3 + 2;
//
//        // Zero out current vertex
//        pmodel->Vertex_Buffer[x] = 0.0;
//        pmodel->Vertex_Buffer[y] = 0.0;
//        pmodel->Vertex_Buffer[z] = 0.0;
//        float initPos[3] = { pmodel->Init_Vertex_Buffer[x], pmodel->Init_Vertex_Buffer[y], pmodel->Init_Vertex_Buffer[z] };
//
//        // Update vertex positions
//        float newPos[3];
//        multv(t_matrix, initPos, newPos);
//        pmodel->Vertex_Buffer[x] += newPos[0];
//        pmodel->Vertex_Buffer[y] += newPos[1];
//        pmodel->Vertex_Buffer[z] += newPos[2];
//
//    }
//}

void DefMesh::Save(std::string filename)
{
    std::ofstream outFile(filename);
    if (!outFile)
    {
        std::cerr << "Error opening output file: " << filename << "!" << std::endl;
        exit(1);
    }

    std::cout << "Saving Mesh! Please hold...\n";

    ////
    // Output TRANSFORMED mesh to file
    ////
    std::vector<float> transformedVerts;
    for (int i = 0; i < pmodel->TotalConnectedPoints; i++)
    {
        float p[3] = { pmodel->Vertex_Buffer[0 + (i * 3)],
                       pmodel->Vertex_Buffer[1 + (i * 3)],
                       pmodel->Vertex_Buffer[2 + (i * 3)] };
        multv(t_matrix, p, p);
        
        transformedVerts.push_back(p[0]);
        transformedVerts.push_back(p[1]);
        transformedVerts.push_back(p[2]);

    }

    ////
    // Header
    ////

    outFile << "ply" << std::endl;
    outFile << "format ascii 1.0" << std::endl;
    outFile << "element vertex "  << pmodel->TotalConnectedPoints << std::endl;
    outFile << "property float x" << std::endl;
    outFile << "property float y" << std::endl;
    outFile << "property float z" << std::endl;
    outFile << "element face "    << pmodel->TotalConnectedTriangles << std::endl;
    outFile << "property list uchar int vertex_index" << std::endl;
    outFile << "end_header" << std::endl;

    ////
    // Points
    ////

    for (int i = 0; i < pmodel->TotalConnectedPoints; i++)
    {
        outFile << transformedVerts[0 + (i * 3)] << " "
                << transformedVerts[1 + (i * 3)] << " "
                << transformedVerts[2 + (i * 3)] << "\n";

    }

    ////
    // Triangles
    ////

    for (int i = 0; i < pmodel->TotalConnectedTriangles; i++)
    {

        outFile << "3 ";

        outFile << pmodel->Faces_Triangles[0 + (i * 3)] << " "
                << pmodel->Faces_Triangles[1 + (i * 3)] << " "
                << pmodel->Faces_Triangles[2 + (i * 3)] << std::endl;

        outFile << std::endl;
    }
    std::cout << "Saving complete!\n";

    return;
}

void DefMesh::glDraw(int meshModel)
{
    float r, g, b;
    
    switch (status){
    case IDLE:
        r = 0.5; g = 0.5; b = 0.5;
        break;
    case SELECTED:
        r = 0.0; g = 0.5; b = 0.0;
        break;
    }

    switch (meshModel){
    case 0:
        // DRAW MESH
        glPushMatrix();
        glMultMatrixf(t_matrix);

        glColor3f(r, g, b);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, pmodel->Faces_Triangles);
        glNormalPointer(GL_FLOAT, 0, pmodel->Normals);
        glDrawArrays(GL_TRIANGLES, 0, pmodel->TotalConnectedTriangles);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);

        glPopMatrix();
        break;

    case 1:
        // DRAW POINTS W/O DEPTH MAP

        glPushMatrix();
        glMultMatrixf(t_matrix);
        glPointSize(0.01f);
        glBegin(GL_POINTS);

        for (int i = 0; i < pmodel->TotalConnectedPoints; i++)
        {
            
            glColor3f(r, g, b);

            glVertex3f(pmodel->Vertex_Buffer[0 + (i * 3)],
                       pmodel->Vertex_Buffer[1 + (i * 3)],
                       pmodel->Vertex_Buffer[2 + (i * 3)]);
            
            glNormal3f(pmodel->Normals[0 + (i * 3)],
                       pmodel->Normals[1 + (i * 3)],
                       pmodel->Normals[2 + (i * 3)]);
        }

        glEnd();
        glPopMatrix();
        break;

    case 2:
        // DRAW POINTS WITH DEPTH MAP

        // Get viewport, projection and modelview
        GLint    viewport[4] = { 0, 0, 1, 1 };
        GLdouble modelview[16];
        GLdouble projection[16];

        glGetIntegerv(GL_VIEWPORT,          viewport  );
        glGetDoublev (GL_MODELVIEW_MATRIX,  modelview );
        glGetDoublev (GL_PROJECTION_MATRIX, projection);

        // Get camera position in world space
        GLdouble camPos[3];
        gluUnProject((viewport[2] - viewport[0]) / 2, (viewport[3] - viewport[1]) / 2, 0.0, 
                      modelview, projection, viewport, &camPos[0], &camPos[1], &camPos[2]);

        // Get mesh center in world space
        float mv_mat[16];
        for (int i = 0; i < 16; i++) mv_mat[i] = modelview[i];

        float meshCenter[3] = { bbox.xCen, bbox.yCen, bbox.zCen };
        mult(mv_mat, t_matrix, mv_mat);
        multv(mv_mat, meshCenter, meshCenter);
        

        // Get min/max distance of mesh from camera or origin
        float min[3], max[3];
        for (int i = 0; i < 3; i++) {
            min[i] = meshCenter[i] - bbox.getRadius() * 0.75;
            max[i] = meshCenter[i] + bbox.getRadius() * 0.75;
        }


        glPushMatrix();
        glMultMatrixf(t_matrix);
        glPointSize(0.01f);
        glBegin(GL_POINTS);
        for (int i = 0; i < pmodel->TotalConnectedPoints; i++)
        {

            float p[3] = { pmodel->Vertex_Buffer[0 + (i * 3)],
                           pmodel->Vertex_Buffer[1 + (i * 3)],
                           pmodel->Vertex_Buffer[2 + (i * 3)] };

            multv(mv_mat, p, p);
            float intensity = (p[2] - min[2]) / (max[2] - min[2]);

            if (status == SELECTED) 
                glColor3f(intensity, 1.0 - intensity, 0.0);
            else
                glColor3f(intensity, 0.0, 1.0 - intensity);
            

            glVertex3f(pmodel->Vertex_Buffer[0 + (i * 3)],
                       pmodel->Vertex_Buffer[1 + (i * 3)],
                       pmodel->Vertex_Buffer[2 + (i * 3)]);

        }
        glEnd();
        glPopMatrix();
        break;
    }


}