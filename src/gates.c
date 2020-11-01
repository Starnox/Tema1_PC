#include "gates.h"

#include <stdio.h>
#include <assert.h>

/* Task 1 - Bit by Bit */

uint8_t get_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint8_t res = -1;

    res = ((nr >> i) & 0x01);

    return res;
}


uint64_t flip_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = -1;

    uint64_t mask = (1LU << i);
    res = (nr ^ mask);

    return res;
}


uint64_t activate_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = 0xFF;

    uint64_t mask = (1LU << i);
    res = (nr | mask);

    return res;
}


uint64_t clear_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = -1;

    uint64_t mask = ~(1LU << i);
    res = (nr & mask);

    return res;
}


/* Task 2 - One Gate to Rule Them All */

uint8_t nand_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    return !(a & b);
}


uint8_t and_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    res = nand_gate(nand_gate(a,b), nand_gate(a,b));

    return res;
}


uint8_t not_gate(uint8_t a)
{
    assert (a == 0 || a == 1);

    uint8_t res = -1;

    res = nand_gate(a,a);

    return res;
}


uint8_t or_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    res = nand_gate(nand_gate(a,a), nand_gate(b,b));

    return res;
}


uint8_t xor_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    uint8_t aux1 = nand_gate(nand_gate(a,b), a);
    uint8_t aux2 = nand_gate(nand_gate(a,b), b);

    res = nand_gate(aux1, aux2);

    return res;
}


/* Task 3 - Just Carry the Bit */

uint8_t full_adder(uint8_t a, uint8_t b, uint8_t c)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);
    assert (c == 0 || c == 1);
    
    uint8_t res = -1;
    uint8_t a_neg = not_gate(a);
    uint8_t b_neg = not_gate(b);
    uint8_t c_neg = not_gate(c);

    uint8_t suma = 0;
    uint8_t carry = 0;

    // il encodez in felul urmator: bitul 0 este bitul carry iar bitul 1 este bitul suma
    // variabile auxiliare pentru suma rezultate din diagrama karnaugh
    uint8_t aux1 = and_gate(a_neg, and_gate(b, c_neg));
    uint8_t aux2 = and_gate(a, and_gate(b_neg, c_neg));
    uint8_t aux3 = and_gate(a_neg, and_gate(b_neg,c));
    uint8_t aux4 = and_gate(a, and_gate(b,c));

    suma = or_gate(or_gate(aux1,aux2), or_gate(aux3,aux4));
    // functie obtinuta tot din diagrama karnaugh dupa minimizare
    uint8_t aux_carry1 = and_gate(a,b);
    uint8_t aux_carry2 = and_gate(b,c);
    uint8_t aux_carry3 = and_gate(a,c);
    carry = or_gate(aux_carry1, or_gate(aux_carry2,aux_carry3));

    res = 0;
    // daca bitul carry e setat
    if(carry)
    {
        res = activate_bit(res, 0);
    }

    // daca bitul suma e setat
    if(suma)
    {
        res = activate_bit(res, 1);
    }

    return res;
}


uint64_t ripple_carry_adder(uint64_t a, uint64_t b)
{
    uint64_t res = -1;

    res = 0;
    uint8_t carry_bit = 0, sum_bit = 0;
    for(int i = 0; i < 64; ++i)
    {
        uint8_t res_full_adder = full_adder(get_bit(a,i), get_bit(b,i), carry_bit);
        
        //printf("i=%d:%d\n",i,res_full_adder);
        // setez carry_bitul pentru urmatorul indice
        carry_bit = get_bit(res_full_adder,0);

        sum_bit = get_bit(res_full_adder, 1);
        // daca bitul suma este setat
        if(sum_bit == 1){
            res = activate_bit(res,i);
        }
    }
    // daca la final carry == 1, inseamana ca avem un overflow
    if(carry_bit == 1)
    {
        return 0;
    }

    return res;
}
