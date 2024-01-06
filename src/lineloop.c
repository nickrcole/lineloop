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
        memset(rendered_frame, 0, FRAME_BUF_SIZE);

        // Render Stage
        render(comp, rendered_frame);

        display_frame(rendered_frame);

        gettimeofday(&render_end, NULL);

        #if LIMIT_FRAMERATE
        // If the frame renders faster than the framerate allows, wait
        suseconds_t target_frame_time = 1000000.0 / MAX_FRAMERATE;
        suseconds_t render_time = render_end.tv_usec - start.tv_usec;
        if (0 < render_time && render_time < target_frame_time) {
            usleep(target_frame_time - render_time);
        }
        #endif

        gettimeofday(&frame_end, NULL);
        total_frame_time = (frame_end.tv_usec - start.tv_usec);
        // printf("Frame rate: %d\n", 1000000 / (frame_end.tv_usec - start.tv_usec));
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
    comp[0] = *initialize_component( TEXT, content, animation, anim_count, &text_color, NULL );
    render_loop();
}

void display_test( void ) {
    int anim_count = 1;
    char* content = "CHAMPS: 20 mins    DOGGIE'S: 15 mins";
    Color *text_color_1 = malloc(sizeof(Color));
    text_color_1->red = 0;
    text_color_1->green = 0;
    text_color_1->blue = 0xFF;
    Animation* animation = malloc(anim_count * sizeof(Animation));
    // animation[0] = bob;
    // animation[0] = bar_jitter;
    animation[0] = scroll_backward;
    Point pos;
    pos.x = 0;
    pos.y = 0;
    comp[0] = *initialize_component( BAR, "bars_component", animation, anim_count, NULL, &pos );
    animation[0] = scroll_forward;
    Color* text_color_2 = malloc(sizeof(Color));
    text_color_2->red = 0xFF;
    text_color_2->green = 0xFF;
    text_color_2->blue = 0xFF;
    // comp[1] = *initialize_component( TEXT, content, animation, 1, text_color_2, NULL );
    free(text_color_1);
    free(text_color_2);
    render_loop();
}

void close_program( int signo ) {
    if (signo == SIGINT) {
        printf("Exiting\n");
        // clear_display();
        exit(0);
    }
}

void audio_init_container() {
    double* bands = malloc(sizeof(double) * NUM_BARS);
    audio_reactive_init(bands);
    audio_driver_init(bands);
}

void audio_init(pthread_t thread) {
    pthread_t audio_thread;
    pthread_create(&thread, NULL, audio_init_container, NULL);
}

int main(int argc, char** argv) {
    int args=0;
    signal(SIGINT, close_program);
    pthread_t loop_thread;
    pthread_t audio_thread;
    driver_init();
    audio_init(audio_thread);
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

Component* initialize_component( COMP_TYPE type, char* content, Animation* animation, int num_anims, Color* color, Point* pos ) {
    Component* comp = malloc(COMPONENT_SIZE);
    comp->rast = malloc(FRAME_BUF_SIZE);
    // comp->frame = malloc(FRAME_BUF_SIZE);
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
        Point default_pos;
        default_pos.x = 0;
        default_pos.y = 1;
        comp->position = default_pos;
    } else {
        comp->position = *pos;
    }
    comp->content = content;
    if (color) {
        comp->color_overlay = *color;
        comp->apply_color = 1;
    } else {
        comp->apply_color = 0;
    }
    switch (type) {
        case TEXT:
            rasterize_text(FONT_PATH, content, comp->rast, comp->color_overlay, comp->brightness);
            break;
        case IMAGE:
            rasterize_image(comp->rast, content);
            break;
        case BAR:
            frame_buf* bar_rast = malloc(FRAME_BUF_SIZE);
            BarData* bars = malloc(sizeof(BarData) * NUM_BARS);
            comp->comp_data = bars;
            attach_bar_components((BarData*) comp->comp_data);     // Provides audio driver with the bar data array
            break;
    }
    return comp;
}