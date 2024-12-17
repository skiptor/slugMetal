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
const int FRAME_COUNT_RIGHT = 12;
const int FRAME_COUNT_LEFT = 12;
const int FRAME_COUNT_JUMP = 6;
const int FRAME_COUNT_CROUCH = 1;
const int FRAME_COUNT_IDLE = 20;
const float FRAME_DURATION = 0.1;

enum EstadoPersonaje {
    IDLE,
    MOVING_RIGHT,
    MOVING_LEFT,
    JUMPING,
    CROUCHING
};

int main() {
    int salir = 0;
    int cc = 0;
    int aux = 0;
    int aux2 = 0;
    srand((unsigned)time(NULL));
    bool teclas_press[ALLEGRO_KEY_MAX] = {false};

    if (!al_init() || !al_install_keyboard() || !al_init_image_addon() || !al_init_primitives_addon() ||
        !al_install_audio() || !al_init_acodec_addon() || !al_reserve_samples(1)) {
        cerr << "Error al inicializar Allegro o sus complementos." << endl;
        return -1;
    }

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        cerr << "Error al crear la ventana." << endl;
        return -1;
    }
    al_set_window_title(display, "Slug Metal");

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

    scrollArea fondo("img/fondo-inmovil.png", 0, 0, .1);
    scrollArea mapa("img/fondo-final.png", 0, 0, .025);
    if (fondo.timer) al_register_event_source(eventos, al_get_timer_event_source(fondo.timer));
    if (mapa.timer) al_register_event_source(eventos, al_get_timer_event_source(mapa.timer));

    ALLEGRO_BITMAP* frames_right[FRAME_COUNT_RIGHT];
    ALLEGRO_BITMAP* frames_left[FRAME_COUNT_LEFT];
    ALLEGRO_BITMAP* frames_jump[FRAME_COUNT_JUMP];
    ALLEGRO_BITMAP* frames_crouch[FRAME_COUNT_CROUCH];
    ALLEGRO_BITMAP* frames_idle[FRAME_COUNT_IDLE];
    ALLEGRO_BITMAP* principal = al_load_bitmap("img/personaje.png");

    for (int i = 0; i < FRAME_COUNT_RIGHT; ++i) {
        string filename = "walk/" + to_string(i) + ".png";
        frames_right[i] = al_load_bitmap(filename.c_str());
        if (!frames_right[i]) {
            cerr << "Error al cargar la imagen: " << filename << endl;
            return -1;
        }
    }

    for (int i = 0; i < FRAME_COUNT_LEFT; ++i) {
        string filename = "walk/" + to_string(i) + ".png";
        frames_left[i] = al_load_bitmap(filename.c_str());
        if (!frames_left[i]) {
            cerr << "Error al cargar la imagen: " << filename << endl;
            return -1;
        }
    }

    for (int i = 0; i < FRAME_COUNT_JUMP; ++i) {
        string filename = "jump/" + to_string(i) + ".png";
        frames_jump[i] = al_load_bitmap(filename.c_str());
        if (!frames_jump[i]) {
            cerr << "Error al cargar la imagen: " << filename << endl;
            return -1;
        }
    }

    for (int i = 0; i < FRAME_COUNT_CROUCH; ++i) {
        string filename = "crouch/" + to_string(i) + ".png";
        frames_crouch[i] = al_load_bitmap(filename.c_str());
        if (!frames_crouch[i]) {
            cerr << "Error al cargar la imagen: " << filename << endl;
            return -1;
        }
    }

    for (int i = 0; i < FRAME_COUNT_IDLE; ++i) {
        string filename = "idle/" + to_string(i) + ".png";
        frames_idle[i] = al_load_bitmap(filename.c_str());
        if (!frames_idle[i]) {
            cerr << "Error al cargar la imagen: " << filename << endl;
            return -1;
        }
    }

    ALLEGRO_SAMPLE *sample = al_load_sample("audio/suelo.wav");

    float pos_x = (SCREEN_WIDTH / 2.0) - 740.0;
    float pos_y = SCREEN_HEIGHT - 500;
    float vel_y = 0;
    bool saltando = false;
    bool disparar = false;
    bool redraw = false;
    int current_frame = 0;
    EstadoPersonaje estado = IDLE;

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

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            teclas_press[evento.keyboard.keycode] = true;
        } else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            teclas_press[evento.keyboard.keycode] = false;
        }

        if (evento.type == ALLEGRO_EVENT_TIMER) {
            if (evento.timer.source == gravedad) {
                bool moving = false;


                if (teclas_press[ALLEGRO_KEY_RIGHT] && pos_x <= 1870) {
                    pos_x += 7;
                    estado = MOVING_RIGHT;
                    moving = true;
                }
                if (teclas_press[ALLEGRO_KEY_LEFT] && pos_x >= 0) {
                    pos_x -= 7;
                    estado = MOVING_LEFT;
                    moving = true;
                }
                if (teclas_press[ALLEGRO_KEY_UP] && !saltando) {
                    saltando = true;
                    vel_y = JUMP_SPEED;
                    estado = JUMPING;
                    moving = true;
                }
                if (teclas_press[ALLEGRO_KEY_SPACE] && !disparar) {
                    disparar = true;
                    aux=pos_x;
                    aux2=pos_y;
                }
                if (teclas_press[ALLEGRO_KEY_DOWN]) {
                    estado = CROUCHING;
                    moving = true;
                }

                if (!moving) {
                    estado = IDLE;
                }

                if (pos_y >= SCREEN_HEIGHT) {
                    salir = 1;
                }

                pos_y += vel_y;
                vel_y += GRAVITY;

                if (saltando) {
                    if (c != -1) {
                        if (vel_y > 0) {
                            vel_y = 0;
                            pos_y = piso.muros[c].y - 25;
                            saltando = false;
                            al_play_sample(sample, 1.0, 0.1, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
                    }

                    if (pos_y >= SCREEN_HEIGHT) {
                        vel_y = 0;
                        pos_y = SCREEN_HEIGHT + 10;
                        saltando = false;
                    }
                } else {
                    if (c != -1) {
                        vel_y = 0;
                        pos_y = piso.muros[c].y - 25;
                    }
                }

                redraw = true;
            } else if (evento.timer.source == anim_timer) {
                current_frame++;
                switch (estado) {
                    case MOVING_RIGHT:
                        current_frame %= FRAME_COUNT_RIGHT;
                        break;
                    case MOVING_LEFT:
                        current_frame %= FRAME_COUNT_LEFT;
                        break;
                    case JUMPING:
                        current_frame %= FRAME_COUNT_JUMP;
                        break;
                    case CROUCHING:
                        current_frame %= FRAME_COUNT_CROUCH;
                        break;
                    case IDLE:
                    default:
                        current_frame %= FRAME_COUNT_IDLE;
                        break;
                }
            }
            if (evento.timer.source == fondo.timer) {
                    fondo.calcularH();
                }
                if (evento.timer.source == mapa.timer) {
                    mapa.calcularH();
                    cc += 2;
                }
        }

        if (redraw && al_is_event_queue_empty(eventos)) {
            redraw = false;
            per.mover(pos_x + cc, pos_y);
            al_clear_to_color(al_map_rgb(255, 255, 255));
            fondo.dibujaH();
            mapa.dibujaH();
            piso.dibujar(c, cc);
            if(disparar){
                al_draw_bitmap(principal, aux+(cc*3), aux , 0);
                if(aux+(cc*4)>SCREEN_WIDTH){
                    disparar=false;
                }
            }

            // Dibujar la animación correspondiente según el estado del personaje
            switch (estado) {
                case MOVING_RIGHT:
                    al_draw_bitmap(frames_right[current_frame], pos_x - 10, pos_y - 20, 0);
                    break;
                case MOVING_LEFT:
                    al_draw_bitmap(frames_left[current_frame], pos_x - 10, pos_y - 20, 0);
                    break;
                case JUMPING:
                    al_draw_bitmap(frames_jump[current_frame], pos_x - 10, pos_y - 20, 0);
                    break;
                case CROUCHING:
                    al_draw_bitmap(frames_crouch[current_frame], pos_x - 10, pos_y - 20, 0);
                    break;
                case IDLE:
                default:
                    al_draw_bitmap(frames_idle[current_frame], pos_x - 10, pos_y - 20, 0);
                    break;
            }

            al_flip_display();
        }
    }

    // Liberar recursos
    for (int i = 0; i < FRAME_COUNT_RIGHT; ++i) {
        al_destroy_bitmap(frames_right[i]);
    }
    for (int i = 0; i < FRAME_COUNT_LEFT; ++i) {
        al_destroy_bitmap(frames_left[i]);
    }
    for (int i = 0; i < FRAME_COUNT_JUMP; ++i) {
        al_destroy_bitmap(frames_jump[i]);
    }
    for (int i = 0; i < FRAME_COUNT_CROUCH; ++i) {
        al_destroy_bitmap(frames_crouch[i]);
    }
    for (int i = 0; i < FRAME_COUNT_IDLE; ++i) {
        al_destroy_bitmap(frames_idle[i]);
    }
    al_destroy_sample(sample);
    al_destroy_event_queue(eventos);
    al_destroy_display(display);
    al_destroy_timer(gravedad);
    al_destroy_timer(anim_timer);
    al_uninstall_audio();

    return 0;
}