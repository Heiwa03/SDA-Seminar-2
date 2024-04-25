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

        // If the document queue is empty and no document is assigned to this printer, exit the loop
        if (sim->done && sim->doc_queue->head == NULL && printer->printer_current_doc == NULL) {
            pthread_mutex_unlock(&sim->mutex);
            break;
        }

        Document* doc = printer->printer_current_doc;
        //printer->printer_current_doc = NULL;  // Clear the assigned document

        pthread_mutex_unlock(&sim->mutex);

        if (doc != NULL) {
            float time_elapsed = print_document(printer, doc);
            if (time_elapsed != -1) {
                pthread_mutex_lock(&sim->mutex);
                sim->total_num_docs_printed++;
                sim->total_time_elapsed += time_elapsed;
                pthread_mutex_unlock(&sim->mutex);
            }
        }

        // Sleep for a short period of time to simulate the time it takes for the printer to become ready for the next document
        usleep(1000);
    }

    free(printer_arg);
    return NULL;
}

void* dispatcher_thread(void* arg) {
    Simulation* sim = (Simulation*)arg;

    while (1) {
        pthread_mutex_lock(&sim->mutex);

        if (sim->doc_queue->head == NULL) {
            sim->done = 1;
            pthread_mutex_unlock(&sim->mutex);
            break;
        }

        Document* doc = dequeue_document(sim->doc_queue);

        // Find the fastest available printer
        Printer* fastest_printer = find_fastest_printer(sim->printer_list);

        // If no printer is available, wait and try again
        if (fastest_printer == NULL) {
            pthread_mutex_unlock(&sim->mutex);
            continue;
        }

        // Assign the document to the fastest printer
        fastest_printer->printer_current_doc = doc;

        pthread_mutex_unlock(&sim->mutex);

        // Sleep for a short period of time to simulate the time it takes for the dispatcher to assign the next document
        usleep(1000);
    }

    return NULL;
}

void run_simulation(Simulation *sim) {
    struct timespec start, finish;
    double real_time;
    if (sim == NULL) {
        fprintf(stderr, "Error: Simulation is NULL\n");
        return;
    }

    sim->done = 0;
    
    clock_gettime(CLOCK_REALTIME, &start);
    pthread_mutex_init(&sim->mutex, NULL);

    // Create the dispatcher thread
    pthread_t dispatcher;
    pthread_create(&dispatcher, NULL, dispatcher_thread, sim);

    // Create a thread for each printer
    int num_printers = sim->printer_list->num_printers;
    pthread_t printer_threads[num_printers];
    Printer* current_printer = sim->printer_list->head;
    for (int i = 0; i < num_printers; i++) {
        PrinterThreadArg* printer_arg = malloc(sizeof(PrinterThreadArg));
        printer_arg->sim = sim;
        printer_arg->printer = current_printer;
        pthread_create(&printer_threads[i], NULL, printer_thread, printer_arg);
        current_printer = current_printer->next_printer;
    }

    // Wait for the dispatcher thread to finish
    pthread_join(dispatcher, NULL);

    // Wait for all printer threads to finish
    for (int i = 0; i < num_printers; i++) {
        pthread_join(printer_threads[i], NULL);
    }

    pthread_mutex_destroy(&sim->mutex);
    clock_gettime(CLOCK_REALTIME, &finish);
    real_time = (finish.tv_sec - start.tv_sec);
    real_time += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    real_time *= 1000;
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