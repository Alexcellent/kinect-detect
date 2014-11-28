==========================================================================
= README =================================================================
==========================================================================
Name: Alexandre Huot
ID: 9317023

I certify that this submission is my original work and meets the Faculty's
Expectations of Originality. 								  Feb 09, 2014




------------------------------------------------------------------------------------
- Running the Program --------------------------------------------------------------
------------------------------------------------------------------------------------
The solution was tested and run on Visual Studio 2012 in the lab computers in H837.
Therefore it should be possible to open codebase.sln and press f5 to run the program.





---------------------------------------------------------------------------------------------------------
- Program Description -----------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------
This description outlines the particular bits of code added for each of the subsections of the assignment
and any additional information about the program design.

NOTES ON PICKING: Please at least read the last section on picking below. Certain parts of my vertex manipulation
don't seem to work so I provided an explanation and hope to show that my selection of points is in fact working, only
my manipulation is off.


Additional Rendering Features:
------------------------------

- A global int variable called 'displayMode' was added. Pressing 's' calls the keyboard function
  which cycles this value through 0,1 or 2 for 'solid', 'solid + wireframe' or 'wireframe'.
- In display(), a switch statement was added to indicate which mode to render. Mode is decided using
  glPolygonMode(GL_FRONT_AND_BACK, <GL_FILL or GL_LINE>);. In the case of drawing both modes at once,
  we first draw the 'smooth' version but with an offset enabled (glPolygonOffset(1, 1);) followed by
  wireframe. The offset allows the wireframe to take priority in display, so that both modes do not
  overlap and make the wireframe invisible.
  
  An explanation on enabling and assigning an offset was found online at:
  http://lwjgl.org/forum/index.php?topic=1162.0;wap2
  
- Note: the following code:
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < mesh->nfaces.size(); i += 1)
		for (int j = 0; j < 3; j += 1){
			glNormal3f(mesh->normal[mesh->nfaces[i][j]][0],
				mesh->normal[mesh->nfaces[i][j]][1],
				mesh->normal[mesh->nfaces[i][j]][2]);

			glVertex3f(mesh->vertex[mesh->faces[i][j]][0],
				mesh->vertex[mesh->faces[i][j]][1],
				mesh->vertex[mesh->faces[i][j]][2]);
		}
		glEnd();
  
  was moved into a function called drawObject(int renderMode). This was done for picking purposes
  where we can choose to 'fake-render' the objects points for picking, otherwise we draw and color
  the object. Thus if we pass GL_RENDER it will be colored, GL_SELECT will draw the points for
  picking.
  
 Arcball and Transformations
 ---------------------------
  
- For arcball, a function called getArcballVector() was added to source.cpp essentially following
  the steps outlined on the wiki provided in the tutorial. In mouse_motion the arcball vectors
  are computed and the rotation angle and axis are computer. The transformation is applied using
  glRotatef() and the transformation is saved in a global matrix called track_matrix (following
  steps provided by Prof. Popa). On display, the 'saved' state of the object is redisplayed using
  glMultMatrixf(track_matrix.getBuffer()).
  
- For translation and zoom, the general idea is to save the current mouse motion in any given axis in a
  variable (in this case x_motion, y_motion, z_motion). Thus whenever a motion is detected in 
  either the translation state (when right-clicking) or the zoom state (middle-clicking), the
  difference in motion on each iteration of the mouse_motion function is saved to the appropriate
  axis-motion. Thus on every redisplay, the transformations are applied using
  glTranslatef(x_motion, y_motion, z_motion);
  
 Selection and Picking
 ---------------------
 
 - As mentioned above, when picking is activated in mouse_button, the first thing we do is render the 
   object using drawObject(GL_SELECT), which performs a good portion of the steps for picking:
   
   - Initializes the viewport (picking window) and selects the buffer (a global buffer is declared 
     and initialized)
   - We change the matrix mode to modelview and apply the transformations (so the invisible object
     for picking is drawn with the correct orientation with respect to the camera)
   - We change the render mode to 'gl_select' and matrix mode to 'gl_projection', after which we can 
     set our pick matrix and perspective
   - Finally we initialize our name stack, push '-1' as the first name (because our index for vertices
     starts at '0'), then name all the points of the object. The code for naming the points also follows
	 steps provided by Prof. Popa
	 
 - Back in mouse_button, we can now determine the number of hits in our picking using glRenderMode(GL_RENDER)
 - We pop the first three values from the buffer, which gives us our number of items, z_min and z_max
 - We then iterate through the buffer and select the point with the smallest z-value (ie equals zmin)
   as this is the point closest to the camera (the selected point).
   
 - Now that we know which vertex is selected, in mouse_motion we can manipulate this vertex based on
   the x and y motion of the mouse. Unfortunately this transformation I was not able to implement
   correctly, so the point only ever moves in the x-y axis respective to the object, and not
   the world itself (so if the object is rotated, the manipulation is still applied with respect to
   the object and not it's position in the scene). However I hope to demonstrate that I can in fact 
   pick a specific point and manipulate it, only that I am missing the few lines of code to manipulate 
   the vertex correctly.
   
   I believe to fix this problem I would need to find the vector that goes from the x-y of the mouse
   click to the center of the object, and assign this as the new position of the vertex. This could
   be done in a similar fashion to getting the arcball vector, but trying several approaches I was
   unable to solve this problem any better than the current implementation.
   
   Note: I find picking works best if you don't rotate the object with arcball first. It still works
   after rotation but the motion is difficult to see because the manipulation only happens in certain 
   directions.
   
That completes this brief description of the added functionality of the provided code.
  
  
 