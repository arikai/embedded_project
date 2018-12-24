#include "random.h"
#include "random_feed.h"
#include "time.h"

static uint16_t feed;

uint8_t random_byte()
{
    uint8_t value = feed >> 8;
    random_feed(value ^ ((uint8_t) get_time()));
    return value;
}

uint8_t random(uint8_t a, uint8_t b)
{
    uint8_t diff;
    uint8_t rand_value = feed >> 8;

    // Because check parameters you pass!
    if( b <= a )
	return a;

    diff = b - a;
    rand_value = random_byte();

    // Yes, it is biased towards smaller numbers
    for( ; rand_value >= diff; rand_value-=diff );
    return a + rand_value;
}

bit random_bool(uint8_t probability)
{
    return random(0, 100) < probability;
}

void random_feed(uint8_t v)
{
    feed = v 
	+ (feed << ( (6 ^ v) & 7))
	+ (feed << 12)
	- (feed >> 2);
}

