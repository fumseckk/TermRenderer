#include <threads.h>
#include <sys/time.h>
#include <unistd.h>

#include "../core.h"
#include "../../headers/debug.h"


// ---------- Variables ----------

static double sec_per_frame = 1. / 20.;
static double current_framerate = 20.;


// ---------- Internal functions ----------

unsigned long _get_utime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}


void _main_server_loop() {
    // time stamp of the last second
    unsigned long last_monitored = _get_utime();
    int frame_count = 0;
    // time stamp of the previous frame
    unsigned long prev = _get_utime();


    while (1) {
        // get the time at the begining of the frame
        unsigned long curr = _get_utime();
        double delta = (curr - prev) / 1000000.;

        // run frame
        _loop(delta);
        frame_count++;

        // update current framerate
        if (delta - last_monitored >= 1000000) {
            current_framerate = 1000000. * frame_count / (delta - last_monitored);
            last_monitored = curr;
            frame_count = 1;
        }

        //update last frame time
        prev = curr;

        // sleep to have constant framerate
        if (sec_per_frame - delta > 0.) usleep((useconds_t)((sec_per_frame - delta) * 1000000.));
    }
}


// --------- Library functions ---------


void set_framerate(double framerate) {
    sec_per_frame = 1. / framerate;
}


double get_current_framerate() {
    return current_framerate;
}
