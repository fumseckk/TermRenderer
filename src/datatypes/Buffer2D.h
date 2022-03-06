#ifndef BUFFER2D_H
#define BUFFER2D_H


struct _buffer2d {
    void* data;
    int width;
    int height;
    int data_size;
};

typedef struct _buffer2d* Buffer2D;

Buffer2D create_buffer2d(int width, int height, int data_size);
void resize_buffer2d(Buffer2D buff, int new_width, int new_height);
void realloc_buffer2d(Buffer2D buff, int new_width, int new_height, int new_data_size);
void free_buffer2d(Buffer2D buff);

void* buffer2d_get(Buffer2D buff, int x, int y);
void buffer2d_set(Buffer2D buff, int x, int y, const void* data);

void buffer2d_fill(Buffer2D buff, const void* data);
void buffer2d_copy(Buffer2D dest, const Buffer2D src);

int buffer2d_get_width(Buffer2D buff);
int buffer2d_get_height(Buffer2D buff);


#endif // #ifndef BUFFER2D_H