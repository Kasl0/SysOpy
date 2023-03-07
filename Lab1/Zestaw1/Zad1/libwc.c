#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct results {
    char** result_array;
    int size;
    int taken;
};

struct results init(int size) {
    struct results new_results;
    new_results.result_array = calloc(size, sizeof(char*));
    new_results.size = size;
    new_results.taken = 0;
    return new_results;
}

void wc(struct results* r, const char* filename) {

    char* tmp_filename = "/tmp/tmpfile.txt";

    char command[100] = "wc ";
    strcat(command, filename);
    strcat(command, " > ");
    strcat(command, tmp_filename);
    system(command);

    FILE* fp;
    fp = fopen(tmp_filename, "r");

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = calloc(size, sizeof(char));
    fread(buffer, sizeof(char), size, fp);
    r -> result_array[r->taken] = buffer;
    fclose(fp);

    r -> taken++;

    remove(tmp_filename);
}

char* return_result(struct results* r, int index) {
    return r -> result_array[index];
}

void delete_result(struct results* r, int index) {
    if(r -> result_array[index] != NULL) {
        free(r -> result_array[index]);
        r -> result_array[index] = NULL;
    }
}

void delete_result_array(struct results* r) {
    for(int a=0; a<r->size ; a++) {
        delete_result(r, a);
    }
    if(r->result_array != NULL) free(r->result_array);
}
