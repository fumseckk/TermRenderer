#include "Buffer2D.h"
#include "../../headers/debug.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// --------- Private functions ---------

bool _check_out_of_bound(Buffer2D buff, int x, int y);


// --------- Function implementation ---------


Buffer2D create_buffer2d(int width, int height, int data_size) {
    Buffer2D buff = malloc(sizeof(Buffer2D));
    FATAL_ERROR(buff == NULL, "Could not allocate %li bytes of memory for buffer ptr", sizeof(Buffer2D));
    buff->data = calloc(width * height, data_size);
    FATAL_ERROR(buff->data == NULL, "Could not allocate %i bytes of memory for buffer", width * height * data_size);

    buff->width = width;
    buff->height = height;
    buff->data_size = data_size;
    return buff;
}


void resize_buffer2d(Buffer2D buff, int new_width, int new_height) {
    buff->width = new_width;
    buff->height = new_height;
    buff->data = realloc(buff->data, new_width * new_height * buff->data_size);
}


void realloc_buffer2d(Buffer2D buff, int new_width, int new_height, int new_data_size) {
    buff->width = new_width;
    buff->height = new_height;
    buff->data_size = new_data_size;
    buff->data = realloc(buff->data, new_width * new_height * new_data_size);
}


void free_buffer2d(Buffer2D buff) {
    free(buff->data);
    free(buff);
}


void* buffer2d_get(Buffer2D buff, int x, int y) {
    if (_check_out_of_bound(buff, x, y)) return NULL;
    return buff->data + (x + y * buff->width) * buff->data_size;
}


void buffer2d_set(Buffer2D buff, int x, int y, const void* data) {
    if (_check_out_of_bound(buff, x, y)) return;
    memcpy(buffer2d_get(buff, x, y), data, buff->data_size);
}


void buffer2d_fill(Buffer2D buff, const void* data) {
    for (int i = 0; i < buff->width * buff->height; i++) {
        memcpy(buff->data + i * buff->data_size, data, buff->data_size);
    }
}


void buffer2d_copy(Buffer2D dest, const Buffer2D src) {
    WARNING(dest->data_size != src->data_size || dest->width != src->width || dest->height != src->height,
            "Copy into buffer of different size");
    memcpy(dest->data, src->data, dest->width * dest->height * dest->data_size);
}


int buffer2d_get_width(Buffer2D buff) {
    return buff->width;
}


int buffer2d_get_height(Buffer2D buff) {
    return buff->height;
}


bool _check_out_of_bound(Buffer2D buff, int x, int y) {
    bool error = x < 0 || y < 0 || x >= buff->width || y >= buff->height;
    WARNING(error, "Buffer index out of bound (%i, %i) in buffer of size (%i, %i)", x, y, buff->width, buff->height);
    return error;
}