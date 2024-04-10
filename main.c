#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "Printers.h"

#define MAX_DOCS 10
#define MAX_LINES 100

void simulate_printing(PrinterList* printer_list, DocumentQueue* doc_queue);

int main () {
    srand(time(NULL));

    DocumentQueue *doc_queue = create_random_document_queue(MAX_DOCS, MAX_LINES);
    if (doc_queue == NULL) {
        printf("Error: Could not create the document queue\n");
        return 1;
    }

    PrinterList *printer_list = create_random_printer_list(5);
    if (printer_list == NULL) {
        printf("Error: Could not create the printer list\n");
        return 1;
    }

    // DEBUG: Show the document queue and printer list
    show_document_queue(doc_queue);
    show_printer_list(printer_list);

    simulate_printing(printer_list, doc_queue);

    free_document_queue(doc_queue);
    free_printer_list(printer_list);

    return 0;
}

void simulate_printing(PrinterList* printer_list, DocumentQueue* doc_queue) {
    int printing_in_progress = 0;
    do {
        // Assign documents to printers
        Printer* current_printer = printer_list->head;
        while (current_printer != NULL && doc_queue->head != NULL) {
            if (current_printer->printer_status == PRINTER_STATUS_FREE) {
                // Assign the first document in the queue to the printer
                current_printer->printer_current_doc = doc_queue->head;
                doc_queue->head = doc_queue->head->next_doc;
                doc_queue->num_docs--;

                // Set the printer status to busy
                current_printer->printer_status = PRINTER_STATUS_BUSY;
            }

            current_printer = current_printer->next_printer;
        }

        // Simulate the printing process
        printing_in_progress = 0;
        current_printer = printer_list->head;
        while (current_printer != NULL) {
            if (current_printer->printer_status == PRINTER_STATUS_BUSY) {
                // Print one line of the document
                current_printer->printer_current_doc->doc_num_lines_to_print--;
                printf("Printer %d is printing document %d (%d lines left)\n",
                       current_printer->printer_id,
                       current_printer->printer_current_doc->doc_id,
                       current_printer->printer_current_doc->doc_num_lines_to_print);
                usleep(30000); // Simulate the time it takes to print a line
                // If the document is fully printed, set the printer status to free
                if (current_printer->printer_current_doc->doc_num_lines_to_print == 0) {
                    current_printer->printer_status = PRINTER_STATUS_FREE;
                    free(current_printer->printer_current_doc);
                    current_printer->printer_current_doc = NULL;
                } else {
                    printing_in_progress = 1;
                }
            }

            current_printer = current_printer->next_printer;
        }
    } while (doc_queue->num_docs > 0 || printing_in_progress);
}