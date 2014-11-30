// Found at: http://www.cs.princeton.edu/~edwardz/tutorials/kinect/kinect0.html

#include <Windows.h>
#include <Ole2.h>

#include <cmath>
#include <cstdio>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#include "simpleMath.h"

const int width = 640;
const int height = 480;

// OpenGL Variables
long  depthToRgbMap[width*height * 2];
float colorarray[width*height * 3];
float vertexarray[width*height * 3];

// We'll be using buffer objects to store the kinect point cloud
GLuint vboId;
GLuint cboId;

// Kinect variables
HANDLE      depthStream;
HANDLE      rgbStream;
INuiSensor* sensor;

// Kinect refresh rate
int k_frame = 0;
int k_refresh = 30;

// Transformation variables
float k_matrix[16];

bool initKinect() {

    loadIdentity(k_matrix);

    // Get a working kinect sensor
    int numSensors;
    if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;
    if (NuiCreateSensorByIndex(0, &sensor) < 0) return false;

    // Initialize sensor
    sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
    sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, // Depth camera or rgb camera?
        NUI_IMAGE_RESOLUTION_640x480,                // Image resolution
        0,        // Image stream flags, e.g. near mode
        2,        // Number of frames to buffer
        NULL,     // Event handle
        &depthStream);
    sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, // Depth camera or rgb camera?
        NUI_IMAGE_RESOLUTION_640x480,                // Image resolution
        0,      // Image stream flags, e.g. near mode
        2,      // Number of frames to buffer
        NULL,   // Event handle
        &rgbStream);
    return sensor;
}

void getDepthData(GLubyte* dest) {
    float* fdest = (float*)dest;
    long* depth2rgb = (long*)depthToRgbMap;
    NUI_IMAGE_FRAME imageFrame;
    NUI_LOCKED_RECT LockedRect;
    if (sensor->NuiImageStreamGetNextFrame(depthStream, 0, &imageFrame) < 0) return;
    INuiFrameTexture* texture = imageFrame.pFrameTexture;
    texture->LockRect(0, &LockedRect, NULL, 0);
    if (LockedRect.Pitch != 0) {
        const USHORT* curr = (const USHORT*)LockedRect.pBits;
        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                // Get depth of pixel in millimeters
                USHORT depth = NuiDepthPixelToDepth(*curr++);
                // Store coordinates of the point corresponding to this pixel
                Vector4 pos = NuiTransformDepthImageToSkeleton(i, j, depth << 3, NUI_IMAGE_RESOLUTION_640x480);
                *fdest++ = pos.x / pos.w;
                *fdest++ = pos.y / pos.w;
                *fdest++ = pos.z / pos.w;
                // Store the index into the color array corresponding to this pixel
                NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
                    NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_RESOLUTION_640x480, NULL,
                    i, j, depth << 3, depth2rgb, depth2rgb + 1);
                depth2rgb += 2;
            }
        }
    }
    texture->UnlockRect(0);
    sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);
}

void getRgbData(GLubyte* dest) {
    float* fdest = (float*)dest;
    long* depth2rgb = (long*)depthToRgbMap;
    NUI_IMAGE_FRAME imageFrame;
    NUI_LOCKED_RECT LockedRect;
    if (sensor->NuiImageStreamGetNextFrame(rgbStream, 0, &imageFrame) < 0) return;
    INuiFrameTexture* texture = imageFrame.pFrameTexture;
    texture->LockRect(0, &LockedRect, NULL, 0);
    if (LockedRect.Pitch != 0) {
        const BYTE* start = (const BYTE*)LockedRect.pBits;
        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                // Determine rgb color for each depth pixel
                long x = *depth2rgb++;
                long y = *depth2rgb++;
                // If out of bounds, then don't color it at all
                if (x < 0 || y < 0 || x > width || y > height) {
                    for (int n = 0; n < 3; ++n) *(fdest++) = 0.0f;
                }
                else {
                    const BYTE* curr = start + (x + width*y) * 4;
                    for (int n = 0; n < 3; ++n) *(fdest++) = curr[2 - n] / 255.0f;
                }

            }
        }
    }
    texture->UnlockRect(0);
    sensor->NuiImageStreamReleaseFrame(rgbStream, &imageFrame);
}

void getKinectData() {
    getDepthData((GLubyte*)vertexarray);
    getRgbData((GLubyte*)colorarray);
}

void drawKinectData() {
    // Refresh at fixed frame rate for efficiency
    k_frame = (k_frame + 1) % k_refresh;
    if (k_frame == 0)
        getKinectData();

    glPushMatrix();
    glMultMatrixf(k_matrix);

    glColor3f(0.5, 0, 0);
    glutSolidSphere(0.01, 30, 30);

    glBegin(GL_POINTS);
    for (int i = 0; i < width*height; ++i) {
        glColor3f(colorarray[i * 3], colorarray[i * 3 + 1], colorarray[i * 3 + 2]);
        glVertex3f(vertexarray[i * 3], vertexarray[i * 3 + 1], vertexarray[i * 3 + 2]);
    }
    glEnd();

    glPopMatrix();
}
