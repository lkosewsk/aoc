#include "aocinput.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#include <stdlib.h>

#define LINEBUF_SIZE 1025

struct _input_t {
	char linebuf[LINEBUF_SIZE];
	int fd;
	int lineidx;
	ssize_t linebuf_size;
};

input_t *aocinput_setup(const char filename[])
{
	input_t *r = (input_t *)malloc(sizeof(input_t));
	r->lineidx = 0;
	r->linebuf_size = 0;
	r->fd = open(filename, O_RDONLY);
	if (r->fd < 0) {
		free(r);
		return NULL;
	}
	return r;
}

int aocinput_teardown(input_t *pfinfo)
{
	close(pfinfo->fd);
	free(pfinfo);
}

static ssize_t fill_input_buf(input_t *pfinfo)
{
	pfinfo->linebuf_size = read(pfinfo->fd, pfinfo->linebuf,
				    LINEBUF_SIZE - 1);
	pfinfo->linebuf[pfinfo->linebuf_size] = 0;
	pfinfo->lineidx = 0;
	return pfinfo->linebuf_size;
}

int aocinput_get_to_sep(input_t *pfinfo, char *output, char sep)
{
	int output_loc = 0;
	if (pfinfo->linebuf_size == 0) {
		fill_input_buf(pfinfo);
	}
	if (pfinfo->linebuf_size == 0) {
		output[0] = 0;
		return 0;
	}

	while (1) {
		const char *start = pfinfo->linebuf + pfinfo->lineidx;
		const char *newline = index(start, sep);
		if (newline == NULL) {  // no newline located, read again
			int l = strlen(start);
			strcpy(output + output_loc, start);
			output_loc += l;
			if (fill_input_buf(pfinfo) == 0)
				return output_loc;
			// else try to find a newline again
		} else {
			// found it, return everything including newline
			strncpy(output + output_loc, start,
				newline - start + 1);
			pfinfo->lineidx = newline - pfinfo->linebuf + 1;
			output[output_loc + newline - start + 1] = 0;
			return output_loc + newline - start + 1;
		}
	}
}
