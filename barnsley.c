#define RAYGUI_IMPLEMENTATION

#include "raygui.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 950
#define HEIGHT 650

void DrawLeaf(Vector2 screenStart, float scale, int maxt)
{
    Vector2 p = { 0.0f, 0.0f };
    for (int i = 0; i < maxt; i++)
    {
        float xn, yn;
        float x = p.x;
        float y = p.y;
        float r = GetRandomValue(0, 10000) / 10000.0f;

        if (r < 0.01f) {
            xn = 0.0f;
            yn = 0.16f * y;
        }
        else if (r < 0.86f) {
            xn = 0.85f * x + 0.04f * y;
            yn = -0.04f * x + 0.85f * y + 1.6f;
        }
        else if (r < 0.93f) {
            xn = 0.20f * x - 0.26f * y;
            yn = 0.23f * x + 0.22f * y + 1.6f;
        }
        else {
            xn = -0.15f * x + 0.28f * y;
            yn = 0.26f * x + 0.24f * y + 0.44f;
        }

        // leaf space => screen space
        Vector2 screen = {
            screenStart.x + xn  * scale,
            screenStart.y - yn * scale
        };

        DrawPixelV(screen, RAYWHITE);
        p.x = xn;
        p.y = yn;
    }
}

int main(int argc,char *argv[]) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Fractals Visualiser");

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x181818FF);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x2A2A2AFF);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0x3A3A3AFF);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x505050FF);
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x808080FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xE0E0E0FF);

    GuiSetStyle(SLIDER, SLIDER_WIDTH, 8);
    GuiSetStyle(SLIDER, BORDER_WIDTH, 1);

    // float leaf params
    float t = 200.0f;
    float scale = 50.0f;

    // dropdown params
    int fractalIndex = 0;
    bool fractalEdit = false;
    const char * fractalList = "Tree;Canopy;L-System;Fern";

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {

        int w = GetScreenWidth();
        int h = GetScreenHeight();
        BeginDrawing();
            ClearBackground(BLACK);
            int l = 0;
            GuiLabel((Rectangle){120,l+=30, 200, 20}, "Leaf Parameters");
            // --------------------------------------------------------
            GuiSliderBar(
                (Rectangle){60, l+=30, 200, 20},
                "Iterations", TextFormat("%.2f", t),
                &t, 100.0f, 50000.0f
            );

            GuiSliderBar(
                (Rectangle){60, l+=30, 200, 20},
                "Scale", TextFormat("%.2f", scale),
                &scale, 10.0f, 200.0f
            );
            // --------------------------------------------------------

            if(GuiDropdownBox(
                (Rectangle){60,l+=30,200,20},
                fractalList,
                &fractalIndex,
                fractalEdit
            )){
                fractalEdit = !fractalEdit;
            }

            Vector2 start = {w/2,h-20};
            DrawLeaf(start,scale,t);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}