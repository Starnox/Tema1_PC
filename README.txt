PC - tema 1
Autor: Mihailescu Eduard-Florin
Seria:CB
Grupa:312

GATES.c
    Task1
        Au fost construite functii elementare folosind operatii pe biti si tehnica de "masca".
    
    Task2
        Folosirea portii nand_gate pentru a reprezenta celalate porti a fost factuta urmarind schema
        data si legea lui De Morgen.

    Task3
        Implementarea a fost realizata rapid dupa ce am reprezentat cele doua functii(suma si carry)
        pe foaie in diagrama Karnaugh si am efectuat minimizarea. Modul de encodare al rezultatului
        este explicat in comentarii in fisierul sursa. Functia ripple_carry_adder urmareste intocmai
        diagrama data: activeaza bitul de suma atunci cand este cazul si memoreaza bitul carry pentru
        urmatoarea operatie. In cazul in care bitul carry este la final 1, inseamna ca ar mai
        mai trebui adaugat un bit la final ceea ce conduce la un overflow.
    
COMMUNICATION.c
    Precizari: encodarea se realizeaza in modul urmator:
        - se scade din codul ASCII al caracterul de encodat codul caracterului 'A'
            si se adauga o unitate pentru a face indexarea de la 1
    Decodarea este procesul invers: Se adauga codul ASCII al caracterul 'A' si se scade 1.

    Task1
        Functii simple, o explicatie sumara a lor se regaseste in codul sursa.

    Task2
        Am folosit un sir de caractere pentru a automatiza putin procesul de trimitere.
        Encodarea lungimii se realizeaza prin shiftarea la stanga cu 2 biti.
        Pentru functia comm_message am importat biblioteca <string.h> pentru a folosi
        functiile strcpy() si  strlen(). Acestea ajuta la infrumusetarea codului si 
        automatizarea sa.

    Task3
        Pentru encodare am folosit 2 variabile de indexare care sunt incremetate la fiecare pas
        cu 2 unitati. Acestea au rolul de a marca pozitia unde trebuie inserat bitul.
        Pentru decodare am abordat o tehnica diferita in functie de paritatea bitului i;
        Aceasta este descrisa in codul sursa.

HUNT.c
    Task1
        Modul de functionare este foarte bine documentat in comenatarii.

    Task2
        Subtaskul trial_of_the_grasses a fost cel mai dificil. Initial am implementat
        un brut pe biti cu complexitatea 2^16 pentru a testa un pic inputul si outputul
        insa dupa o analiza atenta a tabelului expresilor logice si a datelor de iesire
        am observat regula si anume: antibodies_high = cocktail iar antibodies_low = 0;
        Pentru a intializa rezultatul, am dat valoarea lui res = antibodies_high si am shiftat
        cu 16 biti la stanga. O analiza mai detaliata se afla in comentarii.

        Restul taskurilor sunt destul de "straight-forward' (detalii in cod).

Posibile Optimizari:
    Implementarea functiilor des folosite (get_bit() si activate_bit()) din gates.c in
    celalate doua fisiere.
    Standardizare variabilei de incrementare in instructiunile repetitive.





