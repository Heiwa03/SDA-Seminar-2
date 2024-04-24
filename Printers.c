#include "Printers.h"

// Printer specific functions
PrinterList* create_printer_list() {
    PrinterList* printer_list = (PrinterList*) malloc(sizeof(PrinterList));
    if (printer_list == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    printer_list->head = NULL;
    printer_list->num_printers = 0;
    printer_list->num_printers_busy = 0;
    printer_list->num_printers_free = 0;
    printer_list->num_printers_offline = 0;

    return printer_list;
}

/*
Printer* create_printer(int printer_id, struct tm printer_line_print_time) {
    // Allocate memory for the new Printer
    Printer* printer = (Printer*) malloc(sizeof(Printer));

    // Check if the memory allocation was successful
    if (printer == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Initialize the values
    printer->printer_id = printer_id;
    printer->printer_line_print_time = printer_line_print_time;
    printer->printer_current_doc = NULL;
    printer->next_printer = NULL;
    printer->printer_status = PRINTER_STATUS_FREE;
    pthread_mutex_init(&printer->printer_mutex, NULL);

    return printer;
}*/

Printer *create_printer(int printer_id, unsigned int printer_line_print_time) {
    // Allocate memory for new Printer
    Printer* printer = (Printer*) malloc(sizeof(Printer));

    if (printer == NULL) {
        printf("Memory allocation failed.\n"); // It's most likely that malloc might fuck up at this step but,.... oh well...
        return NULL;
    }

    printer->printer_id = printer_id;
    printer->printer_line_print_time = printer_line_print_time;
    printer->printer_current_doc = NULL;
    printer->next_printer = NULL;
    printer->printer_status = PRINTER_STATUS_FREE;

    return printer;
}

void add_printer(PrinterList *printer_list, Printer *printer) {
    if (printer_list->head == NULL) {
        printer_list->head = printer;
    } else {
        // Find last printer in list
        Printer *current_printer = printer_list->head;
        while (current_printer->next_printer != NULL) {
            current_printer = current_printer->next_printer;
        }

        // Add new printer to end of list
        current_printer->next_printer = printer;
    }

    // Increment the number of printers in list
    printer_list->num_printers++;

    // Update the printer status counts (it's supposed to only be free at this point as offline is only in case of breakage)
    switch (printer->printer_status) {
        case PRINTER_STATUS_BUSY:
            printer_list->num_printers_busy++;
            break;
        case PRINTER_STATUS_FREE:
            printer_list->num_printers_free++;
            break;
        case PRINTER_STATUS_OFFLINE:
            printer_list->num_printers_offline++; //SHOW ONLY HAPPEN IF THE PRINTER IS BROKEN NOT AT INITIALIZATION
            break;
    }
}
/*
PrinterList* create_random_printer_list(int max_printers) {
    // Create a new printer list
    PrinterList* printer_list = create_printer_list();
    if (printer_list == NULL) {
        return NULL;
    }

    // Generate a random number of printers
    int num_printers = (1 + rand()) % (max_printers + 1);
    for (int i = 0; i < num_printers; i++) {
        // Create a new printer
        struct tm printer_line_print_time;
        time_t t = time(NULL);
        printer_line_print_time = *localtime(&t);

        Printer* printer = create_printer(i + 1, printer_line_print_time);
        if (printer == NULL) {
            return NULL;
        }

        // Add the printer to the list
        add_printer(printer_list, printer);
    }

    return printer_list;
}*/

PrinterList* create_random_printer_list(int max_printers) {
    // Create new printer list
    PrinterList* printer_list = create_printer_list();
    if (printer_list == NULL) {
        return NULL;
    }

    // Random get of printers
    int num_printers = (1 + rand()) % (max_printers + 1);
    for (int i = 0; i < num_printers; i++) {
        // Random gen of printer line print time
        int printer_line_print_time;
        printer_line_print_time = rand() % 10;

        Printer* printer = create_printer(i + 1, printer_line_print_time);
        if (printer == NULL) {
            return NULL;
        }

        add_printer(printer_list, printer);
    }

    return printer_list;
}

/*
void show_printer_list(PrinterList *printer_list) {
    // Check if the printer list is empty
    if (printer_list->head == NULL) {
        printf("Printer list is empty.\n");
        return;
    }

    // Print the details of each printer in the list
    Printer *current_printer = printer_list->head;
    while (current_printer != NULL) {
        printf("Printer ID: %d\n", current_printer->printer_id);
        printf("Printer Line Print Time: %s", asctime(&(current_printer->printer_line_print_time)));
        printf("Printer Status: ");
        switch (current_printer->printer_status) {
            case PRINTER_STATUS_BUSY:
                printf("Busy\n");
                break;
            case PRINTER_STATUS_FREE:
                printf("Free\n");
                break;
            case PRINTER_STATUS_OFFLINE:
                printf("Offline\n");
                break;
        }
        printf("\n");

        current_printer = current_printer->next_printer;
    }
}*/

void show_printer_list(PrinterList *printer_list) {
    if (printer_list->head == NULL) {
        printf("Printer list is empty.\n");
        return;
    }

    Printer *current_printer = printer_list->head;
    while (current_printer != NULL) {
        printf("Printer ID: %d\n", current_printer->printer_id);
        printf("Printer Line Print Time: %u lps\n", current_printer->printer_line_print_time);
        printf("Printer Status: ");
        switch (current_printer->printer_status) {
            case PRINTER_STATUS_BUSY:
                printf("Busy\n");
                break;
            case PRINTER_STATUS_FREE:
                printf("Free\n");
                break;
            case PRINTER_STATUS_OFFLINE:
                printf("Offline\n");
                break;
        }
        printf("\n");

        current_printer = current_printer->next_printer;
    }
}

void free_printer_list(PrinterList *printer_list) {
    if (printer_list != NULL) {
        // Free each printer in the list
        Printer *current_printer = printer_list->head;
        while (current_printer != NULL) {
            Printer *next_printer = current_printer->next_printer;
            // Free the current document in the printer
            if (current_printer->printer_current_doc != NULL) {
                free(current_printer->printer_current_doc->doc_title);
                free(current_printer->printer_current_doc);
            }
            // Free the printer itself
            free(current_printer);
            current_printer = next_printer;
        }

        free(printer_list);
    }
}
