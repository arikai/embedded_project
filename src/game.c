#include "game.h"

#include "common.h"
#include "ui.h"
#include "time.h"
#include "bindings.h"
#include "led.h"

#include "sound.h"
#include "tracks.h"
#include "random.h"


/*
 * Game data
 */

struct game {
    uint8_t play_count;
    uint8_t win_count;
    uint16_t rooms_visited;
} game = { 0, 0, 0 };

#define BASE_HEALTH 8
#define BASE_WEAPON 2
#define BASE_ARMOR  1
struct player {
    uint8_t health;

    uint8_t level;
    uint8_t exp;

    uint8_t weapon; // Weapon attack
    uint8_t armor;  // Armor defense

    // Recalculated in battle
    uint8_t max_health;
    uint8_t attack;  // Total attack and defense
    uint8_t defense;
} player;

void reset_player(void)
{
    player.health = BASE_HEALTH;
    player.level  = 1;
    player.exp    = 0;
    player.weapon = BASE_WEAPON;
    player.armor  = BASE_ARMOR;
}

void recalculate_player_stats(void)
{
    player.max_health = 4 + player.level;
    if( player.max_health < player.health ) player.health = player.max_health;
    player.attack = ( player.level >> 1 ) + player.weapon + ( player.health >> 2 );
    player.defense = ( player.level >> 1 ) + player.armor + ( player.health >> 2 );
}

bit damage_player( uint8_t damage )
{
    if( damage >= player.health )
    {
	clear();
	sleep(2000);

	leds(0);
	sleep(500);
	info("    YOU DIED", DELAY_SNAIL);

	sleep(10000);
	return 1;
    }
    else
    {
	player.health -= damage;
	leds( 0xff >> ( 8 - player.health ) );
	return 0;
    }
}

enum trait {
    NONE = 0,
    THICK_SKINNED = 0x01,
    INTELLIGENT = 0x02,
}

struct enemy {
    uint8_t health;

    uint8_t base_attack;
    uint8_t base_defense;

    uint8_t traits;
} enemy;


void game_title(void)
{
    // snd_set_bg_track(&intro_track);
    clear();
    info("D U N K M A Z E", 250);
    sleep(1000);
}

enum response {
    OK,
    CANCEL,
};

#define EXPLAIN(KEY) explain_bindings(BIND_ ## KEY ## _DESC, BIND_ ## KEY)

void explain_bindings(char* description, uint8_t expected_key)
{
    clear_line();
    message(description);
    line_home();
    while( read_key(0) != expected_key );
}

#define MAX_NAME 9
static char name[MAX_NAME+1] = "Nobody";

void game_intro(void)
{
    clear();
    info("   Welcome to\n   Dunkmaze!", DELAY_NORMAL);
    sleep(1000);

    clear();
    info("Prepare to die.", DELAY_UNEASY);
    sleep(1000);

    read_input("Name yourself.", DELAY_UNEASY, name, MAX_NAME, 1);
    reset_player();
    recalculate_player_stats();
    damage_player(0);

    clear();
    info("Welcome,\n", DELAY_UNEASY);
    info(name, DELAY_SCARY);
    sleep(1000);

    clear();
    if( !read_ok("Skip tutorial?") )
    {
	clear();
	message("Tutorial\n");
	sleep(1000);
	clear();

	message("Press\n");
	EXPLAIN(OK);
	EXPLAIN(CANCEL);
	EXPLAIN(FORWARD);
	EXPLAIN(BACK);
	EXPLAIN(LEFT);
	EXPLAIN(RIGHT);
	// EXPLAIN(DISCOVER);
	
	clear();
	info("Now,\n", DELAY_NORMAL);
	info("Good luck.", DELAY_UNEASY);
	sleep(2000);
    }

    clear();
    story("You enter\nthe Dunkmaze", DELAY_UNEASY, DELAY_SCARY);

    sleep(1000);
    clear();
}


/*
 * OVERWORLD STAGE
 * Walk through corridors and explore the maze
 *
 * Maze consists of "rooms" which have 3 corridors to next rooms
 * and a corridor to the previous room.
 *
 * Previous rooms are not remembered and Corridors have random length
 * (i.e., by taking 4x(left/right) you will not return to the room
 */

/* 
 * Next room observation probabilities
 * | Prob | Observation | artifact | item | trap | enemy | nothing |
 * |------+-------------+----------+------+------+-------+---------|
 * |  10% | shine       |   2%     |  33% |  35% |   30% |      0% |
 * |  20% | sound       |   2%     |   4% |   4% |   60% |     30% |
 * |  20% | talk        |   5%     |   2% |   3% |   80% |     10% |
 * |  50% | darkness    |   1%     |  25% |  25% |   25% |     24% |
 */

enum observation {
    SHINE,    // 10%
    SOUND,    // 20%
    TALK,     // 20%
    NOTHING,  // 50% 

    DARKNESS  // Special
};

enum event {
    ARTIFACT,
    ITEM,
    TRAP,
    ENEMY,
    NO_EVENT,

    DEATH,
};

int8_t random_observation()
{
    int8_t v = random(0, 100);
    if( v < 10 ) return SHINE;
    else if( v <  ( 10 + 20 ) ) return SOUND;
    else if( v <  ( 30 + 20 ) ) return TALK;
    else return NOTHING;
}

int8_t event_from_observation(int8_t observation)
{
    int8_t v = random(0, 100);
    switch( observation )
    {
	case SHINE:
	    if( v < 2 ) return ARTIFACT;
	    else if ( v < (  2 + 33 )) return ITEM;
	    else if ( v < ( 35 + 35 )) return TRAP;
	    else return ENEMY;

	case SOUND:
	    if( v < 2 ) return ARTIFACT;
	    else if ( v < (  2 +  4 )) return ITEM;
	    else if ( v < (  6 +  4 )) return TRAP;
	    else if ( v < ( 10 + 60 )) return ENEMY;
	    else return NO_EVENT;

	case TALK:
	    if( v < 5 ) return ARTIFACT;
	    else if ( v < (  5 +  2 )) return ITEM;
	    else if ( v < (  7 +  3 )) return TRAP;
	    else if ( v < ( 10 + 80 )) return ENEMY;
	    else return NO_EVENT;

	case NOTHING:
	    if( v < 1 ) return ARTIFACT;
	    else if ( v < (  1 + 25 )) return ITEM;
	    else if ( v < ( 26 + 25 )) return TRAP;
	    else if ( v < ( 51 + 25 )) return ENEMY;
	    else return NO_EVENT;

	case DARKNESS:
	    if( v < 10 ) return ARTIFACT;
	    else if( v < ( 10 + 80 )) return DEATH;
	    else return NO_EVENT;

	default:
	    return ENEMY;
    }
}

XDATA( char ) enemy_description[][] = {
    "It's teeth are\ncovered in blood",
    "Once it was\na person's leg",
    "Smell makes you\nwant to vomit",
    "Your blood is frozen"
};

bit game_battle(void)
{
    story("The monster\nis dead", DELAY_UNEASY, DELAY_NONE);
    return 0;
}

/*
 * Returns 1 if game is over, 0 otherwise
 */
bit enter_corridor(int8_t observation)
{
    uint8_t item_stat;
    bit item_better;
    bit item_weapon;

    ++game.rooms_visited;
    clear();

    story("Walking through\nthe corridor...", DELAY_SLOW, DELAY_NONE);
    sleep(500);
    clear();
    sleep(1000);
    switch( event_from_observation(observation) )
    {
	case ARTIFACT:
	    sleep(2000);
	    story("\"Finally...\"", DELAY_NORMAL, DELAY_NONE);
	    sleep(3000);
	    story("\"After such\na long time\"", DELAY_SLOW, DELAY_SCARY);
	    sleep(3000);
	    story("\"I have\nfound it...\"", DELAY_SCARY, DELAY_SCARY);
	    sleep(7000);
	    
	    // story(" You carefully\n    take it", DELAY_SLOW, DELAY_SCARY);
	    story("  TODO FINISH", DELAY_NORMAL, DELAY_NORMAL );

	    ++game.win_count;
	    return 1;

	case ITEM:
	    story("You see\n an item", DELAY_NORMAL, DELAY_NONE);
	    sleep(500);
	    item_stat = random(2, player.level+3);

	    item_weapon = random_bool(50);
	    clear();
	    if( item_weapon ) // Weapon
	    {
		story("It's a weapon\nAttack: ", DELAY_NORMAL, DELAY_NONE);
		item_better = player.weapon < item_stat;
	    }
	    else { // Armor
		story("It's an armor\nDefense: ", DELAY_NORMAL, DELAY_NONE);
		item_better = player.armor < item_stat;
	    }
	    print_char( item_better ? CHAR_TRIANG_UP : CHAR_TRIANG_DOWN );
	    story_number(item_stat, DELAY_NORMAL, DELAY_NONE);

	    sleep(1000);
	    if( read_ok("Take it?") )
	    {
		if( item_weapon )
		    player.weapon = item_stat;
		else
		    player.armor = item_stat;

		if( random_bool(5) )
		{
		    clear();
		    story("You feel...\nnausea...\n", DELAY_UNEASY, DELAY_SCARY);
		    if( damage_player(1) ) return 1;
		    clear();
		    sleep(1000);

		    story("Your skin\n", DELAY_SCARY, DELAY_NONE);
		    if( damage_player(1) ) return 1;

		    sleep(500);
		    story("is melting...", DELAY_SCARY, DELAY_NONE);
		    if( damage_player(3) ) return 1;

		    sleep(5000);
		    clear();
		    story("...c-u-r-s-e-d", DELAY_SNAIL, DELAY_NONE);
		    sleep(500);
		    if( damage_player(255) ) return 1;
		}
	    }

	    return 0;

	case TRAP:
	    story("You've steeped\ninto a trap", DELAY_NORMAL, DELAY_NONE);
	    return damage_player(random(1,3));
	
	case ENEMY:
	    story("It slowly\napproaches you.", DELAY_SLOW, DELAY_SCARY);
	    sleep(1000);
	    return game_battle();

	case DEATH:
	    sleep(2000);

	    story("Darkness...", DELAY_UNEASY, DELAY_NONE);
	    sleep(2000);
	    if( damage_player(1) ) return 1;
	    sleep(2000);

	    newline();
	    story("You can't breath", DELAY_UNEASY, DELAY_NONE);
	    sleep(2000);
	    if( damage_player(1) ) return 1;
	    sleep(2000);

	    newline();
	    story("It fills your lungs", DELAY_UNEASY, DELAY_NONE);
	    sleep(2000);
	    if( damage_player(1) ) return 1;
	    sleep(2000);

	    newline();
	    story("It's in your veins", DELAY_UNEASY, DELAY_NONE);
	    sleep(2000);
	    if( damage_player(1) ) return 1;
	    sleep(2000);

	    clear();

	    story(" YOU ", DELAY_SCARY, DELAY_NONE);
	    sleep(2000);

	    story("DISAPPEAR\n", DELAY_UNEASY, DELAY_NONE);
	    sleep(4000);

	    story("  IN DARKNESS", DELAY_SNAIL, DELAY_NONE);
	    sleep(4000);
	    if( damage_player(255) ) return 1;

	case NO_EVENT:
	default:
	    return 0;
    }
}

void describe_corridor(int8_t observation)
{
    clear();
    switch( observation )
    {
	case SHINE:
	    info("You see\nsubtle shining", DELAY_NORMAL);
	    break;

	case SOUND:
	    info("You hear\nsome noises", DELAY_NORMAL);
	    break;

	case TALK:
	    info("You hear\nsomeone talking", DELAY_NORMAL);
	    break;

	case NOTHING:
	    info("You see nothing", DELAY_NORMAL);
	    break;

	case DARKNESS:
	    info("    DARKNESS\n", DELAY_UNEASY);
	    info("   DRAWS NEAR", DELAY_SCARY);
	    sleep(1500);
	    clear();
	    info("Will you risk\n", DELAY_UNEASY);
	    info("   your Life?", DELAY_SCARY);
	    break;
    }
}

void game_overworld(void)
{
    int8_t left;
    int8_t forward;
    int8_t right;
    int8_t select = 0;

    uint8_t prevkey = 0;
    uint8_t key;

    while(1)
    {
	prevkey = 0;
	left = random_observation();
	forward = random_observation();
	right = random_observation();

	clear();
	story("You are\nin a room", DELAY_NORMAL, DELAY_NONE);

read_again:
	while(1)
	{
	    key = read_key(0);
	    if( prevkey == key )
		break;

	    switch( key ){
		case BIND_LEFT:    select = left;     prevkey = key; break;
		case BIND_FORWARD: select = forward;  prevkey = key; break;
		case BIND_RIGHT:   select = right;    prevkey = key; break;
		case BIND_BACK:    select = DARKNESS; prevkey = key; break;
		default: prevkey = 0; goto read_again;
	    }

	    describe_corridor(select);
	}
	if( enter_corridor(select) ) return;
    }
}

void game_begin(void){
    while(1)
    {
	// Init player
	reset_player();
	recalculate_player_stats();
	damage_player(0);

	game_overworld();
	++game.play_count;

	while( !read_ok("Play again?" ))
	{ 
	    clear();
	    sleep(15000);
	}
    }
}

void game_start(void)
{
    // game_title();
    // game_intro();
    game_begin();
}
