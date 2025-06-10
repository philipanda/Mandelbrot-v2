# Mandelbrot-v2
Mandelbrot set viewer in SDL

# Reuquirements
- SDL2
 - Fedora: dnf install SDL2 SDL2-devel
- SDL2_image
 - Fedora: dnf install SDL2_image  SDL2_image-devel
# Building

```shell
make
```

# Running

```shell
./mandelbrot
```

# Controls

Arrows - move the camera
Q - Increase resolution
A - Decreaset resolution
W - Zoom in
S - Zoom out
E - Increase precision
D - Decrease precision
P - Make a screenshot in high resolution of the current screen. The precision and zoom level are kept, only the resolution is increased. File will be saved at `$PWD/screenshot.jpg`
