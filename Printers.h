#ifndef PRINTERS_H
#define PRINTERS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct Document {
    int doc_id;
    char *doc_title;
    int doc_num_lines;
    int doc_num_lines_to_print;
    Document *next_doc;
} Document;

typedef struct Printer {
    int printer_id;
    tm printer_line_print_time;
    Document *printer_current_doc;
    Printer *next_printer;
} Printer;


#endif // PRINTERS_H