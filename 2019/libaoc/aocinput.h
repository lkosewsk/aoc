#ifndef AOCINPUT_H
#define AOCINPUT_H

typedef struct _input_t input_t;

input_t *aocinput_setup(const char filename[]);
int aocinput_teardown(input_t *pfinfo);

int aocinput_getline(input_t *pfinfo, char *output);

#endif
