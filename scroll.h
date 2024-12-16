#ifndef SCROLL_H
#define SCROLL_H

#include <allegro5/allegro.h>
#include <string>

// Enumeración para el control del temporizador
enum class timerStartStop { START, STOP };

// Definición de la clase scrollArea
class scrollArea {
public:
    ALLEGRO_BITMAP* imagen;
    ALLEGRO_TIMER* timer;
    int x;
    int y;
    double velocidad;
    int scroll;

    scrollArea(std::string img, int xIni, int yIni, double t);
    ~scrollArea();
    void dibujaV();
    void calcularV();
    void dibujaH();
    void calcularH();
    void starStop(timerStartStop s);
    void incVelocidad();
    void decVelocidad();
};

#endif // SCROLL_H