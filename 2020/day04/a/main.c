#include "aocinput.h"

#include <search.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define MAX_NUM_KEYS 10
const char *required_keys[] = {
	"byr",
	"iyr",
	"eyr",
	"hgt",
	"hcl",
	"ecl",
	"pid",
	//"cid",
};

static int valid_passports = 0;
static int total_passports = 0;
void validate_passport()
{
	ENTRY e, *ep;
	const char **wanted_key;

	total_passports++;

	for (wanted_key = required_keys;
	     wanted_key < required_keys + sizeof(required_keys) / sizeof(char *);
	     wanted_key++) {
		e.key = (char *)*wanted_key;
		//printf("Searching for key '%s'...", e.key);
		ep = hsearch(e, FIND);
		if (!ep) {
			//printf(" NOT found.\n");
			return;
		}
		//printf(" found.\n");
	}
	valid_passports++;
}

typedef struct _kv {
	char key[4];
	char val[512];
} skv;

int main()
{
	char buf[1024];
	int linelen, num_kvs = 0;
	input_t *passfile = aocinput_setup("input");
	skv current_kvs[8];

	assert(hcreate(MAX_NUM_KEYS));

	while ((linelen = aocinput_getline(passfile, buf))) {
		char *onelinebuf = buf;
		char *kv;

		if (linelen == 1) {
			// just a newline, i.e. start on parsing next passport
			validate_passport();

			hdestroy();
			assert(hcreate(MAX_NUM_KEYS));
			num_kvs = 0;
			continue;
		}

		// Remove newline on the end
		buf[linelen - 1] = 0;

		//printf("got line '%s'\n", onelinebuf);
		while ((kv = strtok(onelinebuf, " "))) {
			ENTRY e, *ep;

			char *colon = strchr(kv, ':');
			strcpy(current_kvs[num_kvs].val, colon + 1);
			*colon = 0;
			strcpy(current_kvs[num_kvs].key, kv);

			e.key = current_kvs[num_kvs].key;
			e.data = current_kvs[num_kvs].val;
			ep = hsearch(e, ENTER);
			assert(ep != NULL);
			//printf("entered '%s:%s'\n", e.key, e.data);
			num_kvs++;

			// to let strtok do its thing
			onelinebuf = NULL;
		}
	}
	validate_passport();

	printf("Number valid passports = %d out of %d\n", valid_passports,
							  total_passports);

	aocinput_teardown(passfile);
	return 0;
}
