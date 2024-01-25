#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cjson/cJSON.h"
#include "./../include/render_toolkit.h"

char* readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char*)malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';

    fclose(file);
    return data;
}

void get_preds(Component* pred_comp) {
    char *jsonData = readFile("preds.json");
    if (jsonData == NULL) {
        return 1;
    }

    // Parse the JSON data
    cJSON *json = cJSON_Parse(jsonData);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        free(jsonData);
        return 1;
    }

    cJSON *predString = cJSON_GetObjectItemCaseSensitive(json, "predString");
    if (cJSON_IsString(predString) && (predString->valuestring != NULL)) {
        char* originalString = predString->valuestring;
        size_t length = strlen(originalString) + 1; // +1 for null terminator
        pred_comp->content = malloc(length);
        strcpy(pred_comp->content, originalString);
        // pred_comp->content = new_content;
    }

    // Clean up
    cJSON_Delete(json);
    free(jsonData);
}
