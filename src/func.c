#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "../include/func.h"

#define BLOCK_SIZE 512
#define MAX_FILES 128
#define FAT_START_BLOCK 1
#define FILE_ENTRIES_BLOCK 2
#define DATA_START_BLOCK 10

static FileSystem fs;
static int disk_fd = -1;

int load_disk(const char* disk_path) {
    disk_fd = open(disk_path, O_RDWR);
    if (disk_fd < 0) {
        perror("Erreur ouverture disque");
        return -1;
    }

    // Lire le superbloc
    lseek(disk_fd, 0, SEEK_SET);
    read(disk_fd, &fs.sb, sizeof(SuperBlock));
    return 0;
}

int save_file_entries(FileEntry* entries) {
    lseek(disk_fd, FILE_ENTRIES_BLOCK * BLOCK_SIZE, SEEK_SET);
    write(disk_fd, entries, sizeof(FileEntry) * MAX_FILES);
    return 0;
}

int create_file(const char* name) {
    if (strlen(name) >= MAX_NAME_LEN) {
        fprintf(stderr, "Nom trop long\n");
        return -1;
    }

    FileEntry entries[MAX_FILES];
    lseek(disk_fd, FILE_ENTRIES_BLOCK * BLOCK_SIZE, SEEK_SET);
    read(disk_fd, entries, sizeof(entries));

    for (int i = 0; i < MAX_FILES; i++) {
        if (entries[i].used && strcmp(entries[i].name, name) == 0) {
            fprintf(stderr, "Fichier déjà existant\n");
            return -1;
        }
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (!entries[i].used) {
            strcpy(entries[i].name, name);
            entries[i].size = 0;
            entries[i].start_block = DATA_START_BLOCK + i * 8;
            entries[i].used = 1;
            time(&entries[i].created_at);
            entries[i].modified_at = entries[i].created_at;
            strcpy(entries[i].owner, "bill");
            entries[i].permissions = 0644;
            save_file_entries(entries);
            return 0;
        }
    }

    fprintf(stderr, "Trop de fichiers\n");
    return -1;
}

int write_file(const char* name, const void* data, size_t size) {
    FileEntry entries[MAX_FILES];
    lseek(disk_fd, FILE_ENTRIES_BLOCK * BLOCK_SIZE, SEEK_SET);
    read(disk_fd, entries, sizeof(entries));

    for (int i = 0; i < MAX_FILES; i++) {
        if (entries[i].used && strcmp(entries[i].name, name) == 0) {
            lseek(disk_fd, entries[i].start_block * BLOCK_SIZE, SEEK_SET);
            write(disk_fd, data, size);
            entries[i].size = size;
            time(&entries[i].modified_at);
            save_file_entries(entries);
            return 0;
        }
    }

    fprintf(stderr, "Fichier non trouvé\n");
    return -1;
}

int read_file(const char* name, void* buffer, size_t size) {
    FileEntry entries[MAX_FILES];
    lseek(disk_fd, FILE_ENTRIES_BLOCK * BLOCK_SIZE, SEEK_SET);
    read(disk_fd, entries, sizeof(entries));

    for (int i = 0; i < MAX_FILES; i++) {
        if (entries[i].used && strcmp(entries[i].name, name) == 0) {
            size_t to_read = (size > entries[i].size) ? entries[i].size : size;
            lseek(disk_fd, entries[i].start_block * BLOCK_SIZE, SEEK_SET);
            read(disk_fd, buffer, to_read);
            return to_read;
        }
    }

    fprintf(stderr, "Fichier non trouvé\n");
    return -1;
}

void list_files() {
    FileEntry entries[MAX_FILES];
    lseek(disk_fd, FILE_ENTRIES_BLOCK * BLOCK_SIZE, SEEK_SET);
    read(disk_fd, entries, sizeof(entries));

    printf("Fichiers dans le disque :\n");
    for (int i = 0; i < MAX_FILES; i++) {
        if (entries[i].used) {
            printf("- %s (%u octets)\n", entries[i].name, entries[i].size);
        }
    }
}

