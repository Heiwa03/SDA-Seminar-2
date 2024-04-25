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

void show_short_simulation_setup(Simulation *sim) {
    if (sim == NULL) {
        fprintf(stderr, "Error: Simulation is NULL\n");
        return;
    }

    printf("\nGenerated simulation Setup:\n");
    printf("====================================\n");
    printf("Number of printers: %d\n", sim->printer_list->num_printers);
    printf("Number of documents: %d\n", sim->doc_queue->num_docs);
    printf("====================================\n");
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
        //Document* doc = dequeue_document_front(sim->doc_queue);
        pthread_mutex_unlock(&sim->mutex);

        float time_elapsed = print_document(printer, doc);
        printf("DEBUG: Printer %d : Time elapsed for document %d: %.3f\n", printer->printer_id, doc->doc_id, time_elapsed);  // Debugging print statement
        if (time_elapsed != -1) {
            pthread_mutex_lock(&sim->mutex);
            sim->total_num_docs_printed++;
            sim->total_time_elapsed += time_elapsed;
            pthread_mutex_unlock(&sim->mutex);
        }
    }

    free(printer_arg);
    return NULL;
}

void run_simulation(Simulation *sim) {
    struct timespec start, finish;
    double real_time;
    if (sim == NULL) {
        fprintf(stderr, "Error: Simulation is NULL\n");
        return;
    }
    
    pthread_mutex_init(&sim->mutex, NULL);

    Printer* printer = sim->printer_list->head;
    pthread_t* printer_threads = malloc(sizeof(pthread_t) * sim->printer_list->num_printers);
    int i = 0;

    time_t start_time = time(NULL);

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
    time_t end_time = time(NULL);
    real_time = difftime(end_time, start_time);
    //real_time *= 1000;
    sim->real_time_elapsed = real_time;
    _print_simulation_results(sim);
}

void _print_simulation_results(Simulation *sim) {
    if (sim == NULL) {
        fprintf(stderr, "Error: Simulation is NULL\n");
        return;
    }

    printf("\nSimulation Results:\n");
    printf("====================================\n");
    printf("Total number of printers: %d\n", sim->printer_list->num_printers);
    printf("Total number of documents printed: %d\n", sim->total_num_docs_printed);
    printf("Total active printing time: %.3f seconds\n", sim->total_time_elapsed);
    printf("Total real time elapsed: %lf seconds\n", sim->real_time_elapsed);
    printf("====================================\n");
}