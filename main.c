#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Printers.h"
#include "Documents.h"
#include "Simulation.h"

#define MAX_DOC_TITLE_LENGTH 20

int main () {
    srand(time(NULL));

    Simulation *sim = create_simulation(5, 10, 1000, MAX_DOC_TITLE_LENGTH);
    show_document_queue(sim->doc_queue);
    show_printer_list(sim->printer_list);
    run_simulation(sim);
    free_simulation(sim);

    return 0;
}