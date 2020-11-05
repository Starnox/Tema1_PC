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

    // Variabile care retin numarul de biti setati la rand si indicele de unde incepe vraja
    int number_of_curr_bits = 0;
    int found_spell = 0;

    for(uint16_t i =0; i< 64; ++i)
    {
        // Daca bitul i e setat
        if(memory & (1LU << i))
        {
            number_of_curr_bits++;
        }   
        else
        {
            number_of_curr_bits = 0;
        }

        // Daca gasesc 5 biti consecutivi de 1
        if(number_of_curr_bits == 5)
        {
            // setez de unde incepe vraja si ies din instructiunea repetitiva
            found_spell = i+1;
            break;
        }
    }
    res = 0;
    // Parcurg cei 16 biti din vraja si construiesc rezultatul
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

    // Procedez la fel ca la functia anterioara
    int number_of_curr_bits = 0;
    int found_key = 0;

    for(uint16_t i =0; i< 64; ++i)
    {
        // Daca bitul e setat
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
            // Setez indexul de inceput folosind aritmetica simpla
            // desfasurata pentru claritate
            found_key = i-3-16+1;
            break;
        }
    }
    res = 0;

    // construiesc rezultatul si il returnez
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

    /* ***********Inefficient approach**********
    Parcurg fiecare bit si construiesc in functie de cele doua varibile

    for(uint16_t i = 0; i< 16; ++i)
    {
        // daca bitul i e setat in vraja criptata
        if(spell & (1LU <<i))
        {
            // daca bitu i e setat in cheie
            if(key & (1LU << i))
            {
                // bitul cautat e 0,merg mai departe
                continue;
            }
            else
            {
                // bitul cautat e 1 si il setez
                res |= (1LU << i);
            }
        }
        // invers pe cealalta ramura
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
    */
    // **************Efficient approach*************
    // Functia rezultata in urma analizarii tabelului logic
    /*
        a = spell_encrypted, b = spell_plaintext, c = key 

        a   c   b
        0   0   0
        0   1   1
        1   0   1
        1   1   0   => b = a xor c
    */
    res = spell ^ key;

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
        // Calculez numarul de biti setati
        if(enemy & (1LU << i))
            number_of_set_bits++;
    }
    // Daca numarul e impar
    if(number_of_set_bits & 1)
    {
        enemy = ~enemy + 1;
        res = enemy;
        // Setez ultimii 4 biti
        res |= (1LU << 29);
        res |= (1LU << 30);
    }
    else
    {
        // inamicul e un monstru

        // variabila auxiliara
        uint16_t aux = enemy & (1 - enemy);
        res = aux;

        // Setez ultimii 4 biti
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
    /* O abordare de tip brute-force (prima varianta care mi-a trecut prin minte)
        Complexitate: 2*16 aprox 10^4 Incearca toate variantele pentru antibodies_low
        construieste pe antibodies_high si verifica propietatiile
    */
    // fixing antibodies_low

    /*
    ***************************Inefficient approach - the first one i tried *******************

    // fixez antibodies_low
    uint16_t antibodies_low = 0, antibodies_high = 0;
    for(antibodies_low = 0; antibodies_low < (1LU << 16); ++antibodies_low)
    {
        int ok = 1;
        antibodies_high = 0;
        // merg prin fiecare bit
        for(uint16_t i =0; i < 16; ++i)
        {
            // daca bitul e setat
            if(antibodies_low & (1LU << i))
            {
                // daca cocktail are bitul i 0 
                if(!(cocktail & (1LU << i)))
                {
                    // Solutia nu e buna
                    ok = 0;
                    break;
                }
                
            }
            // daca bitul i e 0 atunci antibodies_high poate sa fie 0 sau 1
            else
            {
                if(cocktail & (1LU << i))
                {
                    antibodies_high |= (1LU << i);
                }
            }
        }
        // daca nu am gasit o solutie buna
        if(ok == 0)
            continue;

        // verific prima conditie
        uint16_t aux1 = antibodies_high & cocktail;
        uint16_t aux2 = antibodies_low | cocktail;
        uint16_t res = aux1 ^ aux2;

        if(res == 0)
        {
            am gasit solutia
            break;
        }
    }

    // building the final result
    res = antibodies_high;
    res <<= 16;
    res |= (antibodies_low);

    */

   /*
        **************Efficient approach********************
        
        a = antibodies_high
        b = antibodies_low
        c = cocktail

            Tabel Logic

        a   0   1   0   1
        b   1   0   0   1
                        Nu este bun pentru ca a&b = 0
        c   1   1   0   
        a&c 0   1   0
        b|c 1   1   0
 (a&c)^(b|c)1   0   0
                singurele optiuni valide -> a == c and b == 0
   */
    res = cocktail;
    res <<= 16;

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
    // presupunem ca padure e de tipul 0 si are grupari de 4 copaci consecutivi
    uint16_t ok = 1;
    for(uint64_t i = 0; i < 64; ++i)
    {
        // daca e copac pe bitul i
        if(map & (1LU << i))
            number_of_trees++;
        else
        {
            // daca nu e grup de 4 copaci
            if(number_of_trees % 4 != 0)
                ok = 0;
        }
        
    }

    // se alege tipul de padure
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

    // last retine pozitia primului copac
    int last = -1;
    for(uint32_t i = 0; i< 32; ++i)
    {
        // daca bitul e setat
        if(map & (1LU << i))
        {
            // daca e primul pe care l-am gasit
            if(last == -1)
                last = i;
            else
            {
                // seteaza rezultatul si iese din instructiunea repetitiva
                res = i - last;
                break;
            }
            
        }   
    }

    /* TODO */

    return res;
}
