#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <assert.h>

long plant_array_size, num_plant_buckets;
unsigned char *plants, *plants_new;
int leftmost_plant;

struct rule {
    unsigned char lhs: 5;
    unsigned char rhs: 1;
};
struct rule rules[128];
int num_rules;
// This is a piss-off and annoys me.  Iterating through num_rules is too slow,
// so observing that the largest binary number formed by 5 unsigned bits is 31,
// we can make a 32-element hashtable that will have 1s wherever a rule matches
// and 0s everywhere else.  Default initialized to 0 by being on the stack.
int hacky_hashtable[32];

inline unsigned char convert_input(const char input);
inline int rule_matches(unsigned char fiveplants, unsigned char rule);
inline void update_bit(unsigned char *plants_arr, unsigned int shift,
                       unsigned char update);
inline void update_loc_by_position(long loc, unsigned int i);

static void fill_output_buf(const char buf, int buflen, char *outputbuf)
{
    int i;
    for (i = 0; i < buflen; ++i) {
        outputbuf[i] = ((1 << i) & buf) ? '#' : '.';
    }
    outputbuf[buflen] = 0;
}

static void output_these_plants(const unsigned char *plantarr)
{
    long i;
    for (i = 0; i < num_plant_buckets; i++) {
        char outputbuf[9];
        int j;
        unsigned char b = plantarr[i];
        fill_output_buf(b, 8, outputbuf);
        printf("%s", outputbuf);
    }
    printf("\n");
}

static void output_plants(int generation, int and_also_new_plants)
{
    int i;
    static long last_num_buckets = 0;
    if (generation == 0 || num_plant_buckets != last_num_buckets) {
        int i;
        printf("    ");
        for (i = 0; i < num_plant_buckets; i++) {
            int o = (i - leftmost_plant) * 8;
            printf("|%-7d", o);
        }
        printf("\n");
        last_num_buckets = num_plant_buckets;
    }

    printf("%2d: ", generation);
    output_these_plants(plants);
    if (and_also_new_plants)
        output_these_plants(plants_new);
}

static void output_rules()
{
    int i;
    for (i = 0; i < num_rules; i++) {
        char outputbuf[6];
        unsigned char b = rules[i].lhs;
        fill_output_buf(b, 5, outputbuf);
        printf("%s => %c\n", outputbuf, rules[i].rhs ? '#' : '.');
    }
}

unsigned char convert_input(const char input)
{
    return input == '#' ? 1 : 0;
}

void get_initial_state()
{
    char c, inputbuf[1024];
    char linebuf[1024];
    int r, num_input_plants, idx;

    FILE *input_file = fopen("input", "r");

    assert(fgets(linebuf, sizeof(linebuf), input_file));
    // load initial state
    r = sscanf(linebuf, "initial state: %s", inputbuf);
    assert(r == 1);
    plants[0] = 0;
    leftmost_plant = 1;
    num_input_plants = strlen(inputbuf);
    // 'plants' is going to be a bitmap.  Convert #s to 1 and .s to 0, and then
    // pack them in.  Rules are 5 bits even though individual chars are 8, so
    // we'll have to deal with that later.
    // note this is not necessarily a perfect multiple of 8, so a few bits will
    // be left untouched.  That's fine.  They are all 0, and that's why we
    // calloc'd plants.
    for (r = 0; r < num_input_plants; r++) {
        idx = r / 8 + 1;
        unsigned char input = convert_input(inputbuf[r]);
        plants[idx] |= input << (r % 8);
        //printf("I am at idx = %d and r =%d\n", idx, r);
    }
    plants[++idx] = 0; // terminating bucket, all 0s.
    num_plant_buckets = idx + 1;

    // throw away second line
    assert(fgets(linebuf, sizeof(linebuf), input_file));

    // parse rules
    num_rules = 0;
    while (fgets(linebuf, sizeof(linebuf), input_file)) {
        r = sscanf(linebuf, "%s => %c", inputbuf, &c);
        assert(r == 2);
        assert(strlen(inputbuf) == 5);
        if (!convert_input(c))
            continue; // no need for rules that don't add anything.
            // Note that rules that add nothing are equivalent to no rule.
        //rules[num_rules].lhs = rules[num_rules].rhs = 0;
        for (r = 0; r < 5; r++) {
            unsigned char input = convert_input(inputbuf[r]);
            rules[num_rules].lhs |= input << r;
        }
        //printf("using index %u\n", rules[num_rules].lhs);
        // fill in the horrible hashtable of hack
        hacky_hashtable[rules[num_rules].lhs] = 1;
        rules[num_rules++].rhs = convert_input(c);
    }

    //output_rules();
    fclose(input_file);
}

int rule_matches(unsigned char fiveplants, unsigned char rule)
{
    // xor would return 0 for all fields  that match identically, which is what
    // we want.  Filter out high order bits for the rule, in case binpacking
    // corrupted them
    return !(fiveplants ^ (rule & 0x1f));
}

// update just one bit, given by address + shift
void update_bit(unsigned char *plants_arr, unsigned int shift,
                unsigned char update)
{
    if (update)
        *plants_arr |= 1 << shift;
    else // update is blank, wipe a bit
        *plants_arr &= ~(1 << shift);
}

/* Given an offset i from a location loc in the char *plants_new, find the
 * correct bit to update.  This means doing some math to find the offset in
 * plants_new and then the correct bit within that offset.  Note that our given
 * 'i' is not actually correct, it's off by 2, since it denotes the start of a
 * 5 character sequence where we are only interested in updating the third one.
 */
void update_loc_by_position(long loc, unsigned int i)
{
    const int relloc = (i + 2) / 8;
    plants_new[loc + relloc] |= 1 << (unsigned int)((i - 6) % 8);
    //update_bit(plants_new + loc + relloc, (i - 6) % 8, 1);
    /*if (i > 5) { // next location over
        update_bit(&plants_new[loc + 1], i - 6, update_val);
    } else {
        update_bit(&plants_new[loc], i + 2, update_val);
    }*/
}

// workhorse
void run_through_rules()
{
    long loc;
    // We can start the plants_new array with all zeros, allowing us to skip any
    // logic killing plants; effectively we only evaluate rules that add plants
    memset(plants_new, 0, num_plant_buckets);
    // Don't need to go through last bucket, we know it's all empty because we
    // ensure that the last bucket is empty before every iteration of
    // run_through_rules.
    for (loc = 0; loc < num_plant_buckets - 1; loc++) {
        unsigned int i;
        // max_shift is 8 because we're using an unsigned short as a buffer, and
        // that's 8 bits larger than a single character (our plants storage
        // unit).  If we wanted to try loop unrolling with i.e. int or long, we
        // would change this to 24 or 56 respectively.
        const int max_shift = 8;
        const unsigned short shifthack = *((unsigned short *)(plants + loc));
        /*if (loc == num_plant_buckets - 1)
            max_shift = 4; // can't compare by overlapping past last bitfield */

        for (i = 0; i < max_shift; i++) {
            int j; //, rule_matched = 0;
            // shift the 16 bits of shithack, and 0x1f = 5 low bits only
            const unsigned char plants_shifted = (unsigned char)(shifthack>>i) & 0x1f;
            if (hacky_hashtable[plants_shifted])
                    update_loc_by_position(loc, i);
#ifdef BE_ORDER_N_NOT_ORDER_1
            // not sure why I would want this now that hacky_hashtable works
            for (j = 0; j < num_rules; j++) {
                if (rule_matches(plants_shifted, rules[j].lhs)) {
                    //printf("Matched rule %d in loc %d, pos %d\n", j, loc, i);
                    update_loc_by_position(loc, i);
                    //rule_matched = 1;
                    break;
                }
            }
#endif
        }
    }
}

// count up last row.  Note that we just run through every bucket, calculate its
// offset from 0, and do simple multiplication.  Probably could have done much
// more clever stuff like multiplying char values by offsets but it's late.
long tally_plants()
{
    int j;
    long i, tally = 0;
    for (i = 0; i < num_plant_buckets; i++) {
        unsigned char c = plants[i];
        for (j = 0; j < 8; j++) {
            if ((1 << j) & c)
                tally += (i - leftmost_plant) * 8 + j;
        }
    }
    return tally;
}

// clean up after every iteration
void adjust_and_reset_plants()
{
    unsigned char *dest;
    int resized = 0;

    if (plant_array_size - num_plant_buckets < 3) {
        // too close, need to reallocate arrays.  If they grow too big we'll
        // stomp over memory.
        plant_array_size *= 2;
        printf("Resizing, plant_array_size now %ld\n", plant_array_size);
        plants = realloc(plants, plant_array_size);
        assert(plants);
        resized = 1;
    }
    dest = plants;

    // if there's no empty bucket at the end of plants...
    if (plants_new[num_plant_buckets - 1] != 0) {
        // easy, add another bucket of empties, this is always OK because we
        // have at least a 2 bucket guarantee for size discrepancy
        plants_new[num_plant_buckets++] = 0;
    }
    // if there's no empty bucket at the beginning of plants...
    if (plants_new[0] != 0) {
        // we will need to enlarge plants
        plants[0] = 0;
        dest = plants + 1;
        ++leftmost_plant;
    }
    memcpy(dest, plants_new, num_plant_buckets);
    if (dest != plants) // if we prefixed, make sure we account for that
        ++num_plant_buckets;
    if (resized) {  // then resize plants_new to match
        plants_new = realloc(plants_new, plant_array_size);
        assert(plants_new);
    }
}

int main(int argc, char *argv[])
{
    long i, generations;
    if (argc != 2) {
        printf("Specify the number of generations you want output for.\n");
        return 1;
    }
    generations = atol(argv[1]);
    plant_array_size = 1024; // initial size
    plants = (unsigned char *)calloc(plant_array_size, sizeof(char));
    assert(plants);
    assert(plants[0] == 0);
    plants_new = (unsigned char *)calloc(plant_array_size, sizeof(char));
    assert(plants_new);

    get_initial_state();
    //printf("Got num rules: %d\n", num_rules);
    //output_plants(0, 0);
    for (i = 0; i < generations; i++) {
        run_through_rules();
        adjust_and_reset_plants();
        //output_plants(i, 0);
    }
    printf("Plants after final (%ldth) generation: %ld\n", i, tally_plants());
    free(plants);
    free(plants_new);
    return 0;
}
