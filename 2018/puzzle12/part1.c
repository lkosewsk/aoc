#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char plants[1024], plants_new[1024];
int leftmost_plant, num_plant_buckets;

struct rule {
    unsigned char lhs: 5;
    unsigned char rhs: 1;
};
struct rule rules[1024];
int num_rules;

int plant_tally;

inline unsigned char convert_input(const char input);

void fill_output_buf(const char buf, int buflen, char *outputbuf)
{
    int i;
    for (i = 0; i < buflen; ++i) {
        outputbuf[i] = ((1 << i) & buf) ? '#' : '.';
    }
    outputbuf[buflen] = 0;
}

void output_these_plants(const unsigned char *plantarr)
{
    for (int i = 0; i < num_plant_buckets; i++) {
        char outputbuf[9];
        int j;
        unsigned char b = plantarr[i];
        fill_output_buf(b, 8, outputbuf);
        printf("%s", outputbuf);
    }
    printf("\n");
}

void output_plants(int generation, int and_also_new_plants)
{
    int i;
    static int last_num_buckets = 0;
    if (generation == 0 || num_plant_buckets != last_num_buckets) {
        printf("    ");
        for (int i = 0; i < num_plant_buckets; i++) {
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

void output_rules()
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

    fgets(linebuf, sizeof(linebuf), input_file);
    // load initial state
    r = sscanf(linebuf, "initial state: %s", inputbuf);
    plants[0] = 0;
    leftmost_plant = 1;
    num_input_plants = strlen(inputbuf);
    for (r = 0; r < num_input_plants; r++) {
        idx = r / 8 + 1;
        unsigned char input = convert_input(inputbuf[r]);
        plants[idx] |= input << (r % 8);
        //printf("I am at idx = %d and r =%d\n", idx, r);
    }
    plants[++idx] = 0; // terminating bucket
    num_plant_buckets = idx + 1;

    // throw away second line
    fgets(linebuf, sizeof(linebuf), input_file);

    // parse rules
    num_rules = 0;
    while (fgets(linebuf, sizeof(linebuf), input_file)) {
        r = sscanf(linebuf, "%s => %c", inputbuf, &c);
        for (r = 0; r < 5; r++) {
            unsigned char input = convert_input(inputbuf[r]);
            rules[num_rules].lhs |= input << r;
        }
        rules[num_rules++].rhs = convert_input(c);
    }

    // tally plants
    plant_tally = 0;

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

void update_bit(unsigned char *plants_arr, int shift, unsigned char update)
{
    if (update)
        *plants_arr |= 1 << shift;
    else // update is blank, wipe a bit
        *plants_arr &= ~(1 << shift);
}

void update_loc_by_position(int loc, int i, unsigned char update_val)
{
    if (i > 5) { // next location over
        update_bit(&plants_new[loc + 1], i - 6, update_val);
    } else {
        update_bit(&plants_new[loc], i + 2, update_val);
    }
}

void run_through_rules()
{
    int loc;
    memcpy(plants_new, plants, sizeof(plants));
    for (loc = 0; loc < num_plant_buckets; loc++) {
        int i, max_shift = 8;
        unsigned short shifthack = *((unsigned short *)(&plants[loc]));
        if (loc == num_plant_buckets - 1)
            max_shift = 4; // can't compare by overlapping past last bitfield

        for (i = 0; i < max_shift; i++) {
            int j, rule_matched = 0;
            // 0x1f = 5 low bits, and shift the 16 bits of shithack
            unsigned char plants_shifted = (unsigned char)(shifthack>>i) & 0x1f;
            for (j = 0; j < num_rules; j++) {
                if (rule_matches(plants_shifted, rules[j].lhs)) {
                    //printf("Matched rule %d in loc %d, pos %d\n", j, loc, i);
                    update_loc_by_position(loc, i, rules[j].rhs);
                    rule_matched = 1;
                    break;
                }
            }
            if (!rule_matched) // no rule matches, so kill plant
                update_loc_by_position(loc, i, 0);
        }
    }
}

int tally_plants()
{
    int i, j, tally = 0;
    for (i = 0; i < num_plant_buckets; i++) {
        unsigned char c = plants[i];
        for (j = 0; j < 8; j++) {
            if ((1 << j) & c)
                tally += (i - leftmost_plant) * 8 + j;
        }
    }
    return tally;
}

void adjust_and_reset_plants()
{
    unsigned char *dest = plants;

    if (plants_new[num_plant_buckets - 1] != 0) {
        // easy, add another bucket of empties
        plants_new[num_plant_buckets++] = 0;
    }
    if (plants_new[0] != 0) {
        // we will need to enlarge plants
        plants[0] = 0;
        dest = plants + 1;
        ++leftmost_plant;
    }
    memcpy(dest, plants_new, num_plant_buckets);
    if (dest != plants)
        ++num_plant_buckets;
}

int main(int argc, char *argv[])
{
    int i, generations;
    if (argc != 2) {
        printf("Specify the number of generations you want output for.\n");
        return 1;
    }
    generations = atoi(argv[1]);
    get_initial_state();
    //printf("Got num rules: %d\n", num_rules);
    //output_plants(0, 0);
    for (i = 1; i <= generations; i++) {
        run_through_rules();
        adjust_and_reset_plants();
        //output_plants(i, 0);
    }
    printf("Total plants after final (%dth) generation: %d\n", i - 1,
                                                               tally_plants());
    return 0;
}
