#include "Printers.h"

// Document specific functions
DocumentQueue *create_document_queue() {
    DocumentQueue *doc_queue = (DocumentQueue *)malloc(sizeof(DocumentQueue));
    doc_queue->head = NULL;
    doc_queue->num_docs = 0;
    doc_queue->num_docs_printed = 0;
    doc_queue->num_docs_not_printed = 0;
    return doc_queue;
}

Document* create_document(int doc_id, char* doc_title, int doc_num_lines) {
    Document* doc = (Document*) malloc(sizeof(Document));

    if (doc == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    doc->doc_id = doc_id;

    // FIX: Copy by value for doc_title otherwise we lose it after generation
    doc->doc_title = malloc(strlen(doc_title) + 1);
    if (doc->doc_title != NULL) {
        strcpy(doc->doc_title, doc_title);
    }

    doc->doc_num_lines = doc_num_lines;
    doc->doc_num_lines_to_print = doc_num_lines;
    doc->next_doc = NULL;

    return doc;
}

void add_document(DocumentQueue *doc_queue, Document *doc) {
    // Check if the doc queue is empty
    if (doc_queue->head == NULL) {
        doc_queue->head = doc;
        doc->doc_id = 1; // Start the id from 1
    } else {
        // Find the last doc in the queue
        Document *current_doc = doc_queue->head;
        while (current_doc->next_doc != NULL) {
            current_doc = current_doc->next_doc;
        }

        // Check if the doc id is unique and increment it from the last id
        if (doc->doc_id <= current_doc->doc_id) {
            doc->doc_id = current_doc->doc_id + 1;
        }

        // Add the new doc to the end of the queue
        current_doc->next_doc = doc;
    }

    // Increment number of docs in the queue
    doc_queue->num_docs++;
    doc_queue->num_docs_not_printed++;
}

DocumentQueue* create_random_document_queue(int max_docs, int max_lines) {
    // Create a new doc queue
    DocumentQueue* doc_queue = create_document_queue();
    if (doc_queue == NULL) {
        return NULL;
    }

    // Random gen of docs
    int num_docs = (1 + rand()) % (max_docs + 1);
    for (int i = 0; i < num_docs; i++) {
        // Random gen of lines
        int num_lines = (1 + rand()) % (max_lines + 1);

        // Random get of titles
        char* doc_title = _generate_random_string(MAX_DOC_TITLE_LENGTH);

        Document* doc = create_document(i + 1, doc_title, num_lines);
        if (doc == NULL) {
            free(doc_title);
            return NULL;
        }

        // Add the doc to the queue
        add_document(doc_queue, doc);

        // Free the title string
        free(doc_title);
    }

    return doc_queue;
}

void show_document_queue(DocumentQueue *doc_queue) {
    if (doc_queue->head == NULL) {
        printf("Document queue is empty.\n");
        return;
    }

    Document *current_doc = doc_queue->head;
    while (current_doc != NULL) {
        printf("Document ID: %d\n", current_doc->doc_id);
        printf("Document Title: %s\n", current_doc->doc_title);
        printf("Number of Lines: %d\n", current_doc->doc_num_lines);
        printf("Number of Lines to Print: %d\n", current_doc->doc_num_lines_to_print);
        printf("\n");

        current_doc = current_doc->next_doc;
    }
}

void free_document_queue(DocumentQueue *doc_queue) {
    if (doc_queue != NULL) {
        // Free each doc in queue
        Document *current_doc = doc_queue->head;
        while (current_doc != NULL) {
            Document *next_doc = current_doc->next_doc;
            // Free the doc title (Otherwise MEMORY LEAAAAAAKS)
            free(current_doc->doc_title);
            // Free the doc
            free(current_doc);
            current_doc = next_doc;
        }

        // Free the doc queue
        free(doc_queue);
    }
}

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
    pthread_mutex_init(&printer->printer_mutex, NULL);

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
            pthread_mutex_destroy(&current_printer->printer_mutex);  // Destroy the mutex
            free(current_printer);
            current_printer = next_printer;
        }

        free(printer_list);
    }
}

// Helper Printers functions


// Helper Document functions
char* _generate_random_string(int length) {
    char* string = malloc((length + 1) * sizeof(char));
    char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    if (string) {
        for (int i = 0; i < length; ++i) {
            string[i] = letters[rand() % (sizeof(letters) - 1)];
        }
        string[length] = '\0';
    }

    return string;
}