#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "Printers.h"

#define MAX_DOCS 10
#define MAX_LINES 100

DocumentQueue *doc_queue;

void simulate_printing(PrinterList* printer_list, DocumentQueue* doc_queue);
void* printer_thread(void* arg);

int main () {
    srand(time(NULL));

    doc_queue = create_random_document_queue(MAX_DOCS, MAX_LINES);
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

// Function for each printer thread
void* printer_thread(void* arg) {
    Printer* printer = (Printer*)arg;

    while (1) {
        pthread_mutex_lock(&printer->printer_mutex);

        // If the printer has a document, print it
        if (printer->printer_current_doc != NULL) {
            printer->printer_current_doc->doc_num_lines_to_print--;
            printf("Printer %d is printing document %d (%d lines left)\n",
                   printer->printer_id,
                   printer->printer_current_doc->doc_id,
                   printer->printer_current_doc->doc_num_lines_to_print);

            // If the document is fully printed, set the printer status to free
            if (printer->printer_current_doc->doc_num_lines_to_print == 0) {
                printer->printer_status = PRINTER_STATUS_FREE;
                free(printer->printer_current_doc);
                printer->printer_current_doc = NULL;
            }
        }

        pthread_mutex_unlock(&printer->printer_mutex);

        // If there are no more documents and the printer is free, exit the loop
        if (doc_queue->num_docs == 0 && printer->printer_status == PRINTER_STATUS_FREE) {
            break;
        }

        // Sleep for a second to simulate real-time printing
        usleep(500*1000);
    }

    return NULL;
}

void simulate_printing(PrinterList* printer_list, DocumentQueue* doc_queue) {
    // Create a thread for each printer
    Printer* current_printer = printer_list->head;
    pthread_t* printer_threads = malloc(sizeof(pthread_t) * printer_list->num_printers);
    int i = 0;

    while (current_printer != NULL) {
        pthread_create(&printer_threads[i], NULL, printer_thread, current_printer);
        current_printer = current_printer->next_printer;
        i++;
    }

    // While there are still documents in the queue
    while (doc_queue->num_docs > 0) {
        // Assign documents to printers
        current_printer = printer_list->head;
        while (current_printer != NULL && doc_queue->head != NULL) {
            pthread_mutex_lock(&current_printer->printer_mutex);

            if (current_printer->printer_status == PRINTER_STATUS_FREE) {
                // Assign the first document in the queue to the printer
                current_printer->printer_current_doc = doc_queue->head;
                doc_queue->head = doc_queue->head->next_doc;
                doc_queue->num_docs--;

                // Set the printer status to busy
                current_printer->printer_status = PRINTER_STATUS_BUSY;
            }

            pthread_mutex_unlock(&current_printer->printer_mutex);
            current_printer = current_printer->next_printer;
        }
    }

    // Wait for all printer threads to finish
    for (i = 0; i < printer_list->num_printers; i++) {
        pthread_join(printer_threads[i], NULL);
    }

    free(printer_threads);
}