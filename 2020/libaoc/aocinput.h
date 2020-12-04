#ifndef AOCINPUT_H
#define AOCINPUT_H

typedef struct _input_t input_t;

input_t *aocinput_setup(const char filename[]);
int aocinput_teardown(input_t *pfinfo);

int aocinput_get_to_sep(input_t *pfinfo, char *output, char sep);

inline int aocinput_getline(input_t *pfinfo, char *output)
{
	return aocinput_get_to_sep(pfinfo, output, '\n');
}

int aocinput_count_lines(const char filename[]);

#endif
