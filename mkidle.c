#include <stdio.h>
#include <stdlib.h>
#include <time.h>

idle_wait(int i, int *adj_us)
{
    int j = i;
    struct timespec ts;
    struct timespec res;
    int start_us,now_us;

    if (i == 0) {
        *adj_us = 1200; 
        return;
    }

    clock_gettime(CLOCK_REALTIME, &ts);
    start_us = ts.tv_sec*1000 + ts.tv_nsec / 1000;

    while(j--)
        usleep(1000);

    clock_gettime(CLOCK_REALTIME, &ts);
    now_us = ts.tv_sec*1000  + ts.tv_nsec / 1000;
    *adj_us = abs(now_us - start_us)/i;
    if (*adj_us > 1400)
        *adj_us = 1200; 
}

busy_wait(int i, int adj_us)
{
    int j;
    struct timespec ts;
    struct timespec res;
    int start_us,now_us;

    clock_gettime(CLOCK_REALTIME, &ts);
    start_us = ts.tv_sec*1000 + ts.tv_nsec / 1000;

    while(i--) {
        while(1) {
            clock_gettime(CLOCK_REALTIME, &ts);
            now_us = ts.tv_sec*1000  + ts.tv_nsec / 1000;
	    //printf("%d\n", now_us);
	    if (abs(now_us - start_us) > adj_us) {
	        start_us = now_us;
	        //printf("%d\n", now_us);
                break;
	    }
        }
    }
}

#define UP 1
#define DOWN 2

main()
{
    int adj;
    int dir = UP;
    int idle_ratio;
    int busy_ratio = 0;
    FILE *fp;

    fp = fopen("/tmp/mkidle", "w");
    if (fp == NULL)
        exit(1);

    while(1) {
        idle_ratio = 1000 - busy_ratio;

 	fseek(fp, 0L, SEEK_SET);	
	fprintf(fp, "%d %d\n", idle_ratio/10, busy_ratio/10);

        idle_wait(idle_ratio, &adj);
        busy_wait(busy_ratio, adj);

        if (dir == UP) {
            busy_ratio += 20;
        } else {
            busy_ratio -= 20;
        }

	if (busy_ratio == 1000) {
            dir = DOWN;
        } else if (busy_ratio == 0) {
            dir = UP;
        }
    }
}
