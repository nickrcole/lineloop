/**
 * @file lineloop.c
 * @date 12/30/23
 * @brief Controls render loop and communicates with driver
 * @author Nicholas Cole https://nicholascole.dev
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <sys/time.h>
#include <pthread.h>
#include <portaudio.h>
#include "./../include/render_toolkit.h"
#include "./../include/animations.h"
#include "./../include/audio.h"

const int FRAME_BUF_SIZE = sizeof(frame_buf);
const int COMPONENT_SIZE = sizeof(Component);
char* FONT_PATH = "/home/pi/lineloop/fonts/TNR.ttf";
Component* comp;

void show_image( frame_buf* frame )
{
  int  i, j;

  for ( i = 0; i < FRAME_BUF_WIDTH; i++ )
  {
    for ( j = 0; j < FRAME_BUF_HEIGHT; j++ )
      putchar( (*frame)[i][j] == 0 ? ' '
                                   : (*frame)[i][j] < 128 ? '+'
                                                          : '*' );
    putchar( '\n' );
  }
}

void render_loop( void ) {
    struct timeval start, render_end, frame_end;
    double total_frame_time;
    frame_buf* rendered_frame = malloc(FRAME_BUF_SIZE);
    memset(rendered_frame, 0, FRAME_BUF_SIZE);

    while (1) {
        gettimeofday(&start, NULL);

        for (int j = 0; j < MAX_COMPONENTS; j++) {
            if (!comp[j].rast) {
                continue;
            }
            // Prepare stage
            for (int i = 0; i < MAX_ANIMATIONS; i++) {
                if (!comp[j].animation[i]) {
                    break;
                }
                Point new_pos = comp[j].animation[i](comp[j].position, comp[j].speed);
                comp[j].position = new_pos;
            }

            // Render stage
            comp[j].frame = render_frame(comp);
        }

        // Layer stage
        rendered_frame = layer_components(comp);

        display_frame(rendered_frame);

        gettimeofday(&render_end, NULL);
        #if LIMIT_FRAMERATE
        if (render_end.tv_usec - start.tv_usec >= 0) {
            usleep(16666 - (render_end.tv_usec - start.tv_usec));  // Enforce framerate
        }
        #endif
        gettimeofday(&frame_end, NULL);
        total_frame_time = (frame_end.tv_usec - start.tv_usec);
        // printf("Frame rate: %d\n", 1000000 /(frame_end.tv_usec - start.tv_usec));
    }
}

void clear_display( void ) {
    frame_buf* clear = malloc(FRAME_BUF_SIZE);
    memset(clear, 0, FRAME_BUF_SIZE);
    display_frame(clear);
}

void display_predictions( void ) {
    int anim_count = 1;
    Color text_color;
    text_color.red = 0;
    text_color.green = 255;
    text_color.blue = 0;
    char* content = "CHAMPS: 20 mins    DOGGIE'S: 15 mins";
    Animation* animation = malloc(anim_count * sizeof(Animation));
    animation[0] = scroll_forward;
    comp[0] = *initialize_component( TEXT, content, animation, anim_count, &text_color, 0, NULL );
    render_loop();
}

void display_test( void ) {
    int anim_count = 1;
    char* content = "CHAMPS: 20 mins    DOGGIE'S: 15 mins";
    Animation* animation = malloc(anim_count * sizeof(Animation));
    // animation[0] = bob;
    // animation[0] = bar_jitter;
    Color bar_color;
    bar_color.red = 0;
    bar_color.green = 255;
    bar_color.blue = 0;
    animation[0] = scroll_backward;
    for (int i = 0; i < NUM_BARS; i++) {
        Point pos;
        pos.x = 2*i;
        pos.y = 14;
        comp[i] = *initialize_component( BAR, "./../images/star.png", animation, anim_count, &bar_color, 0, &pos );
    }
    render_loop();
}

void close_program( int signo ) {
    if (signo == SIGINT) {
        printf("Exiting\n");
        // clear_display();
        exit(0);
    }
}

void initialize_audio() {
    double* bands = malloc(sizeof(double) * NUM_BARS);
    audio_reactive_anim_init(bands);
    audio_init(bands);
}

void audio_reactive_init(pthread_t thread) {
    pthread_t audio_thread;
    pthread_create(&thread, NULL, initialize_audio, NULL);
}

int main(int argc, char** argv) {
    int args=0;
    signal(SIGINT, close_program);
    pthread_t loop_thread;
    pthread_t audio_thread;
    driver_init();
    audio_reactive_init(audio_thread);
    comp = malloc(MAX_COMPONENTS * sizeof(Component));
    memset(comp, 0, MAX_COMPONENTS * sizeof(Component));
    while (argc > ++args)               // Process command-line args
    {
        if (argv[args][0] == '-')
        {
            switch (toupper(argv[args][1]))
            {
            case 'C':                   // -C: clear the display
                clear_display();
                exit(0);
                break;
            case 'P':
                display_predictions();
                break;
            case 'T':
                if (pthread_create(&loop_thread, NULL, display_test, NULL) != 0) {
                    fprintf(stderr, "Error creating thread\n");
                    return 1; // Return an error code
                }
                break;
            default:                    // Otherwise error
                printf("Unrecognised option '%c'\n", argv[args][1]);
                printf("Options:\n"
                       "  -c     clear display\n"\
                       "  -p     diplay comp\n"\
                      );
                return(1);
            }
        }
    }
    pthread_join(loop_thread, NULL);
    pthread_join(audio_thread, NULL);
}

Component* initialize_component( COMP_TYPE type, char* content, Animation* animation, int num_anims, Color* color, int layer, Point* pos ) {
    Component* comp = malloc(COMPONENT_SIZE);
    comp->rast = malloc(FRAME_BUF_SIZE);
    comp->frame = malloc(FRAME_BUF_SIZE);
    comp->animation = malloc(MAX_ANIMATIONS * sizeof(Animation));
    memset(comp->rast, 0, FRAME_BUF_SIZE);
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        if (i < num_anims) {
            comp->animation[i] = animation[i];
        } else {
            comp->animation[i] = NULL;
        }
    }
    comp->speed = 0.45;
    comp->brightness = 0.1;
    if (!pos) {
        pos->x = 0;
        pos->y = 0;
    }
    comp->position = *pos;
    comp->content = content;
    comp->layer = layer;
    if (color) {
        comp->color_overlay = *color;
        comp->apply_color = 1;
    } else {
        comp->apply_color = 0;
    }
    switch (type) {
        case TEXT:
            rasterize_text(FONT_PATH, content, comp->rast);
            break;
        case IMAGE:
            rasterize_image(comp->rast, content);
            break;
        case BAR:
            frame_buf* bar_rast = malloc(FRAME_BUF_SIZE);
            int bitmap[294][15] = {
                                     {0xFFFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                                     {0xFFFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
                                  };
            memcpy(bar_rast, &bitmap, FRAME_BUF_SIZE);
            comp->rast = bar_rast;
            break;
    }
    return comp;
}