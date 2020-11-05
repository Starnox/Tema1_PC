#include "communication.h"
#include "util_comm.h"

#include <stdio.h>
#include <string.h>


/* Task 1 - The Beginning */

void send_byte_message(void)
{
    /* TODO
     * Send the encoding of the characters: R, I, C, K
     */

    // Pentru fiecare caracter apelez send_squanch cu parametrul encodat
    send_squanch('R' - 'A' + 1);
    send_squanch('I' - 'A' + 1);
    send_squanch('C' - 'A' + 1);
    send_squanch('K' - 'A' + 1);
}


void recv_byte_message(void)
{
    /* TODO
     * Receive 5 encoded characters, decode them and print
     * them to the standard output (as characters)
     *
     * ATTENTION!: Use fprintf(stdout, ...)
     */
    for(int i = 0; i< 5; ++i)
    {
        // Primesc fiecare caracter si il decodez (operatia inversa de la encodat)
        int8_t character = (recv_squanch() + (int8_t)'A' - 1);

        fprintf(stdout,"%c",character);
    }
}


void comm_byte(void)
{
    /* TODO
     * Receive 10 encoded characters and send each character (the character is
     * still encoded) 2 times
     */
    for(int i = 0; i < 10; ++i)
    {
        int8_t received = recv_squanch();

        // Exact ce se cere - trimit de doua ori ce am primit
        send_squanch(received);
        send_squanch(received);
    }


}


/* Task 2 - Waiting for the Message */

void send_message(void)
{
    /* TODO
     * Send the message: HELLOTHERE
     * - send the encoded length
     * - send each character encoded
     */
    char message[] = "HELLOTHERE";

    // Activez bitii corespunzatori encodarii 10 = 8 + 2 = 2^3 + 2^1 
    // care sunt shiftati cu doua pozitii si ajung 2^5 + 2^3

    int8_t encoded_length = 10;
    encoded_length <<= 2;
    

    send_squanch(encoded_length);

    for(int i = 0; i< 10; ++i)
    {
        // trimit fiecare caracter in parte
        send_squanch(message[i] - 'A' + 1);
    }
}


void recv_message(void)
{
    /* TODO
     * Receive a message:
     * - the first value is the encoded length
     * - length x encoded characters
     * - print each decoded character
     * 
     * ATTENTION!: Use fprintf(stdout, ...)
     */

    // lungimea encodata si decodata
    int8_t encoded_length = recv_squanch();
    int8_t decoded_length = 0;

    // Parcurg cei 4 biti aferenti encodarii si verific daca sunt setati
    // Daca sunt atunci ii aduc shiftati cu doua pozitii la stanga in lungimea decodata
    for(int i =2 ; i<= 5; ++i)
    {
        if(encoded_length & (1 << i))
        {
            decoded_length += (1 << (i-2));
        }
    }

    // trimit mesajul si il afisez
    send_squanch(decoded_length);
    fprintf(stdout,"%d",decoded_length);

    // Afisez mesajul
    for(int i = 0; i< decoded_length; ++i)
    {
        int8_t received = recv_squanch();

        fprintf(stdout,"%c",received+'A'-1);
    }


}


void comm_message(void)
{
    /* TODO
     * Receive a message from Rick and do one of the following depending on the
     * last character from the message:
     * - 'P' - send back PICKLERICK
     * - anything else - send back VINDICATORS
     * You need to send the messages as you did at the previous tasks:
     * - encode the length and send it
     * - encode each character and send them
     */

    // Precizare - pentru aceasta functie am importat o biblioteca noua <string.h>
    // Ea este folosita pentru a usura prelucrarea celor doua siruri de caracter


    char message[100];
    int8_t encoded_length = recv_squanch();
    int decoded_length = 0;

    // Se procedeaza la fel ca la functia anterioara
    for(int i =2 ; i<= 5; ++i)
    {
        if(encoded_length & (1 << i))
        {
            decoded_length += (1 << (i-2));
        }
    }

    // Primesc toate caracterele si retin de fiecare data pe ultimul
    int8_t last_received = 0;
    for(int i = 0; i < decoded_length; ++i)
    {
        last_received = recv_squanch();
    }

    // Verific ultimul caracter primit si incarc in variabila message string-ul corespunzator
    if((char)(last_received+'A'-1) == 'P')
    {
        strcpy(message,"PICKLERICK");
    }
    else
    {
        strcpy(message,"VINDICATORS");
    }

    // Encodez lungimea si o trimit
    int length = strlen(message);
    int my_message_encoded_length = (length << 2);

    send_squanch(my_message_encoded_length);


    // Encodez caracterele si le trimit
    for(int i = 0; i< length; ++i)
    {
        int8_t encoded_char = message[i]-'A'+1;
        send_squanch(encoded_char);
    }

}

/* Task 3 - In the Zone */

void send_squanch2(uint8_t c1, uint8_t c2)
{
    /* TODO
     * Steps:
     * - "merge" the character encoded in c1 and the character encoded in c2
     * - use send_squanch to send the newly formed byte
     */

    int8_t res = 0;
    // Parcurg cei 4 biti ai fiecarui caracter
    // c1 porneste de pe bitul 1 si iar bitul c2 de pe bitul 0
    int index1 = 1;
    int index2 = 0;
    for(int i =0; i< 4; ++i)
    {
        // Verific daca bitii sunt setati pentru fiecare dintre caractere si ii asez in pozitia
        // corecta in rezultat folosind cele doua variabile c1 si c2
        if(c1 & (1 << i))
        {
            res |= (1 << index1);
        }
        if(c2 & (1 << i))
        {
            res |= (1 << index2);
        }
        index1+=2;
        index2+=2;
    }
    send_squanch(res);  
}


uint8_t decode_squanch2(uint8_t c)
{
    /*
     * Decode the given byte:
     * - split the two characters as in the image from ocw.cs.pub.ro
     */

    uint8_t res = -1;
    res = 0;

    // pentru bitii de rang impar retin o variabila care memoreaza unde trebuie sa asez bitul urmator
    int index_for_odd = 4;
    for(int i = 0; i< 8; ++i)
    {
        // Daca i e impar
        if(i&1)
        {
            // Daca bitul e setat
            if(c & (1 << i))
            {
                res |= (1 << (index_for_odd));
            }
            index_for_odd++;
        }// Par
        else
        {
            if(c & (1 << i))
            {
                // Setez bitul corespunzator
                res |= (1 << (i / 2));
            }
        }
        
    }

    return res;
}
