
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "header.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(
        stderr,
        "Missing argument: Expected a path to the video file (.mp4, ...).\n");
    exit(EXIT_FAILURE);
  }
  char* video_path = argv[1];
  atexit(delete_files);
  Video* vid = read_vid(video_path, FRAMES_PATH, 25, "00:00:00", "00:00:10");
  init_window();

  while (1) {
    begin_drawing();
    display_vid(vid, 25);
    end_drawing();
  }

  flush_vid(vid);
  leave_window();
}