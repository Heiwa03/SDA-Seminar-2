#ifndef PRINTERS_H
#define PRINTERS_H

#define MAX_DOC_TITLE_LENGTH 20

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

typedef struct tm tm;

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
    struct Document *next_doc;
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

// Document specific functions
DocumentQueue *create_document_queue();
Document *create_document(int doc_id, char *doc_title, int doc_num_lines);
void add_document(DocumentQueue *doc_queue, Document *doc);
DocumentQueue* create_random_document_queue(int max_docs, int max_lines);
void free_document_queue(DocumentQueue *doc_queue);
// DEBUG: Document specific functions
void show_document_queue(DocumentQueue *doc_queue);

// Printer specific functions
PrinterList *create_printer_list();
Printer *create_printer(int printer_id, tm printer_line_print_time);
void add_printer(PrinterList *printer_list, Printer *printer);
PrinterList* create_random_printer_list(int num_printers);
void free_printer_list(PrinterList *printer_list);
// DEBUG: Printer specific functions
void show_printer_list(PrinterList *printer_list);

// Helper document functions
char* _generate_random_string(int length);
#endif // PRINTERS_H