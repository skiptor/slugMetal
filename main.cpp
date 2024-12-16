#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include <time.h>
#include "scroll.h"
#include "colision.h"

using namespace std;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const float GRAVITY = 0.5f;
const float JUMP_SPEED = -10.0f;
const float FRAME_DURATION = 0.15; // Duración de cada frame en segundos

int main() {
    int salir = 0;
    int cc = 0;
    srand((unsigned)time(NULL));
    bool teclas_press[ALLEGRO_KEY_MAX] = {false}; // Inicializar teclas en false

    // Inicializar Allegro
    if (!al_init() || !al_install_keyboard() || !al_init_image_addon() || !al_init_primitives_addon() ||
        !al_install_audio() || !al_init_acodec_addon() || !al_reserve_samples(1)) {
        cerr << "Error al inicializar Allegro o sus complementos." << endl;
        return -1;
    }

    // Crear display
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        cerr << "Error al crear la ventana." << endl;
        return -1;
    }
    al_set_window_title(display, "Slug Metal");

    // Crear timers y cola de eventos
    ALLEGRO_TIMER* gravedad = al_create_timer(1.0 / 60);
    ALLEGRO_TIMER* anim_timer = al_create_timer(FRAME_DURATION);
    ALLEGRO_EVENT_QUEUE* eventos = al_create_event_queue();
    if (!gravedad || !anim_timer || !eventos) {
        cerr << "Error al crear temporizadores o eventos." << endl;
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(eventos, al_get_keyboard_event_source());
    al_register_event_source(eventos, al_get_display_event_source(display));
    al_register_event_source(eventos, al_get_timer_event_source(gravedad));
    al_register_event_source(eventos, al_get_timer_event_source(anim_timer));

    // Inicializar fondos y personaje
    scrollArea fondo("img/fondo-inmovil.png", 0, 0, .1);
    scrollArea mapa("img/fondo-final.png", 0, 0, .05);
    if (fondo.timer) al_register_event_source(eventos, al_get_timer_event_source(fondo.timer));
    if (mapa.timer) al_register_event_source(eventos, al_get_timer_event_source(mapa.timer));

    // Cargar imágenes de la animación
    ALLEGRO_BITMAP* walk[12];
    for (int i = 0; i < 12; ++i) {
        string filename = "walk/" + to_string(i) + ".png";
        frames[i] = al_load_bitmap(filename.c_str());
        if (!frames[i]) {
            cerr << "Error al cargar la imagen: " << filename << endl;
            return -1;
        }
    }
    ALLEGRO_SAMPLE *sample = al_load_sample("audio/suelo.wav");

    // Posición y velocidad del personaje
    float pos_x = (SCREEN_WIDTH / 2.0) - 740.0;
    float pos_y = SCREEN_HEIGHT - 500;
    float vel_y = 0;
    bool saltando = false;
    bool redraw = false;
    int current_frame = 0;

    // Inicializar laberinto
    laberinto piso = laberinto();
    personaje per = personaje(1, 1);

    fondo.starStop(timerStartStop::START);
    mapa.starStop(timerStartStop::START);
    al_start_timer(gravedad);
    al_start_timer(anim_timer);

    while (!salir) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(eventos, &evento);

        int c = per.colision(piso.muros);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            salir = 1;
        }

        // Control de teclas
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            teclas_press[evento.keyboard.keycode] = true;
        } else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            teclas_press[evento.keyboard.keycode] = false;
        }

        // Lógica principal
        if (evento.type == ALLEGRO_EVENT_TIMER) {
            if (evento.timer.source == gravedad) {
                if (teclas_press[ALLEGRO_KEY_RIGHT] && pos_x <= 1870) {
                    pos_x += 7;
                }
                if (teclas_press[ALLEGRO_KEY_LEFT] && pos_x >= 0) {
                    pos_x -= 7;
                }
                
                if (teclas_press[ALLEGRO_KEY_SPACE] && !saltando) {
                    saltando = true;
                    vel_y = JUMP_SPEED;
                }

                if(pos_y>=SCREEN_HEIGHT){
                    salir=1;
                }

                
                pos_y += vel_y;  // Movimiento vertical
                vel_y += GRAVITY;
                // Salto y gravedad
                if (saltando) {
                    

                    if (c != -1) { // Colisión detectada
                        if (vel_y > 0) { // Sólo ajustar si cae, no si sube
                            vel_y = 0;
                            pos_y = piso.muros[c].y - 25; // Ajustar posición sobre el muro
                            saltando = false;
                            al_play_sample(sample, 1.0, 0.1, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
                    }

                    if (pos_y >= SCREEN_HEIGHT) { // Límite inferior
                        
                        vel_y = 0;
                        pos_y = SCREEN_HEIGHT + 10;
                        saltando = false;
                    }
                } else {
                    if (c != -1) { // Colisión detectada
                        vel_y = 0;
                        pos_y = piso.muros[c].y - 25; // Ajustar posición sobre el muro
                    }
                }

                if (evento.timer.source == fondo.timer) {
                    fondo.calcularH();
                }
                if (evento.timer.source == mapa.timer) {
                    mapa.calcularH();
                    cc += 2;
                }

                redraw = true;
            } else if (evento.timer.source == anim_timer) {
                current_frame = (current_frame + 1) % 12;
            }
        }

        // Dibujar
        if (redraw && al_is_event_queue_empty(eventos)) {
            redraw = false;
            per.mover(pos_x+cc,pos_y);
            al_clear_to_color(al_map_rgb(255, 255, 255));
            fondo.dibujaH();
            mapa.dibujaH();
            piso.dibujar(c, cc);
            al_draw_bitmap(walk[current_frame], pos_x-10, pos_y-20, 0);
            al_flip_display();
        }
    }

    // Liberar recursos
    for (int i = 0; i < FRAME_COUNT; ++i) {
        al_destroy_bitmap(frames[i]);
    }
    al_destroy_sample(sample);
    al_destroy_event_queue(eventos);
    al_destroy_display(display);
    al_destroy_timer(gravedad);
    al_destroy_timer(anim_timer);
    al_uninstall_audio();

    return 0;
}