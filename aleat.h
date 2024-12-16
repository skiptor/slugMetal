#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <iostream>
#include <list>

using namespace std;


#define ANCHO  640
#define ALTO  1100

#define VELOCIDAD 2

int aleatorio(int max);


class crater {
    
public:
    int x;
    int y;
    crater();
    void dibuja(ALLEGRO_BITMAP *c);
    void recalcular();
    bool visible();
};


class cola {
public:
    ALLEGRO_TIMER *timer;
    ALLEGRO_BITMAP *img;
    list<crater> crateres;

    cola(string s);
    void start();
    void push();
    void pop();
    bool siCrear();
    void dibuja();
    void recalcular();
};



