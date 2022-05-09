#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INTEL_BRIGHT_P "/sys/class/backlight/intel_backlight/brightness"
#define INTEL_ACTUAL_BRIGHT_P "/sys/class/backlight/intel_backlight/actual_brightness"
#define INTEL_MAX_BRIGHT_P "/sys/class/backlight/intel_backlight/max_brightness"

#define BUFFER_SIZE 64

#define ERROR -1

// return a number from the file in string to integer
int getIntFile(FILE *f) {
    if (f == NULL)
        return ERROR;

    char c, buffer[BUFFER_SIZE];

    while ((c=fgetc(f))=='\n' && c != EOF);
    
    if (c<'0' && c > '9') return ERROR;

    int i=0;

    do {
        buffer[i++] = c;

    } while ((c=fgetc(f))>= '0' && c<='9');

    buffer[i] = '\0';

    return atoi(buffer);
    
}

int main(int argc, char *argv[]) {

    if (argc < 2) return 1;

    FILE *bright_file = fopen(INTEL_BRIGHT_P, "w"),
         *actual_file = fopen(INTEL_ACTUAL_BRIGHT_P, "r"),
         *max_b_file = fopen(INTEL_MAX_BRIGHT_P, "r");

    if (actual_file == NULL || bright_file == NULL)
        return 1;

    int inc = atoi(argv[1]),
        actual = getIntFile(actual_file),
        mx = getIntFile(max_b_file),
        new_bright = actual+inc<=mx ? actual+inc : mx;

    printf("%d\n", new_bright);

    if (actual == ERROR || mx == ERROR)
        return 1;

    fprintf(bright_file, "%d", new_bright);
    

    fclose(bright_file);
    fclose(actual_file);
    fclose(max_b_file);

    return 0;
    

}
