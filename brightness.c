#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INTEL_BRIGHT_P "/sys/class/backlight/intel_backlight/brightness"
#define INTEL_ACTUAL_BRIGHT_P "/sys/class/backlight/intel_backlight/actual_brightness"
#define INTEL_MAX_BRIGHT_P "/sys/class/backlight/intel_backlight/max_brightness"

#define BUFFER_SIZE 64

#define ERROR -1

// return a number from the file in string to integer
int getIntFile(char *fpath) {

    FILE *f = fopen(fpath, "r");

    if (f == NULL) {
        fprintf(stderr, "Could not open %s: %s", fpath, strerror(errno));
        exit(errno);
    }

    char c, buffer[BUFFER_SIZE];

    while ((c=fgetc(f))=='\n' && c != EOF);
    
    if (c<'0' && c > '9') return ERROR;

    int i=0;

    do {
        buffer[i++] = c;

    } while (i < BUFFER_SIZE && (c=fgetc(f))>= '0' && c<='9');

    buffer[i] = '\0';

    fclose(f);

    return atoi(buffer);
    
}

int main(int argc, char *argv[]) {

    if (argc < 2) return 1;

    FILE *bright_file = fopen(INTEL_BRIGHT_P, "w");
    if (bright_file == NULL) {
        fprintf(stderr, "Could not open %s: %s", INTEL_BRIGHT_P, strerror(errno));
        return errno;
    }

    int inc = atoi(argv[1]),
        actual = getIntFile(INTEL_ACTUAL_BRIGHT_P),
        mx = getIntFile(INTEL_MAX_BRIGHT_P),
        new_bright = (int)(actual + mx*inc/100);
        
    if (new_bright < 0) new_bright = 0;
    if (new_bright > mx) new_bright = mx;

    printf("%d\n", new_bright);

    fprintf(bright_file, "%d", new_bright);
    
    fclose(bright_file);

    return 0;
    

}
