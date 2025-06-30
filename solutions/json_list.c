#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_list.h"

JObject* parse_json(const char *filename) {
    json_error_t error;
    json_t *root = json_load_file(filename, 0, &error);
    if (!root) {
        fprintf(stderr, "Fehler beim Laden der JSON-Datei: %s\n", error.text);
        return NULL;
    }

    if (!json_is_array(root)) {
        fprintf(stderr, "JSON ist kein Array.\n");
        json_decref(root);
        return NULL;
    }

    size_t index;
    json_t *value;
    JObject *head = NULL;
    JObject *tail = NULL;

    json_array_foreach(root, index, value) {
        if (!json_is_object(value)) {
            fprintf(stderr, "Element %zu ist kein Objekt.\n", index);
            continue;
        }

        json_t *jname = json_object_get(value, "name");
        json_t *jage  = json_object_get(value, "age");

        if (!json_is_string(jname) || !json_is_integer(jage)) {
            fprintf(stderr, "Ungültige Felder in Element %zu.\n", index);
            continue;
        }

        JObject *node = malloc(sizeof(JObject));
        if (!node) {
            fprintf(stderr, "Speicherallokation fehlgeschlagen.\n");
            json_decref(root);
            free_list(head);
            return NULL;
        }

        const char *name_str = json_string_value(jname);
        node->name = strdup(name_str);
        node->age = (int)json_integer_value(jage);
        node->next = NULL;

        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    json_decref(root);
    return head;
}

void print_list(JObject *head) {
    for (JObject *p = head; p != NULL; p = p->next) {
        printf("Name: %s, Age: %d\n", p->name, p->age);
    }
}

void free_list(JObject *head) {
    JObject *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp->name);
        free(tmp);
    }
}
