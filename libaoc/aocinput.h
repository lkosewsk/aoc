#ifndef AOCINPUT_H
#define AOCINPUT_H

typedef struct _input_t input_t;

input_t *setup_input(const char filename[]);
int teardown_input(input_t *pfinfo);

int get_a_line(input_t *pfinfo, char *output);

#endif
