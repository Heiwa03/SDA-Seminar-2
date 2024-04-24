#include "Documents.h"


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

DocumentQueue* create_random_document_queue(int max_docs, int max_lines, int max_title_length) {
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
        char* doc_title = _generate_random_string(max_title_length);

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