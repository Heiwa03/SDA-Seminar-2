#ifndef PRINTERS_H
#define PRINTERS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

enum PrinterStatus {
    PRINTER_STATUS_BUSY,
    PRINTER_STATUS_FREE,
    PRINTER_STATUS_OFFLINE
};

typedef struct Document {
    int doc_id;
    char *doc_title;
    int doc_num_lines;
    int doc_num_lines_to_print;
    Document *next_doc;
} Document;

typedef struct DocumentQueue {
    Document *head;
    int num_docs;
    int num_docs_printed;
    int num_docs_not_printed;
} DocumentQueue;

typedef struct Printer {
    int printer_id;
    tm printer_line_print_time;
    Document *printer_current_doc;
    Printer *next_printer;
    enum PrinterStatus printer_status;
    
} Printer;

typedef struct PrinterList {
    Printer *head;
    int num_printers;
    int num_printers_busy;
    int num_printers_free;
    int num_printers_offline;
} PrinterList;

// Document specific functions
DocumentQueue *create_document_queue();
Document *create_document(int doc_id, char *doc_title, int doc_num_lines);
void add_document(DocumentQueue *doc_queue, Document *doc);
DocumentQueue* create_random_document_queue(int max_docs, int max_lines);
// DEBUG: Document specific functions
void show_document_queue(DocumentQueue *doc_queue);

// Printer specific functions
PrinterList *create_printer_list();
Printer *create_printer(int printer_id, tm printer_line_print_time);
void add_printer(PrinterList *printer_list, Printer *printer);

#endif // PRINTERS_H