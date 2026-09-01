#include "display.h"
#include "vector.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include "mesh.h"
#include "camera.h"
#include "triangle.h"
#include "vector.h"
#include "transform.h"


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

bool isPacmanMode = true; // Flag to indicate if pacman mode is active
bool isRunning = false;

int widthWindow = 800;
int heightWindow = 600;

Uint32 timePast;
Uint32 timeNew = 0;
double timeDiff;

int advance = 0;
int lateral = 0;

typedef struct {
    Vector2D pos;
    Vector2D vel;
    int width;
    int height;
    uint32_t color;
} Square;

Square rectangle = {{0, 0}, {0, 0}, 50, 50, 0xFFFF0000};

Mesh meshI;
Camera cameraI;
 //0xFFFF0000, 0xFF444444
uint32_t colorMesh = 0xFFFF0000;
Vector3D lightDir;

double cubeAngle = 0.0f;

void setup(){
    lightDir = init3D(-1.0, -1.0, -1.0);
    lightDir = normalize3D(lightDir);
    meshI = loadMeshFromOBJ("suzanne.obj");
    cameraI = initializeCamera();
    isRunning = initDisplay(widthWindow, heightWindow);
    if(!isRunning){
        printf("Failed to initialize display\n");
    }
}
/*
void processInput(){
    SDL_Event event;
    while(SDL_PollEvent(&event) != 0){ //read all events from the event queue
        if(event.type == SDL_QUIT){
            isRunning = false;
        }
        else if(event.type == SDL_KEYDOWN){// key pressed
            switch(event.key.keysym.sym){ //which key was pressed problems 
                case SDLK_ESCAPE:
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    isRunning = false;
                    break;  
                case SDLK_p:
                    if(SDL_SetRelativeMouseMode(SDL_TRUE) < 0){//wait window is alive
                        printf("Failed to set relative mouse mode: %s\n", SDL_GetError());
                        isRunning = false;
                    }
                    break;
                /*case SDLK_w://if simultanious keys are pressed
                    rectangle.vel.y = -100; // Move up
                    break;
                case SDLK_s:
                    rectangle.vel.y = 100; // Move down
                    break;
                case SDLK_a:
                    rectangle.vel.x = -100; // Move left
                    break;
                case SDLK_d:
                    rectangle.vel.x = 100; // Move right
                    break;
                default:
                    break;
            }
        }else if(event.type == SDL_KEYUP){// key released
            switch(event.key.keysym.sym){ //which key was released
                case SDLK_w:
                case SDLK_s:
                    rectangle.vel.y = 0; // Stop vertical movement
                    break;
                case SDLK_a:
                case SDLK_d:
                    rectangle.vel.x = 0; // Stop horizontal movement
                    break;//
                default:
                    break;
            }
        }
        else if(event.type == SDL_MOUSEMOTION){
            if(abs(event.motion.xrel) < 50 && abs(event.motion.yrel) < 50){//if mouse moved
                updateCameraRotation(&cameraI, event.motion.xrel, event.motion.yrel);
            }
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN){
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }
    }
    // Handle continuous key presses for movement
    rectangle.vel.x = 0; // Reset horizontal velocity
    rectangle.vel.y = 0; // Reset vertical velocity
    advance = 0;
    lateral = 0;
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL); // Get the current state of the keyboard
    if(keyboardState[SDL_SCANCODE_W]){
        rectangle.vel.y += -100; // Move up
        advance = 1;
    }
    if(keyboardState[SDL_SCANCODE_S]){
        rectangle.vel.y += 100; // Move down
        advance = -1;
    }
    if(keyboardState[SDL_SCANCODE_A]){
        rectangle.vel.x += -100; // Move left
        lateral = -1;
    }
    if(keyboardState[SDL_SCANCODE_D]){
        rectangle.vel.x += 100; // Move right
        lateral = 1;
    }
}
*/
void processInput(){
    SDL_Event event;
    while(SDL_PollEvent(&event) != 0){ 
        if(event.type == SDL_QUIT){
            isRunning = false;
        }
        else if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                        SDL_ShowCursor(SDL_ENABLE);
                    } 
                    else {
                        isRunning = false;
                    }
                    break;  
                /*case SDLK_w://if simultanious keys are pressed
                    rectangle.vel.y = -100; // Move up
                    break;
                case SDLK_s:
                    rectangle.vel.y = 100; // Move down
                    break;
                case SDLK_a:
                    rectangle.vel.x = -100; // Move left
                    break;
                case SDLK_d:
                    rectangle.vel.x = 100; // Move right
                    break;
                default:
                    break;
            }
        }else if(event.type == SDL_KEYUP){// key released
            switch(event.key.keysym.sym){ //which key was released
                case SDLK_w:
                case SDLK_s:
                    rectangle.vel.y = 0; // Stop vertical movement
                    break;
                case SDLK_a:
                case SDLK_d:
                    rectangle.vel.x = 0; // Stop horizontal movement
                    break;*/
                default:
                    break;
            }
        }
        else if(event.type == SDL_MOUSEMOTION){
            if(SDL_GetRelativeMouseMode() == SDL_TRUE && abs(event.motion.xrel) < 50 && abs(event.motion.yrel) < 50){//if mouse moved
                updateCameraRotation(&cameraI, event.motion.xrel, event.motion.yrel);
            }
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN){
            if (event.button.button == SDL_BUTTON_LEFT) {
                if(SDL_SetRelativeMouseMode(SDL_TRUE) < 0){
                    printf("Error al capturar el ratón: %s\n", SDL_GetError());
                }
                else{
                    SDL_ShowCursor(SDL_DISABLE);
                }
            }
        }
    }
    // Handle continuous key presses for movement
    rectangle.vel.x = 0; // Reset horizontal velocity
    rectangle.vel.y = 0; // Reset vertical velocity
    advance = 0;
    lateral = 0;
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL); // Get the current state of the keyboard
    if(keyboardState[SDL_SCANCODE_W]){
        rectangle.vel.y += -100; // Move up
        advance = 1;
    }
    if(keyboardState[SDL_SCANCODE_S]){
        rectangle.vel.y += 100; // Move down
        advance = -1;
    }
    if(keyboardState[SDL_SCANCODE_A]){
        rectangle.vel.x += -100; // Move left
        lateral = -1;
    }
    if(keyboardState[SDL_SCANCODE_D]){
        rectangle.vel.x += 100; // Move right
        lateral = 1;
    }
}

void drawRectanglePM(){
    drawRectangle(rectangle.pos.x, rectangle.pos.y, rectangle.width, rectangle.height, rectangle.color);// Draw the rectangle at its current position
    if(rectangle.pos.x + rectangle.width >= widthWindow){// if the rectangle is partially off the right edge of the window
        drawRectangle(rectangle.pos.x - widthWindow, rectangle.pos.y, rectangle.width, rectangle.height, rectangle.color);
    }
    if(rectangle.pos.y + rectangle.height >= heightWindow){// if the rectangle is partially off the bottom edge of the window
        drawRectangle(rectangle.pos.x, rectangle.pos.y - heightWindow, rectangle.width, rectangle.height, rectangle.color);
    }
    if(rectangle.pos.x < 0){// if the rectangle is partially off the left edge of the window
        drawRectangle(rectangle.pos.x + widthWindow, rectangle.pos.y, rectangle.width, rectangle.height, rectangle.color);
    }
    if(rectangle.pos.y < 0){// if the rectangle is partially off the top edge of the window
        drawRectangle(rectangle.pos.x, rectangle.pos.y + heightWindow, rectangle.width, rectangle.height, rectangle.color);
    }
    if(rectangle.pos.x + rectangle.width >= widthWindow && rectangle.pos.y + rectangle.height >= heightWindow){// if the rectangle is partially off the right and bottom edges of the window
        drawRectangle(rectangle.pos.x - widthWindow, rectangle.pos.y - heightWindow, rectangle.width, rectangle.height, rectangle.color);
    }
    if(rectangle.pos.x < 0 && rectangle.pos.y < 0){// if the rectangle is partially off the left and top edges of the window
        drawRectangle(rectangle.pos.x + widthWindow, rectangle.pos.y + heightWindow, rectangle.width, rectangle.height, rectangle.color);
    }
    if(rectangle.pos.x < 0 && rectangle.pos.y + rectangle.height >= heightWindow){// if the rectangle is partially off the left and bottom edges of the window
        drawRectangle(rectangle.pos.x + widthWindow, rectangle.pos.y - heightWindow, rectangle.width, rectangle.height, rectangle.color);
    }
    if (rectangle.pos.y < 0 && rectangle.pos.x + rectangle.width >= widthWindow)// if the rectangle is partially off the top and right edges of the window
    {
        drawRectangle(rectangle.pos.x - widthWindow, rectangle.pos.y + heightWindow, rectangle.width, rectangle.height, rectangle.color);
    }
    
}

void render(){
    clearDisplay(0xFF000000); // Clear the display with black color
    if(isPacmanMode)
        drawRectanglePM(); // Draw the rectangle in pacman mode
    else
        drawRectangle(rectangle.pos.x, rectangle.pos.y, rectangle.width, rectangle.height, rectangle.color); // Draw the current position in pacman mode
    if(!updateDisplay()){
        printf("Failed to update display\n");
        isRunning = false;
    }
}

int clippingTriangle(Triangle t, Triangle* clippedTriangles){
    Vector3D pointsOut[3];
    Vector3D pointsIn[4];
    int numOut = 0;
    int numIn = 0;
    for(int i = 0; i < 3; i++){
        if(t.points[i].z < 0.1f){
            pointsOut[numOut] = t.points[i];
            numOut++;
        }else{
            pointsIn[numIn] = t.points[i];
            numIn++;
        }
    }

    if(numIn == 3){
        // All points are on the camera, so the triangle is not visible
        clippedTriangles[0] = t;
        return 1;
    }else if(numIn == 2)
    {
        // Two points are on the camera, so we need to clip the triangle
        float aux = (0.1f -pointsIn[0].z)/(pointsOut[0].z - pointsIn[0].z);
        clippedTriangles[0].points[0] = pointsIn[0];
        clippedTriangles[0].points[1] = pointsIn[1];
        clippedTriangles[0].points[2] = init3D(pointsIn[0].x + aux * (pointsOut[0].x - pointsIn[0].x), pointsIn[0].y + aux * (pointsOut[0].y - pointsIn[0].y), 0.1f);
        clippedTriangles[1].points[0] = pointsIn[1];
        clippedTriangles[1].points[1] = clippedTriangles[0].points[2];
        aux = (0.1f -pointsIn[1].z)/(pointsOut[0].z - pointsIn[1].z);
        clippedTriangles[1].points[2] = init3D(pointsIn[1].x + aux * (pointsOut[0].x - pointsIn[1].x), pointsIn[1].y + aux * (pointsOut[0].y - pointsIn[1].y), 0.1f);
        return 2;
    }
    else if (numIn == 1)
    {
        // One point is on the camera, so we need to clip the triangle
        float aux = (0.1f -pointsIn[0].z)/(pointsOut[0].z - pointsIn[0].z);
        clippedTriangles[0].points[0] = pointsIn[0];
        clippedTriangles[0].points[1] = init3D(pointsIn[0].x + aux * (pointsOut[0].x - pointsIn[0].x), pointsIn[0].y + aux * (pointsOut[0].y - pointsIn[0].y), 0.1f);
        aux = (0.1f -pointsIn[0].z)/(pointsOut[1].z - pointsIn[0].z);
        clippedTriangles[0].points[2] = init3D(pointsIn[0].x + aux * (pointsOut[1].x - pointsIn[0].x), pointsIn[0].y + aux * (pointsOut[1].y - pointsIn[0].y), 0.1f);
        return 1;
    }
    else
    {
        return 0; // All points are behind the camera, so the triangle is not visible   
    }
}


void renderMesh(){
    clearDisplay(0xFF000000); // Clear the display with black color
    Vector3D movement = {0.0f, 0.0f, 5.0f}; // Move the mesh 5 units along the z-axis
    Vector3D screenEdges[3];
    Vector3D aux;
    for(int i = 0; i < meshI.numTriangles; i++){
        Triangle tAux;
        for (int j = 0; j < 3; j++)
        {
            aux = rotateY(meshI.triangles[i].points[j], cubeAngle);
            aux = sum3D(aux, movement);
            aux = rest3D(aux, cameraI.position);
            aux = rotateY(aux, -cameraI.yaw);
            aux = rotateX(aux, -cameraI.pitch);
            tAux.points[j] = aux;
        } 
        Triangle clippedTriangles[2];
        int numClipped = clippingTriangle(tAux, clippedTriangles);
        for (int l = 0; l < numClipped; l++)
        {
            Vector3D normal = triangleNormal(clippedTriangles[l]);
            //Vector3D cameraRay = rest3D(clippedTriangles[l].points[0], cameraI.position);
            Vector3D cameraRay = clippedTriangles[l].points[0]; // Assuming the camera is at the origin (0, 0, 0)
            if(dotProduct3D(normal, cameraRay) < 0.0f){ // Only draw the triangle if it's facing the camera
                for (int j = 0; j < 3; j++)
                {
                    screenEdges[j] = worldToScreen(&cameraI, clippedTriangles[l].points[j], widthWindow, heightWindow);
                }
                uint32_t modifiedColorMesh = applyLight(colorMesh,dotProduct3D(normal, lightDir));
                //drawLine(screenEdges[0].x, screenEdges[0].y, screenEdges[0].z,screenEdges[1].x, screenEdges[1].y, screenEdges[1].z, 0xFFFFFFFF); // Draw the first edge of the triangle
                //drawLine(screenEdges[1].x, screenEdges[1].y, screenEdges[1].z, screenEdges[2].x, screenEdges[2].y, screenEdges[2].z, 0xFFFFFFFF); // Draw the second edge of the triangle
                //drawLine(screenEdges[2].x, screenEdges[2].y, screenEdges[2].z, screenEdges[0].x, screenEdges[0].y, screenEdges[0].z, 0xFFFFFFFF); // Draw the third edge of the triangle
                drawFilledTriangle(screenEdges[0].x, screenEdges[0].y, screenEdges[0].z, screenEdges[1].x, screenEdges[1].y, screenEdges[1].z, screenEdges[2].x, screenEdges[2].y, screenEdges[2].z, modifiedColorMesh); // Draw the filled triangle
            }
        }
    }
    if(!updateDisplay()){
        printf("Failed to update display\n");
        isRunning = false;
    }
}



void updatePM(){
    timePast = timeNew;
    timeNew = SDL_GetTicks();// Get the number of milliseconds since the SDL library initialization    
    timeDiff = (timeNew - timePast)/1000.0; // Convert to seconds
    double distanceX = rectangle.vel.x * timeDiff; // Calculate the distance to move based on speed and time difference
    double distanceY = rectangle.vel.y * timeDiff; // Calculate the distance to move based on speed and time difference
    Vector2D movement = init(distanceX, distanceY); // Create a vector for the movement
    rectangle.pos = sum(rectangle.pos, movement); // Update the position of the rectangle by adding the movement vector to the current position
    if(rectangle.pos.x >= widthWindow){ // If the rectangle has moved off the right edge of the window
        rectangle.pos.x -= widthWindow;
    }
    else if(rectangle.pos.x + rectangle.width <= 0){// If the rectangle has moved off the left edge of the window
        rectangle.pos.x += widthWindow;
    }

    if(rectangle.pos.y >= heightWindow){// If the rectangle has moved off the bottom edge of the window
        rectangle.pos.y -= heightWindow;
    }
    else if(rectangle.pos.y + rectangle.height <= 0){// If the rectangle has moved off the top edge of the window
        rectangle.pos.y += heightWindow;
    }
}

void updateC(){
    timePast = timeNew;
    timeNew = SDL_GetTicks();// Get the number of milliseconds since the SDL library initialization    
    timeDiff = (timeNew - timePast)/1000.0; // Convert to seconds
    double distanceX = rectangle.vel.x * timeDiff; // Calculate the distance to move based on speed and time difference
    double distanceY = rectangle.vel.y * timeDiff; // Calculate the distance to move based on speed and time difference
    Vector2D movement = init(distanceX, distanceY); // Create a vector for the movement
    rectangle.pos = sum(rectangle.pos, movement); // Update the position of the rectangle by adding the movement vector to the current position

    if(rectangle.pos.x + rectangle.width >= widthWindow){// If the rectangle has moved off the right edge of the window
        rectangle.vel.x = 0;
        rectangle.pos.x = widthWindow - rectangle.width; // Keep the rectangle within the window bounds
    }
    if(rectangle.pos.x < 0){// If the rectangle has moved off the left edge of the window
        rectangle.vel.x = 0;
        rectangle.pos.x = 0; // Keep the rectangle within the window bounds
    }

    if(rectangle.pos.y + rectangle.height >= heightWindow){// If the rectangle has moved off the bottom edge of the window
        rectangle.vel.y = 0;
        rectangle.pos.y = heightWindow - rectangle.height; // Keep the rectangle within the window bounds
    }
    if(rectangle.pos.y < 0){// If the rectangle has moved off the top edge of the window
        rectangle.vel.y = 0;
        rectangle.pos.y = 0; // Keep the rectangle within the window bounds
    }
}

void updateMesh(){
    timePast = timeNew;
    timeNew = SDL_GetTicks();// Get the number of milliseconds since the SDL library initialization    
    timeDiff = (timeNew - timePast)/1000.0; // Convert to seconds
    updateCameraPosition(&cameraI, advance, lateral, timeDiff);
    cubeAngle += 1.0f; 
}


int main(int argc, char* argv[]){
    setup();
    while(isRunning){
        processInput();
        /*
        if(isPacmanMode)
            updatePM();
        else
            updateC();
        render();
        */
        updateMesh();
        renderMesh();
        SDL_Delay(16);//delay for 16 milliseconds to limit the frame rate to ~60 FPS
    }
    freeDisplay();
    return 0;
}