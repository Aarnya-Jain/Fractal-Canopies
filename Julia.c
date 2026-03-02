#define RAYGUI_IMPLEMENTATION

#include "raygui.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 950
#define HEIGHT 650

void DrawJulia(float R,Vector2 screen_start,Vector2 screen_end,Vector2 c,int maxit,float scale){
  for(int px = screen_start.x;px<=screen_end.x;++px){
    for(int py = screen_start.y;py<=screen_end.y;++py){
      Vector2 pix = {px,py};

      float width = screen_end.x - screen_start.x;
      float height = screen_end.y - screen_start.y;

      float zx = (px - screen_start.x) / width;
      float zy = (py - screen_start.y) / height;

      zx = zx * 2.0f - 1.0f;
      zy = zy * 2.0f - 1.0f;

      float aspect = width / height;
      zx *= aspect;

      zx *= scale;
      zy *= scale;

      int i = 0;

      while((zx*zx + zy*zy < R*R)&&(i<maxit)){
        float xtemp = zx*zx - zy*zy + c.x;
        zy = 2.0f*zx*zy + c.y;
        zx = xtemp;

        ++i;
      }

      if(i == maxit){
        DrawPixelV(pix,RAYWHITE);
      } else{
        DrawPixelV(pix,BLACK);
      }
    }
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

    // Julia params
    float maxit = 150.0f;
    float scale = 1.0f;
    float R = 2.0f;
    float cx = -0.7f;
    float cy = 0.27015f;

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

            Vector2 start = {0,0};
            Vector2 end = {(float)w,(float)h};
            Vector2 c = {cx,cy};
            DrawJulia(R,start,end,c,maxit,scale);

            int l = 0;
            GuiLabel((Rectangle){120,l+=30, 200, 20}, "Julia Parameters");
            // --------------------------------------------------------
            GuiSliderBar(
                (Rectangle){60, l+=30, 200, 20},
                "Iterations", TextFormat("%.2f", maxit),
                &maxit, 100.0f, 500.0f
            );

            GuiSliderBar(
                (Rectangle){60, l+=30, 200, 20},
                "Scale", TextFormat("%.2f", scale),
                &scale, 0.5f, 4.0f
            );

            GuiSliderBar(
                (Rectangle){60, l+=30, 200, 20},
                "Escape Radius", TextFormat("%.2f", R),
                &R, 1.5f, 4.0f
            );

            GuiSliderBar(
                (Rectangle){60, l+=30, 200, 20},
                "c-x", TextFormat("%.2f", cx),
                &cx, -1.5f, 1.5f
            );

            GuiSliderBar(
                (Rectangle){60, l+=30, 200, 20},
                "c-y", TextFormat("%.2f", cy),
                &cy, -1.5f, 1.5f
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

        EndDrawing();
    }

    CloseWindow();
    return 0;
}