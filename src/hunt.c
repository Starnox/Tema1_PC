#include "hunt.h"
#include <stdio.h>

/* Task 1 - Axii */

uint16_t find_spell(uint64_t memory)
{
    /*
     * The spell is 16 bits and they are in the memory exactly after 5
     * consecutive bits of 1:
     *
     *            high                                             low
     *                [][][x]...[x][x][x][1][1][1][1][1][]...[][][]
     *                    |----16bits---|
     *                          Spell
     *
     * There is only 1 group of 5 consecutive bits of 1 and the other groups
     * have less than consecutive bits of 1
     * Steps:
     * - find 5 consecutive bits of 1
     * - the NEXT 16 bits == spell
     */

    uint16_t res = -1;

    /* TODO */

    int number_of_curr_bits = 0;
    int found_spell = 0;

    for(uint16_t i =0; i< 64; ++i)
    {
        // if the bit is set
        if(memory & (1LU << i))
        {
            number_of_curr_bits++;
        }   
        else
        {
            number_of_curr_bits = 0;
        }

        if(number_of_curr_bits == 5)
        {
            found_spell = i+1;
            break;
        }
    }
    res = 0;
    for(uint16_t i = 0; i < 16; ++i)
    {
        if(memory & (1LU << (found_spell+i)))
        {
            res |= (1LU << i);
        }
    }

    return res;
}


uint16_t find_key(uint64_t memory)
{
    /*
     * The key is 16 bits and they are in the memory exactly before 3
     * consecutive bits of 1:
     *
     *                high                                           low
     *                     [][][][1][1][1][x][x]...[x][x][]...[][][]
     *                                    |----16bits---|
     *                                           Key
     * Steps:
     * - find 3 consecutive bits of 1
     * - the PREVIOUS 16 bits == key
     */

    uint16_t res = -1;

    /* TODO */

    int number_of_curr_bits = 0;
    int found_key = 0;

    for(uint16_t i =0; i< 64; ++i)
    {
        // if the bit is set
        if(memory & (1LU << i))
        {
            number_of_curr_bits++;
        }   
        else
        {
            number_of_curr_bits = 0;
        }

        if(number_of_curr_bits == 3)
        {
            // set the found_key variable to the beggining of the key
            found_key = i-3-16+1;
            break;
        }
    }
    res = 0;
    for(uint16_t i =0; i< 16; ++i)
    {
        if(memory & (1LU << (i+found_key)))
        {
            res |= (1LU << i);
        }
    }
    return res;
}


uint16_t decrypt_spell(uint16_t spell, uint16_t key)
{
    /*
     * Find the spell knowing that
     * spell_encrypted = spell_plaintext ^ key
     */

    uint16_t res = -1;
    res = 0;

    for(uint16_t i = 0; i< 16; ++i)
    {
        // if bit i is set in the encrypted spell
        if(spell & (1LU <<i))
        {
            // if bit i is set in the key
            if(key & (1LU << i))
            {
                // the bit is 0
                continue;
            }
            else
            {
                // the bit is 1
                res |= (1LU << i);
            }
        }
        else
        {
            if(key & (1LU << i))
            {
                res |= (1LU << i);
            }
            else
            {
                continue;
            }
            
        }
    }

    /* TODO */

    return res;
}


/* Task 2 - Sword Play */

uint32_t choose_sword(uint16_t enemy)
{
    /*
     * Help Geralt construct a sword that will give him an advantage in battle.
     * The steps we will follow:
     * - choose the corect sword
     * - brand the sword with the correct runes such that the equation
     * for that specific enemy type is true
     *
     * How does the sword look (Sword template):
     *  [][][][] [0][0][0]...[0][0][0] [][][][][][][][][][][][][][][][]
     *  -------- --------------------- --------------------------------
     *     Type    Does not matter      The "Value" such that the equation
     *                               from 2 holds depending on the enemy type
     *    4 bits      12 bits                     16 bits
     *
     * 1. Type of the sword:
     * - 1001 - Silver Sword - for monsters
     * - 0110 - Steel Sword - for humans
     *
     *  If the number of active bits from variable "enemy" is even == Monster
     *  else == Human
     *
     * 2.
     * Monster Equation:
     *    Value ^ (Enemy & (1 - Enemy)) = 0
     *
     * Human Equation:
     *    Value + Enemy = 0
     */

    uint32_t res = -1;
    res = 0;

    /* TODO */
    uint16_t number_of_set_bits = 0;
    for(uint16_t i = 0; i< 16; ++i)
    {
        // if the bit is set increment the variable
        if(enemy & (1LU << i))
            number_of_set_bits++;
    }
    // if is odd
    if(number_of_set_bits & 1)
    {
        enemy = ~enemy + 1;
        // then the enemy is a human
        // set the first 16 bits (flip the bits)
        for(uint16_t i = 0; i< 16; ++i)
        {
            // if is set don't do anything
            if(enemy & (1LU <<i))
            {
                res |= (1 << i);
            }
            
            
        }
        // set the last 4 bits
        res |= (1LU << 29);
        res |= (1LU << 30);
    }
    else
    {
        // the enemy is a monster

        // set the first 16 bits
        uint16_t aux = enemy & (1 - enemy);
        for(uint16_t i = 0; i< 16; ++i)
        {
            if(aux & (1LU << i))
            {
                res |= (1 << i);
            }
            
        }

        // set the last 4 bits
        res |= (1LU << 28);
        res |= (1LU << 31);
    }
    

    return res;
}


/* Task 3 - The Witcher Trials */

uint32_t trial_of_the_grasses(uint16_t cocktail)
{
    /*
     * To become a witcher one must survive a cocktail of different
     * herbs. The body should generate the correct antibodies to
     * neutralize the given cocktail.
     *
     *
     * The antibodies need to respect the following properties:
     *   (antibodies_high & cocktail) ^ (antibodies_low | cocktail) = 0
     *   antibodies_low & antibodies_high = 0
     *   antibodies_low | antibodies_high = cocktail
     *
     * Where:
     *  [][][]...[][][] | [][][]...[][][]
     *  ---------------   ---------------
     *  antibodies_high    antibodies_low
     *      16 bits           16 bits
     *      -------------------------
     *              antibodies
     */

    uint32_t res = -1;
    res = 0;

    /* TODO */
    // binary backtrace

    // fixing antibodies_low
    uint16_t antibodies_low = 0, antibodies_high = 0;
    for(antibodies_low = 0; antibodies_low < (1LU << 16); ++antibodies_low)
    {
        int ok = 1;
        antibodies_high = 0;
        // going through each bit
        for(uint16_t i =0; i < 16; ++i)
        {
            // if ith bit is set
            if(antibodies_low & (1LU << i))
            {
                // if cocktail has the ith bit 0 
                if(!(cocktail & (1LU << i)))
                {
                    // the combination is not right
                    ok = 0;
                    break;
                }
                
            }
            // ith bit of antibodies_low is 0 then antibodies_high can either be 0 or 1
            else
            {
                // if ith bit of coktail is set
                if(cocktail & (1LU << i))
                {
                    antibodies_high |= (1LU << i);
                }
            }
        }
        // didn't find a good value
        if(ok == 0)
            continue;
        // checking the first condition
        uint16_t aux1 = antibodies_high & cocktail;
        uint16_t aux2 = antibodies_low | cocktail;
        uint16_t res = aux1 ^ aux2;

        if(res == 0)
        {
            // we found good values
            break;
        }
    }

    // building the final result
    for(uint32_t i = 0; i< 16; ++i)
    {
        if(antibodies_low & (1LU << i))
            res |= (1LU << i);
        if(antibodies_high & (1LU << i))
            res |= (1LU << (i+16));
    }

    return res;
}


uint8_t trial_of_forrest_eyes(uint64_t map)
{
    /*
     * For the next trail, the candidate is tied up and blindfolded.
     * They are also taken to a forest and they should return until
     * the next morning.
     *
     * The candidate knows that there are 4 possibilities for the forest:
     * Brokilon, Hindar, Wolven Glade and Caed Dhu.
     *
     * The candidate also knows those forests by heart, but first you
     * need to identify the forest.
     *
     * The characteristics for 3 of the forests are as follows:
     * - Brokilon - has ONLY groups of 4 trees (or a forest without any tree)
     *   Ex: a group of 4 trees: "...00111100..."
     * - Hindar - has ONLY 2 trees in the MIDDLE
     * - Wolven Glade - FULL of trees
     *
     * The "map" variable encodes the following information:
     * - 1 tree
     * - 0 patch of grass
     *
     * You should put in the "res" variable the index for the identified
     * forrest.
     * - Brokilon - index 0
     * - Hindar - index 1
     * - Wolven Glade - index 2
     * - Caed Dhu - index 3
     */

    uint8_t res = -1;

    /* TODO */
    uint16_t number_of_trees = 0;
    // presume the forest is of type 0 with groups of 4 trees
    uint16_t ok = 1;
    for(uint64_t i = 0; i < 64; ++i)
    {
        // if we have a tree on bit i
        if(map & (1LU << i))
            number_of_trees++;
        else
        {
            // checking if we don't have groups of 4 trees
            if(number_of_trees % 4 != 0)
                ok = 0;
        }
        
    }

    // deciding on the forest
    if(number_of_trees == 64)
        res = 2;
    else if(number_of_trees == 0 || ok)
        res = 0;
    else if(number_of_trees == 2)
    {
        // if the middle 2 bits are 1
        if((map & (1LU << 32)) && (map & (1LU << 31)))
            res = 1;
    }
    else
    {
        res = 3;
    }

    return res;
}


uint8_t trial_of_the_dreams(uint32_t map)
{
    /*
     * For this final trial, the candidates go into a trance and when
     * they woke up they would be a fully fledged witcher.
     *
     * For this task, you need to help the witcher find the number
     * of steps that would take the candidate from the dream realm to
     * the real world.
     *
     * The "map" variable would have only 2 bits of 1:
     * - candidate location
     * - portal location
     *
     * You should put in the "res" variable the distance from the
     * candidate position to the portal.
     *
     *  Ex:
     *  [0][0][0][0][0][0]....[1][0][0][0][0][1][0]....[0][0][0]
     *
     *  res = 5
     */

    uint8_t res = -1;
    int last = -1;
    for(uint32_t i = 0; i< 32; ++i)
    {
        // if the bit is set
        if(map & (1LU << i))
        {
            // if is the first bit we found
            if(last == -1)
                last = i;
            else
            {
                // set the distance and break
                res = i - last;
                break;
            }
            
        }   
    }

    /* TODO */

    return res;
}
