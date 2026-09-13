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


struct EngineState {
    bool isRunning;
    int widthWindow;
    int heightWindow;
    Uint32 timePast;
    Uint32 timeNew;
    double timeDiff;
    int advance;
    int lateral;
    Mesh meshI;
    Camera cameraI;
    uint32_t colorMesh;
    Vector3D lightDir;
    double cubeAngle;
    Vector3D movement;
};

void setup( struct EngineState* engineState ){
    engineState->isRunning = false;
    engineState->widthWindow = 800;
    engineState->heightWindow = 600;
    engineState->timePast = 0;
    engineState->timeNew = 0;
    engineState->timeDiff = 0.0;
    engineState->advance = 0;
    engineState->lateral = 0;
    engineState->colorMesh = 0xFFFF0000;
    engineState->lightDir = init3D(-1.0, -1.0, -1.0);
    engineState->lightDir = normalize3D(engineState->lightDir);
    engineState->cubeAngle = 0.0f;
    engineState->meshI = loadMeshFromOBJ("suzanne1.obj");
    engineState->cameraI = initializeCamera();
    engineState->movement = init3D(0.0f, 0.0f, 5.0f); // Move the mesh 5 units along the z-axis
    engineState->isRunning = initDisplay(engineState->widthWindow, engineState->heightWindow);
    if(!engineState->isRunning){
        printf("Failed to initialize display\n");
    }
}

void processInput(struct EngineState* engineState){
    SDL_Event event;
    while(SDL_PollEvent(&event) != 0){ 
        if(event.type == SDL_QUIT){
            engineState->isRunning = false;
        }
        else if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                        SDL_ShowCursor(SDL_ENABLE);
                    } 
                    else {
                        engineState->isRunning = false;
                    }
                    break;  
            }
        }else if(event.type == SDL_KEYUP){// key released
            switch(event.key.keysym.sym){ //which key was released
                case SDLK_w:
                case SDLK_s:
                    break;
                case SDLK_a:
                case SDLK_d:
                    break;
                default:
                    break;
            }
        }
        else if(event.type == SDL_MOUSEMOTION){
            if(SDL_GetRelativeMouseMode() == SDL_TRUE && abs(event.motion.xrel) < 50 && abs(event.motion.yrel) < 50){//if mouse moved
                updateCameraRotation(&engineState->cameraI, event.motion.xrel, event.motion.yrel);
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
    engineState->advance = 0;
    engineState->lateral = 0;
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL); // Get the current state of the keyboard
    if(keyboardState[SDL_SCANCODE_W]){
        engineState->advance = 1;
    }
    if(keyboardState[SDL_SCANCODE_S]){
        engineState->advance = -1;
    }
    if(keyboardState[SDL_SCANCODE_A]){
        engineState->lateral = -1;
    }
    if(keyboardState[SDL_SCANCODE_D]){
        engineState->lateral = 1;
    }
}

void renderMesh(struct EngineState* engineState){
    clearDisplay(0xFF000000); // Clear the display with black color
    for(int i = 0; i < engineState->meshI.numTriangles; i++){
        Triangle tAux = applyTransformations(engineState->meshI.triangles[i], engineState->movement, engineState->cameraI, engineState->cubeAngle);
        processAndDrawTriangle(&engineState->cameraI, tAux, engineState->widthWindow, engineState->heightWindow, engineState->lightDir,
             engineState->meshI.textureBuffer, engineState->meshI.textureWidth, engineState->meshI.textureHeight);
    }
    if(!updateDisplay()){
        printf("Failed to update display\n");
        engineState->isRunning = false;
    }
}



void updateMesh( struct EngineState* engineState){
    engineState->timePast = engineState->timeNew;
    engineState->timeNew = SDL_GetTicks();// Get the number of milliseconds since the SDL library initialization    
    engineState->timeDiff = (engineState->timeNew - engineState->timePast)/1000.0; // Convert to seconds
    updateCameraPosition(&engineState->cameraI, engineState->advance, engineState->lateral, engineState->timeDiff);
    engineState->cubeAngle +=  60.0f * engineState->timeDiff; // Rotate the cube at 60 degrees per second
}


int main(int argc, char* argv[]){
    struct EngineState engineState;
    setup(&engineState);
    while(engineState.isRunning){
        processInput(&engineState);
        updateMesh(&engineState);
        /*if (engineState.timeDiff > 0.0) {
            printf("FPS: %.0f\n", 1.0 / engineState.timeDiff);
        }*/
        renderMesh(&engineState);
        SDL_Delay(16);//delay for 16 milliseconds to limit the frame rate to ~60 FPS
    }
    freeMesh(&engineState.meshI);
    freeDisplay();
    return 0;
}