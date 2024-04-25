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
    char user_choice = 'n';
    Simulation *sim = NULL;

    do {
    sim = create_simulation(5, 10, 1000, MAX_DOC_TITLE_LENGTH);

    if (sim == NULL) {
        fprintf(stderr, "Error: Unable to create simulation\n");
        return 1;
    }
    show_short_simulation_setup(sim);
    printf("Do you want to run the simulation with this setup? (y/n): ");
    scanf("%c", &user_choice);
    getchar();
    if (user_choice != 'y' && user_choice != 'n') {
        printf("Invalid option.\n");
        user_choice = 'n';
    }
    } while (user_choice == 'n');

    show_document_queue(sim->doc_queue);
    show_printer_list(sim->printer_list);

    run_simulation(sim);
    free_simulation(sim);

    return 0;
}