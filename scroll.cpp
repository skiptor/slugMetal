#include "scroll.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

// Implementación de los métodos de la clase scrollArea
scrollArea::scrollArea(std::string img, int xIni, int yIni, double t) {
    imagen = al_load_bitmap(img.c_str());
    x = xIni;
    y = yIni;
    scroll = 0;
    velocidad = t;
    timer = al_create_timer(velocidad);
}

scrollArea::~scrollArea() {
    al_destroy_bitmap(imagen);
}

void scrollArea::dibujaV() {
    al_draw_bitmap(imagen, x, scroll, 0);
    al_draw_bitmap(imagen, x, scroll - al_get_bitmap_height(imagen), 0);
}

void scrollArea::calcularV() {
    scroll += 2;  // Incrementa el desplazamiento vertical
    if (scroll >= al_get_bitmap_height(imagen))  // Resetea la posición si se completa el ciclo
        scroll = 0;
}

void scrollArea::dibujaH()
{
    al_draw_bitmap(imagen, scroll, y, 0);
    al_draw_bitmap(imagen, scroll + al_get_bitmap_width(imagen), y, 0);
}

void scrollArea::calcularH()
{
    scroll -= 2; 
    if (scroll >= al_get_bitmap_width(imagen)) 
        scroll = 0;
    

}

void scrollArea::starStop(timerStartStop s) {
    if (s == timerStartStop::START)
        al_start_timer(timer);  // Inicia el temporizador
    else if (s == timerStartStop::STOP)
        al_stop_timer(timer);   // Detiene el temporizador
}

// Implementación de las funciones adicionales según sea necesario
void scrollArea::incVelocidad() {
    if (velocidad > 0.01) {
        velocidad -= 0.01;  // Aumenta la velocidad del scroll
        al_set_timer_speed(timer, velocidad);
    }
}

void scrollArea::decVelocidad() {
    if (velocidad < 0.9) {
        velocidad += 0.01;  // Disminuye la velocidad del scroll
        al_set_timer_speed(timer, velocidad);
    }
}


