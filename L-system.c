#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 950
#define HEIGHT 650

// Base Grammar Used :
/*
    S -> FB
    F -> FF
    B -> [lFB][rFB] | FB
*/

Vector2 DrawF(Vector2 start,float length,float angle,float thickness,float wind,float baseThickness) {

  float sway = (1.0f - thickness/baseThickness);
  float windOffset = wind*sway;

  float x_mid = start.x + sinf(angle+windOffset)*length;
  float y_mid = start.y - cosf(angle+windOffset)*length;
  Vector2 mid = {x_mid,y_mid};

  float x_end = mid.x + sinf(angle+windOffset)*length;
  float y_end = mid.y - cosf(angle+windOffset)*length;
  Vector2 end = {x_end,y_end};

  DrawLineEx(start,mid,thickness,RAYWHITE);
  DrawLineEx(mid,end,thickness,RAYWHITE);
  return end;
}

void DrawB(Vector2 start,float length,float angle,float thickness,float ratio,float branch_angle_l,float branch_angle_r,float wind,float baseThickness,float branchProb) {
  if(length < 1) return;

  float new_length = length*ratio;
  float new_thickness = thickness*ratio;

  float r = (float)rand() / RAND_MAX;

  if(r <= branchProb) {
    // lFB
    float new_angle_l = angle + branch_angle_l;
    Vector2 leftEnd = DrawF(start,length,new_angle_l,thickness,wind,baseThickness);
    DrawB(leftEnd,new_length,new_angle_l,new_thickness,ratio,branch_angle_l,branch_angle_r,wind,baseThickness,branchProb);

    // rFB
    float new_angle_r = angle - branch_angle_r;
    Vector2 RightEnd = DrawF(start,length,new_angle_r,thickness,wind,baseThickness);
    DrawB(RightEnd,new_length,new_angle_r,new_thickness,ratio,branch_angle_l,branch_angle_r,wind,baseThickness,branchProb);
  } else{
    Vector2 end = DrawF(start,length,angle,thickness,wind,baseThickness);
    DrawB(end,new_length,angle,new_thickness,ratio,branch_angle_l,branch_angle_r,wind,baseThickness,branchProb);
  }


}

void DrawTree(Vector2 start,float length,float angle,float thickness,float ratio,float branch_angle_l,float branch_angle_r,float wind,float baseThickness,float branchProb) {

  Vector2 end = DrawF(start,length,angle,thickness,wind,baseThickness);

  float new_length = length*ratio;
  float new_thickness = thickness*ratio;
  DrawB(end,new_length,angle,new_thickness,ratio,branch_angle_l,branch_angle_r,wind,baseThickness,branchProb);
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

    // randomness
    float branchProbability = 0.7f;

    // dropdown params
    int fractalIndex = 0;
    bool fractalEdit = false;
    const char * fractalList = "Tree;Julia-Set;L-System;Fern";

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {

        int w = GetScreenWidth();
        int h = GetScreenHeight();
        BeginDrawing();
            ClearBackground(BLACK);

            int l = 0;

            GuiCheckBox(
                (Rectangle){120,l+=10,15,15},
                "Symmetry",
                &symmetric
            );

            GuiLabel((Rectangle){120, l+=20, 200, 20}, "L - Sys Parameters");

            GuiSliderBar(
                (Rectangle){60, l+=20, 200, 20},
                "Length", TextFormat("%.1f", length),
                &length, 50.0f, 500.0f
            );

            GuiSliderBar(
                (Rectangle){60, l+=30, 200, 20},
                "Thickness", TextFormat("%.1f", thickness),
                &thickness, 1.0f, 50.0f
            );

            GuiSliderBar(
                (Rectangle){60, l+=30, 200, 20},
                "Ratio", TextFormat("%.2f", ratio),
                &ratio, 0.5f, 0.75f
            );

            if(symmetric) {
              GuiSliderBar(
                  (Rectangle){60, l+=30, 200, 20},
                  "Angle", TextFormat("%.1f", angleDegL),
                  &angleDegL, 0.0f, 180.0f
              );

              angleDegR = angleDegL;
            }

            else{
              GuiSliderBar(
                  (Rectangle){60, l+=30, 200, 20},
                  "L - Angle", TextFormat("%.1f", angleDegL),
                  &angleDegL, 0.0f, 180.0f
              );

              GuiSliderBar(
                  (Rectangle){60, l+=30, 200, 20},
                  "R - Angle", TextFormat("%.1f", angleDegR),
                  &angleDegR, 0.0f, 180.0f
              );
            }

            GuiSliderBar(
                (Rectangle){60, l+=30, 200, 20},
                "Branch %",
                TextFormat("%.0f%%", branchProbability * 100),
                &branchProbability,
                0.0f, 1.0f
            );

            GuiToggle(
                  (Rectangle){60,l+=30,200,20},
                  "Wind",
                  &wind
            );

              if(GuiDropdownBox(
                  (Rectangle){60,l+=30,200,20},
                  fractalList,
                  &fractalIndex,
                  fractalEdit
              )){
                fractalEdit = !fractalEdit;
            }

            float time = GetTime();

            float windStrength = 0.15f;
            float windSpeed = 1.5f;

            float windValue = 0.0f;
            if(wind){
                windValue = sinf(time * windSpeed) * windStrength;
            }

            Vector2 start = {w/2,h-20};
            srand(42);
            DrawTree(start,length,0,thickness,ratio,(angleDegL)*DEG2RAD,(angleDegR)*DEG2RAD,windValue,thickness,branchProbability);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}