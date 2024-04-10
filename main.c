#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Printers.h"

#define MAX_DOCS 10
#define MAX_LINES 100

int main () {
    srand(time(NULL));

    // Create a random document queue
    DocumentQueue *doc_queue = create_random_document_queue(MAX_DOCS, MAX_LINES);
    if (doc_queue == NULL) {
        printf("Error: Could not create the document queue\n");
        return 1;
    }

    // Show the document queue
    show_document_queue(doc_queue);

    // Create a random printer list
    PrinterList *printer_list = create_random_printer_list(5);
    if (printer_list == NULL) {
        printf("Error: Could not create the printer list\n");
        return 1;
    }

    // Show the printer list
    show_printer_list(printer_list);



    return 0;
}