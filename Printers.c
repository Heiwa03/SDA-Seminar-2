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

Document *create_document(int doc_id, char *doc_title, int doc_num_lines) {
    Document *doc = (Document *)malloc(sizeof(Document));
    doc->doc_id = doc_id;
    doc->doc_title = doc_title;
    doc->doc_num_lines = doc_num_lines;
    doc->doc_num_lines_to_print = doc_num_lines;
    doc->next_doc = NULL;
    return doc;
}

void add_document(DocumentQueue *doc_queue, Document *doc) {
    // Check if the document queue is empty
    if (doc_queue->head == NULL) {
        doc_queue->head = doc;
        doc->doc_id = 1; // Start the id from 1
    } else {
        // Find the last document in the queue
        Document *current_doc = doc_queue->head;
        while (current_doc->next_doc != NULL) {
            current_doc = current_doc->next_doc;
        }

        // Check if the document id is unique and increment it from the last id
        if (doc->doc_id <= current_doc->doc_id) {
            doc->doc_id = current_doc->doc_id + 1;
        }

        // Add the new document to the end of the queue
        current_doc->next_doc = doc;
    }

    // Increment the number of documents in the queue
    doc_queue->num_docs++;
    doc_queue->num_docs_not_printed++;
}

// Printer specific functions


// Helper Printers functions


// Helper Document functions
