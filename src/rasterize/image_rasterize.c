#define STB_IMAGE_IMPLEMENTATION
#include "./../../include/stb_image.h"
#include "./../../include/render_toolkit.h"

const size_t NUM_PIXELS_TO_PRINT = 10U;
 
void rasterize_image( frame_buf* frame, char* image ) {
    int width, height, comp;

    // Load PNG image
    unsigned char *data = stbi_load(image, &width, &height, &comp, 0);
    if (data == NULL) {
        fprintf(stderr, "Error loading image\n");
        return;
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            // Calculate index for the current pixel
            int index = (y * width + x) * comp;

            // Print pixel values
            // printf("Pixel at (%d, %d): ", x, y);
            int pixel = data[index];
            pixel <<= 8;
            pixel += data[index + 1];
            pixel <<= 8;
            pixel += data[index + 2];
            (*frame)[x][y] = pixel;
            // for (int c = 0; c < comp; c++) {
            //     printf("%u ", data[index + c]);
            // }
            // printf("\n");
        }
    }

    // Free allocated memory
    stbi_image_free(data);
}