#include "Simulation.h"

Simulation *create_simulation(int num_printers, int max_docs, int max_lines, int max_title_length) {
    Simulation *sim = (Simulation *)malloc(sizeof(Simulation));
    if (sim == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for simulation\n");
        exit(1);
    }
    
    sim->printer_list = create_random_printer_list(num_printers);
    sim->doc_queue = create_random_document_queue(max_docs, max_lines, max_title_length);
    
    return sim;
}

void free_simulation(Simulation *sim) {
    if (sim == NULL) {
        return;
    }
    
    free_printer_list(sim->printer_list);
    free_document_queue(sim->doc_queue);
    free(sim);
}

void* printer_thread(void* arg) {
    PrinterThreadArg* printer_arg = (PrinterThreadArg*)arg;
    Simulation* sim = printer_arg->sim;
    Printer* printer = printer_arg->printer;

    while (1) {
        pthread_mutex_lock(&sim->mutex);

        if (sim->doc_queue->head == NULL) {
            pthread_mutex_unlock(&sim->mutex);
            break;
        }

        Document* doc = dequeue_document(sim->doc_queue);
        pthread_mutex_unlock(&sim->mutex);

        float time_elapsed = print_document(printer, doc);
        printf("Time elapsed for document: %f\n", time_elapsed);  // Debugging print statement
        if (time_elapsed != -1) {
            pthread_mutex_lock(&sim->mutex);
            sim->total_num_docs_printed++;
            sim->total_time_elapsed += time_elapsed;
            pthread_mutex_unlock(&sim->mutex);
        }

        free(doc);
    }

    free(printer_arg);
    return NULL;
}

void run_simulation(Simulation *sim) {
    if (sim == NULL) {
        fprintf(stderr, "Error: Simulation is NULL\n");
        return;
    }

    pthread_mutex_init(&sim->mutex, NULL);

    Printer* printer = sim->printer_list->head;
    pthread_t* printer_threads = malloc(sizeof(pthread_t) * sim->printer_list->num_printers);
    int i = 0;

    while (printer != NULL) {
        PrinterThreadArg* arg = malloc(sizeof(PrinterThreadArg));
        arg->sim = sim;
        arg->printer = printer;
        pthread_create(&printer_threads[i], NULL, printer_thread, arg);
        printer = printer->next_printer;
        i++;
    }

    for (i = 0; i < sim->printer_list->num_printers; i++) {
        pthread_join(printer_threads[i], NULL);
    }

    free(printer_threads);
    pthread_mutex_destroy(&sim->mutex);

    printf("Total number of documents printed: %d\n", sim->total_num_docs_printed);
    printf("Total time elapsed: %f seconds\n", sim->total_time_elapsed);
}