#ifndef HEADER_H
#define HEADER_H

#include "../../headers/renderer.h"

// Parameters
#define VIDEO_PATH  "rickroll.mp4" // "./celeste_chap1_speedrun.mkv"
#define FRAMES_PATH "./images/" // Put / at the end for folder
#define SLEEP_TIME 60 // In milliseconds. Waiting 100ms sets the speed at 10 FPS.


typedef struct {
    Color* mat;
    int rows;
    int cols;
    int max;
    char type[2];
} Image;

typedef struct {
    Image** frames;
    int frames_number;
} Video;


extern char files_path[300];

// Prototypes
Image* read_im(char* file);

// - video_path: current location of the video to render
// - frames_path: folder where the frames will be created
// - framerate: framerate of the generated video. Don't go too high, performance-heavy. 0 will set the default (15).
// - width of the generated video.  0 will set the default (100).
// - height of the generated video. 0 will keep aspect ratio.
// - start_pos: video will be generated starting from there. Format hh:mm:ss
// - end_pos: video will be generated up to there. Format hh:mm:ss
Video* read_vid(char* video_path, char* frames_path, unsigned int framerate, char* start_pos, char* end_pos);

void display_im(Image* im);
void display_vid(Video* vid, unsigned int fps);
void flush_im(Image* im);
void flush_vid(Video* vid);
void delete_files(void);

#endif