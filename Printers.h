#ifndef PRINTERS_H
#define PRINTERS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "Documents.h"

enum PrinterStatus {
    PRINTER_STATUS_BUSY,
    PRINTER_STATUS_FREE,
    PRINTER_STATUS_OFFLINE
};

typedef struct Printer {
    int printer_id;
    unsigned int printer_line_print_time; // in lines per second
    Document *printer_current_doc;
    struct Printer *next_printer;
    enum PrinterStatus printer_status;
} Printer;

typedef struct PrinterList {
    Printer *head;
    int num_printers;
    int num_printers_busy;
    int num_printers_free;
    int num_printers_offline;
} PrinterList;

// Printer specific functions
PrinterList *create_printer_list();
Printer *create_printer(int printer_id, unsigned int printer_line_print_time);
void add_printer(PrinterList *printer_list, Printer *printer);
PrinterList* create_random_printer_list(int num_printers);
void free_printer_list(PrinterList *printer_list);
// DEBUG: Printer specific functions
void show_printer_list(PrinterList *printer_list);

// Returns the time it took to print the document or -1 if the printer is unavailable
float print_document(Printer *printer, Document *doc);

#endif // PRINTERS_H