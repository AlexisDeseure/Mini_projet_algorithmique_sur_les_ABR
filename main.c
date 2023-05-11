#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp4.h"


int main() {
    char a[92] = "AghhgftvfvgGYUFYTyuyufg545645645+-+-+-...A,,,;;:;;,;:,zeygfyuzYGYUGUYGYUuyidfFTGgvhbjhjghjA";
    ignorerCasse(a);
    printf("mot : %s", a);
    return 0;
}
