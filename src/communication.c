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

        // Decode and send
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

    int8_t encoded_length = 0;
    encoded_length |= (1 << 3);
    encoded_length |= (1 << 5);
    // Send the encoded length

    send_squanch(encoded_length);

    // Send the message
    for(int i = 0; i< 10; ++i)
    {
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
    
    int8_t encoded_length = recv_squanch();
    int decoded_length = 0;
    for(int i =2 ; i<= 5; ++i)
    {
        if(encoded_length & (1 << i))
        {
            decoded_length += (1 << (i-2));
        }
    }
    send_squanch(decoded_length);
    fprintf(stdout,"%d",decoded_length);
    // print the recived message
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

    char message[100];
    int8_t encoded_length = recv_squanch();
    int decoded_length = 0;
    for(int i =2 ; i<= 5; ++i)
    {
        if(encoded_length & (1 << i))
        {
            decoded_length += (1 << (i-2));
        }
    }
    int8_t last_received = 0;
    for(int i = 0; i < decoded_length; ++i)
    {
        last_received = recv_squanch();
    }

    // If the last character of the message is P
    if((char)(last_received+'A'-1) == 'P')
    {
        strcpy(message,"PICKLERICK");
    }
    else
    {
        strcpy(message,"VINDICATORS");
    }

    // encode the length and send it
    int length = strlen(message);
    int my_message_encoded_length = 0;

    for(int i = 0; i< 8; ++i)
    {
        if(length & (1 << i))
        {
            my_message_encoded_length |= (1 << (i+2));
        }
    }
    send_squanch(my_message_encoded_length);


    // encode the characters and send them
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
    // Go through all the 4 bits of both characters
    // c1 starts from bit 1 and c2 starts from bit 0
    int ct1 = 1;
    int ct2 = 0;
    for(int i =0; i< 4; ++i)
    {
        if(c1 & (1 << i))
        {
            res |= (1 << ct1);
        }
        if(c2 & (1 << i))
        {
            res |= (1 << ct2);
        }
        ct1+=2;
        ct2+=2;
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
    int ct_for_odd = 4;
    for(int i = 0; i< 8; ++i)
    {
        // if odd
        if(i&1)
        {
            if(c & (1 << i))
            {
                res |= (1 << (ct_for_odd));
            }
            ct_for_odd++;
        }// even
        else
        {
            if(c & (1 << i))
            {
                res |= (1 << (i / 2));
            }
        }
        
    }

    /* TODO */

    return res;
}
