#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/mouse.h>

#include <iostream>
#include <list>

using namespace std;

struct rect {
    int x;
    int y;
    int w;
    int h;
}; 


bool colision(rect a, rect b);
int  colisionList(rect a, rect r[]);


class laberinto {

public:
   rect muros[81];

   laberinto();    
   void agregarMuro(int m, int x1, int y1, int w1, int h1);
   void dibujar(int m, int timer);
};

//----------------------------------------------------------

class personaje {
   int x;
   int y;

public:   
   personaje(int x1, int y1);
   void dibujar();
   void mover(int x1, int y1);
   int colision(rect r[]);
};


