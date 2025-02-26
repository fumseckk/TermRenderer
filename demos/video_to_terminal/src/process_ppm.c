#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include "header.h"

#define PPMREADBUFLEN 256
char _files_path[300];

Image* read_im(char* file) {
  char buff[50], type[3];
  Image* im = malloc(sizeof(Image));

  FILE* fp = fopen(file, "rb");
  if (fp == NULL) {
    printf("Couldn't open file %s\n", file);
    exit(EXIT_FAILURE);
  }

  // Image allocation

  fscanf(fp, "%2s", type);
  type[2] = '\0';
  if (strcmp(type, "P6") != 0) {  // Make sure we have a P6 file
    printf("This file is not of type P6");
    return 0;
  }

  // Read cols and rows of image + useless stat `max` jsp pk je la garde en vrai

  fscanf(fp, "%d", &(im->cols));
  fscanf(fp, "%d", &(im->rows));
  fscanf(fp, "%d", &(im->max));

  // Pixel allocation
  Color* mat = malloc(3 * im->rows * im->cols * sizeof(Color));

  // Skip newline
  fread(buff, sizeof(char), 1, fp);

  // Fetch pixel colors from file
  for (int i = 0; i < im->rows; i++) {
    for (int j = 0; j < im->cols; j++) {
      fread(&(mat[i * im->cols + j].r), sizeof(char), 1, fp);
      fread(&(mat[i * im->cols + j].g), sizeof(char), 1, fp);
      fread(&(mat[i * im->cols + j].b), sizeof(char), 1, fp);
    }
  }
  im->mat = mat;
  fclose(fp);
  return im;
}

Video* read_vid(char* video_path, char* frames_path, unsigned int framerate,
                char* start_pos, char* end_pos) {
  char buff[400];
  sprintf(_files_path, "%s", frames_path);

  int width = get_screen_width();
  int height = get_screen_height();
  if (!framerate) framerate = 15;
  if (!width) width = 180;
  if (!height) height = -1;

  // Create all the frames in PPM format, scaled down, FPS down.
  sprintf(buff, "mkdir -p %s", frames_path);
  system(buff);
  sprintf(buff,
          "ffmpeg -i %s -ss %s -to %s -vf "
          "scale=w=%d:h=%d:force_original_aspect_ratio=decrease -r %d "
          "%sframe%%04d.ppm -hide_banner -loglevel error",
          video_path, start_pos, end_pos, width, height, framerate,
          frames_path);
  system(buff);

  // Count the number of frames
  int frames_number = 0;
  {
    DIR* dirp;
    struct dirent* entry;
    if ((dirp = opendir(frames_path)) == NULL) {
      printf("Could not open specified path");
      exit(EXIT_FAILURE);
    }
    while ((entry = readdir(dirp)) != NULL) {
      if (entry->d_type == DT_REG) { /* If the entry is a regular file */
        frames_number++;
      }
    }
    closedir(dirp);
  }

  Video* vid = malloc(sizeof(Video));
  vid->frames = malloc(sizeof(Image*) * frames_number);

  // sprintf(buff, "%sframe%04d.ppm", frames_path, 1);
  // vid->frames[0] = read_im(buff);

  for (int i = 1; i <= frames_number; i++) {
    sprintf(
        buff, "%sframe%04d.ppm", frames_path,
        i);  // Creates path for each frame (path = ./frames_path/frame[i].ppm)
    vid->frames[i - 1] = read_im(buff);
  }

  vid->frames_number = frames_number;

  return vid;
}

void display_im(Image* im) {
  for (int x = 0; x < im->rows; x++) {
    for (int y = 0; y < im->cols; y++) {
      draw_point(y, x, im->mat[x * im->cols + y]);
    }
  }
}

void display_vid(Video* vid, unsigned int fps) {
  clock_t begin;
  clock_t end;
  double T = (double)(1000 * 1) / (double)fps;
  for (int i = 0; i < vid->frames_number; i++) {
    begin = clock();

    begin_drawing();
    display_im(vid->frames[i]);
    end_drawing();

    end = clock();

    unsigned long dtime = (end - begin) * 1000 / CLOCKS_PER_SEC;

    if (T - dtime > 0) {
      usleep((useconds_t)1000 * (T - dtime));
    }
  }
}

void flush_im(Image* im) {
  free(im->mat);
  free(im);
}

void flush_vid(Video* vid) {
  for (int i = 0; i < vid->frames_number; i++) {
    flush_im(vid->frames[i]);
  }
  free(vid->frames);
  free(vid);
}

void delete_files() {
  DIR* folder = opendir(_files_path);
  struct dirent* next_file;
  char filepath[600];

  while ((next_file = readdir(folder)) != NULL) {
    sprintf(filepath, "%s/%s", _files_path, next_file->d_name);
    remove(filepath);
  }
  closedir(folder);
}