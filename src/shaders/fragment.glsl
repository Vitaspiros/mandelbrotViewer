#version 440 core
in vec3 colorVertex;
out vec4 fragmentColor;

uniform float fractalScalar;
uniform float zoomingOffsetX;
uniform float zoomingOffsetY;

const int ITERATIONS = 500;

// Thanks to https://stackoverflow.com/a/17897228
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

int mandelbrot(int iter, vec2 pixel) {
    // convert pixel to range (0, 2)
    vec2 c = vec2(pixel.x * fractalScalar*2 + zoomingOffsetX, pixel.y * fractalScalar*2 - zoomingOffsetY);

    // I use vec2 to represent complex numbers, where x is the real part and y the imaginary part
    vec2 z = vec2(0.0);
    for (int i = 0; i < iter; i++) {
        // square z
        float x = z.x;
        z.x = x * x - z.y * z.y;
        z.y = x * z.y * 2;

        // add z + c
        z += c;

        // check if z has reached infinity
        if (isinf(z.x) || isinf(z.y)) return i;
    }
    return 1;
}

void main() {
    vec2 uv = vec2(colorVertex.xy);
    int exitedOnIteration = mandelbrot(ITERATIONS, uv);
    float clampedColor = exitedOnIteration/5 * 0.1;
    vec3 hsvColor = hsv2rgb(vec3(clampedColor, 0.5, (clampedColor >= 0.9 ? 0.9 : clampedColor)));
    fragmentColor = vec4(hsvColor, 1.0);
}