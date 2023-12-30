# FREETYPE:
sudo apt-get install libpng-dev
sudo apt-get install libharfbuzz-dev
sudo cp /usr/include/harfbuzz/* /usr/include/
sudo apt-get install libbz2-dev
make

# TEXT RENDERER:
gcc -o text_render text_render.c -Ifreetype/include -L/usr/lib/arm-linux-gnueabihf -lfreetype -lm

# DRIVER:
gcc -Wall -o rpi_pixleds rpi_pixleds.c rpi_dma_utils.c -I./include -fcommon