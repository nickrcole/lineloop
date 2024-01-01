/*
text_render.c

Rasterizes text into a 2D array to be passed into LineLoop
Nicholas Cole — 12/30/23
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <ft2build.h>
#include "./../../include/render_toolkit.h"
#include FT_FREETYPE_H


#define WIDTH   15
#define HEIGHT  294

void
draw_bitmap( FT_Bitmap*  bitmap,
             FT_Int      x,
             FT_Int      y,
             frame_buf*  frame)
{
  FT_Int  i, j, p, q;
  FT_Int  x_max = x + bitmap->width;
  FT_Int  y_max = y + bitmap->rows;


  /* for simplicity, we assume that `bitmap->pixel_mode' */
  /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */

  for ( i = x, p = 0; i < x_max; i++, p++ )
  {
    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
      if ( i < 0      || j < 0       ||
           i >= WIDTH || j >= HEIGHT )
        continue;

      (*frame)[j][i] |= bitmap->buffer[q * bitmap->width + p];
    }
  }
}

// void
// show_image( frame_buf* frame )
// {
//   int  i, j;


//   for ( i = 0; i < HEIGHT; i++ )
//   {
//     for ( j = 0; j < WIDTH; j++ )
//       putchar( (*frame)[i][j] == 0 ? ' '
//                                 : (*frame)[i][j] < 128 ? '+'
//                                                     : '*' );
//     putchar( '\n' );
//   }
// }

void flip_bitmap( frame_buf* frame ) {
    int i, j, k;

    for ( k = 0; k < HEIGHT; k++ ) {
        for (i = 0, j = WIDTH - 1; i < j; i++, j--) {
            int temp = (*frame)[k][i];
            (*frame)[k][i] = (*frame)[k][j];
            (*frame)[k][j] = temp;
        }
    }
}


int
rasterize_text( char*      filename,
                char*      text,
                frame_buf* frame)
{
  FT_Library    library;
  FT_Face       face;

  FT_GlyphSlot  slot;
  FT_Matrix     matrix;                 /* transformation matrix */
  FT_Vector     pen;                    /* untransformed origin  */
  FT_Error      error;

  double        angle;
  int           target_height;
  int           n, num_chars;

  num_chars     = strlen( text );
  angle         = ( -90.0 / 360 ) * 3.14159 * 2;
  target_height = HEIGHT;

  error = FT_Init_FreeType( &library );              /* initialize library */
  error = FT_New_Face( library, filename, 0, &face );
  error = FT_Set_Char_Size( face, 15 * 64, 0,
                            75, 0 );                /* set character size */

  slot = face->glyph;

  /* set up matrix */
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

  /* the pen position in 26.6 cartesian space coordinates; */
  pen.x = 3 * 64;
  pen.y = ( target_height - 2 ) * 64;

  for ( n = 0; n < num_chars; n++ )
  {
    /* set transformation */
    FT_Set_Transform( face, &matrix, &pen );

    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
    if ( error )
      continue;                 /* ignore errors */

    /* now, draw to our target surface (convert position) */
    draw_bitmap( &slot->bitmap,
                 slot->bitmap_left,
                 target_height - slot->bitmap_top,
                 frame );

    /* increment pen position */
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }

  // Flip bitmap on the y axis
  flip_bitmap( frame );
  // show_image( frame );
  FT_Done_Face    ( face );
  FT_Done_FreeType( library );

  return 0;
}

/* EOF */