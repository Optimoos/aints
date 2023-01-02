#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
//#define RAYGUI_CUSTOM_ICONS     // It requires providing gui_icons.h in the same directory
//#include "gui_icons.h"          // External icons data provided, it can be generated with rGuiIcons tool
#include "raygui.h"
#include "aints.h"
#include <entt/entt.hpp>
#include <string>
#include <iostream>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 900;

    entt::registry registry;


    for (int i = 0; i < 10; i++){
        auto entity = registry.create();
        aints ant1;
        //movementNeuron mn(ant1);
        //ant1.addNeuron();
        ant1.setId(i);
        ant1.updateLocation(800, 800);
        registry.emplace<aints>(entity, ant1);
    }

    auto antview = registry.view<aints>();

    for (auto entity: antview){
        auto &anAnt = antview.get<aints>(entity);
        std::cout << "Ant Entity ID " << anAnt.getId() << std::endl;
    }


    InitWindow(screenWidth, screenHeight, "Aints");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    bool Button000Pressed = false;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        for (auto entity: antview){
            auto &anAnt = antview.get<aints>(entity);
            anAnt.tick();
            DrawPixel(anAnt.getX(), anAnt.getY(), BLACK);
        }

        //DrawPixel(ant1.getX(), ant1.getY(), BLACK );
        //int64_t newAntX = ant1.getX() - 1;
        //ant1.updateLocation(newAntX, ant1.getY());


        // raygui: controls drawing
        //----------------------------------------------------------------------------------
        Button000Pressed = GuiButton((Rectangle){ 430, 276, 120, 24 }, "SAMPLE TEXT");
        //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

