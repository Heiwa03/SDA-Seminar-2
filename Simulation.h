#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include "Printers.h"
#include "Documents.h"

typedef struct Simulation {
    PrinterList *printer_list;
    DocumentQueue *doc_queue;
    int total_num_docs_printed;
    float total_time_elapsed;
    pthread_mutex_t mutex;  // Add this line
} Simulation;

typedef struct PrinterThreadArg {
    Simulation *sim;
    Printer *printer;
} PrinterThreadArg;

Simulation* create_simulation(int num_printers, int max_docs, int max_lines, int max_title_length);
void free_simulation(Simulation *sim);
void run_simulation(Simulation *sim);

#endif // SIMULATION_H