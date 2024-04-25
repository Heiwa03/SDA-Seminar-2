#ifndef DOCUMENTS_H
#define DOCUMENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

typedef struct Document {
    int doc_id;
    char *doc_title;
    int doc_num_lines;
    int doc_num_lines_to_print;
    struct Document *next_doc;
} Document;

typedef struct DocumentQueue {
    Document *head;
    int num_docs;
    int num_docs_printed;
    int num_docs_not_printed;
} DocumentQueue;

DocumentQueue *create_document_queue();
Document *create_document(int doc_id, char *doc_title, int doc_num_lines);
void add_document(DocumentQueue *doc_queue, Document *doc);
DocumentQueue* create_random_document_queue(int max_docs, int max_lines, int max_title_length);
void free_document_queue(DocumentQueue *doc_queue);
// DEBUG: Document specific functions
void show_document_queue(DocumentQueue *doc_queue);
Document *dequeue_document(DocumentQueue *doc_queue);

// Helper document functions
char* _generate_random_string(int length);


#endif // DOCUMENTS_H