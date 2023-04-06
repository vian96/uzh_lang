#include <stdio.h>
#include <stdarg.h>

void Print (int num) {
    printf ("OUT: %d\n", num);
}

int Scan () {
    int x = 0;
    printf ("Input int: ");

    while (!scanf ("%d", &x)) {
        printf ("There is an error in your input, try again:\n");
         while (getchar () != '\n') { ; } // clears input buffer
    }

    return x;
}

