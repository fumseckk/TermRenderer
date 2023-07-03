
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "header.h"

int main() {
    atexit(delete_files);
    Video* vid = read_vid(VIDEO_PATH, FRAMES_PATH, 25, "00:00:00", "00:00:15");
    init_window();



    while(1) {
        begin_drawing();
        display_vid(vid, 25);
        end_drawing();
    }

    flush_vid(vid);
    leave_window();
}