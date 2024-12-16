#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/mouse.h>

#include <iostream>

#include "colision.h"

using namespace std;



bool colision(rect a, rect b) 
{
    return a.x < b.x + b.w &&
           a.x + a.w > b.x &&
           a.y < b.y + b.h &&
           a.y + a.h > b.y;
}


int  colisionList(rect a, rect r[], int len)
{
    for (int i = 0; i <= len; i++)
       if (colision(a, r[i])){
        cout << "Colision detectada en el muro: "<<i <<endl;
        return i;
       }

    return -1;
}

//-----------------------------------------------------------------------------
laberinto::laberinto()
{
    agregarMuro(0, 5, 996, 1320, 5); 
    agregarMuro(1, 835, 844, 273, 5); 
    agregarMuro(2, 1320, 965, 5, 40);
    agregarMuro(3, 1320, 965, 40, 5);
    agregarMuro(4, 1355, 936, 5, 40); 
    agregarMuro(5, 1355, 936, 40, 5);
    agregarMuro(6, 1390, 915, 5, 40);
    agregarMuro(7, 1390, 915, 40, 5); 
    agregarMuro(8, 1425, 890, 5, 40); 
    agregarMuro(9, 1425, 890, 242, 5); 
    agregarMuro(10, 1950, 891, 560, 5); 
    agregarMuro(11, 2509, 891, 5, 16);
    agregarMuro(12, 2509, 907, 32, 5);
    agregarMuro(13, 2536, 907, 5, 16);
    agregarMuro(14, 2536, 923, 32, 5);
    agregarMuro(15, 2567, 923, 5, 16);
    agregarMuro(16, 2567, 939, 32, 5);
    agregarMuro(17, 2598, 939, 5, 16);
    agregarMuro(18, 2598, 955, 32, 5);
    agregarMuro(19, 2629, 955, 5, 16);
    agregarMuro(20, 2629, 971, 32, 5);
    agregarMuro(21, 2658, 971, 5, 16);
    agregarMuro(22, 2658, 986, 157, 5);
    agregarMuro(23, 2815, 952, 5, 34);
    agregarMuro(24, 2815, 952, 16, 5);

    agregarMuro(25, 300, 270, 5, 10);
    agregarMuro(26, 260, 190, 5, 10);
    agregarMuro(27, 200, 250, 5, 10);
    agregarMuro(28, 150, 200, 5, 10);
    agregarMuro(29, 250, 290, 5, 10);
    agregarMuro(30, 180, 230, 5, 10);
    agregarMuro(31, 130, 260, 5, 10);
    agregarMuro(32, 290, 170, 5, 10);
    agregarMuro(33, 220, 210, 5, 10);
    agregarMuro(34, 160, 200, 5, 10);
    agregarMuro(35, 300, 240, 5, 10);
    agregarMuro(36, 270, 300, 5, 10);
    agregarMuro(37, 200, 190, 5, 10);
    agregarMuro(38, 140, 260, 5, 10);
    agregarMuro(39, 250, 290, 5, 10);
    agregarMuro(40, 180, 230, 5, 10);
    agregarMuro(41, 130, 260, 5, 10);
    agregarMuro(42, 290, 170, 5, 10);
    agregarMuro(43, 220, 210, 5, 10);
    agregarMuro(44, 160, 200, 5, 10);
    agregarMuro(45, 300, 240, 5, 10);
    agregarMuro(46, 270, 300, 5, 10);
    agregarMuro(47, 200, 190, 5, 10);
    agregarMuro(48, 140, 260, 5, 10);
    agregarMuro(49, 250, 290, 5, 10);
    agregarMuro(50, 180, 230, 5, 10);
    agregarMuro(51, 130, 260, 5, 10);
    agregarMuro(52, 290, 170, 5, 10);
    agregarMuro(53, 220, 210, 5, 10);
    agregarMuro(54, 160, 200, 5, 10);
    agregarMuro(55, 300, 240, 5, 10);
    agregarMuro(56, 270, 300, 5, 10);
    agregarMuro(57, 200, 190, 5, 10);
    agregarMuro(58, 140, 260, 5, 10);
    agregarMuro(59, 250, 290, 5, 10);
    agregarMuro(60, 180, 230, 5, 10);
    agregarMuro(61, 130, 260, 5, 10);
    agregarMuro(62, 290, 170, 5, 10);
    agregarMuro(63, 220, 210, 5, 10);
    agregarMuro(64, 160, 200, 5, 10);
    agregarMuro(65, 300, 240, 5, 10);
    agregarMuro(66, 270, 300, 5, 10);
    agregarMuro(67, 200, 190, 5, 10);
    agregarMuro(68, 140, 260, 5, 10);
    agregarMuro(69, 250, 290, 5, 10);
    agregarMuro(70, 180, 230, 5, 10);
    agregarMuro(71, 130, 260, 5, 10);
    agregarMuro(72, 290, 170, 5, 10);
    agregarMuro(73, 220, 210, 5, 10);
    agregarMuro(74, 160, 200, 5, 10);
    agregarMuro(75, 300, 240, 5, 10);
    agregarMuro(76, 270, 300, 5, 10);
    agregarMuro(77, 200, 190, 5, 10);
    agregarMuro(78, 140, 260, 5, 10);
    agregarMuro(79, 250, 290, 5, 10);
    agregarMuro(80, 180, 230, 5, 10);
}


void laberinto::agregarMuro(int m, int x1, int y1, int w1, int h1)
{
   muros[m].x = x1;
   muros[m].y = y1;
   muros[m].w = w1;
   muros[m].h = h1;
}


void laberinto::dibujar(int m, int timer)
{
    for (int i = 0; i <= 80; i++)
    {
        ALLEGRO_COLOR color = (i == m) ? al_map_rgb(255,0,0) : al_map_rgb(0,0,0); 
        al_draw_filled_rectangle(muros[i].x-timer, muros[i].y, muros[i].x-timer + muros[i].w, muros[i].y + muros[i].h, color);
    }
}

//----------------------------------------------------------------------

personaje::personaje(int x1, int y1)
{
   x = x1;
   y = y1;
}

void personaje::dibujar()
{
   al_draw_filled_triangle(x + 10, y, x, y + 20, x + 20, y + 20, al_map_rgb(0,255,0));   
}

void personaje::mover(int x1, int y1)
{
   x = x1;
   y = y1;
}

int personaje::colision(rect r[])
{
    rect a = {x, y, 50, 50};
    return colisionList(a, r, 80);
}

