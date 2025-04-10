#ifndef FUNC_H
#define FUNC_H

#include "type.h"

// Fonction de gestion du disque
int load_disk(const char* disk_path);

// Fonctions pour gérer les fichiers
int create_file(const char* name);
int write_file(const char* name, const void* data, size_t size);
int read_file(const char* name, void* buffer, size_t size);
void list_files();

#endif // FUNC_H

