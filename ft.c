#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 950
#define HEIGHT 650

void DrawTree(double leftAngle , double rightAngle, double ratio) {

}

void DrawBranch(Vector2 start,float length,float angle,float thickness,float ratio,float branch_angle_l,float branch_angle_r) {
  if(length < 1) return;
  float x_end = start.x + sinf(angle)*length;
  float y_end = start.y - cosf(angle)*length;
  Vector2 end = {x_end,y_end};

  float new_length = length*ratio;
  float new_thickness = thickness*ratio;
  float new_angle_l = angle + branch_angle_l;
  float new_angle_r = angle - branch_angle_r;
  DrawLineEx(start,end,thickness,RAYWHITE);
  DrawBranch(end,new_length,new_angle_l,new_thickness,ratio,branch_angle_l,branch_angle_r);
  DrawBranch(end,new_length,new_angle_r,new_thickness,ratio,branch_angle_l,branch_angle_r);
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

    // params
    float length = 250.0f;
    float thickness = 20.0f;
    float ratio = 0.7f;

    // angles
    bool symmetric = false;
    float angleDegL = 20.0f;
    float angleDegR = 20.0f;

    // wind toggle
    bool wind = false;

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
            GuiLabel((Rectangle){120, 30, 200, 20}, "Tree Parameters");

            GuiSliderBar(
                (Rectangle){60, 60, 200, 20},
                "Length", TextFormat("%.1f", length),
                &length, 50.0f, 500.0f
            );

            GuiSliderBar(
                (Rectangle){60, 90, 200, 20},
                "Thickness", TextFormat("%.1f", thickness),
                &thickness, 1.0f, 50.0f
            );

            GuiSliderBar(
                (Rectangle){60, 120, 200, 20},
                "Ratio", TextFormat("%.2f", ratio),
                &ratio, 0.5f, 0.75f
            );

            GuiCheckBox(
                (Rectangle){120,10,15,15},
                "Symmetry",
                &symmetric
            );

            if(symmetric) {
              GuiSliderBar(
                  (Rectangle){60, 150, 200, 20},
                  "Angle", TextFormat("%.1f", angleDegL),
                  &angleDegL, 0.0f, 180.0f
              );

              angleDegR = angleDegL;

              GuiToggle(
                  (Rectangle){60,180,200,20},
                  "Wind",
                  &wind
              );

              if(GuiDropdownBox(
                  (Rectangle){60,210,200,20},
                  fractalList,
                  &fractalIndex,
                  fractalEdit
              )){
                fractalEdit = !fractalEdit;
              }
            }

            else
            {
              GuiSliderBar(
                  (Rectangle){60, 150, 200, 20},
                  "L - Angle", TextFormat("%.1f", angleDegL),
                  &angleDegL, 0.0f, 180.0f
              );

              GuiSliderBar(
                  (Rectangle){60, 180, 200, 20},
                  "R - Angle", TextFormat("%.1f", angleDegR),
                  &angleDegR, 0.0f, 180.0f
              );

              GuiToggle(
                  (Rectangle){60,210,200,20},
                  "Wind",
                  &wind
              );

              if(GuiDropdownBox(
                  (Rectangle){60,240,200,20},
                  fractalList,
                  &fractalIndex,
                  fractalEdit
              )){
                fractalEdit = !fractalEdit;
              }
            }

            Vector2 start = {w/2,h-20};
            DrawBranch(start,length,0,thickness,ratio,(angleDegL)*DEG2RAD,(angleDegR)*DEG2RAD);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}