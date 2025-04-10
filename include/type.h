#ifndef FS_TYPE
#define FS_TYPE

#include <stdint.h>
#include <time.h>

#define MAX_NAME_LEN 32

typedef struct {
    uint32_t magic;
    uint32_t total_blocks;
    uint32_t free_blocks;
    uint32_t root_dir_block;
} SuperBlock;

typedef struct {
    SuperBlock sb;      // Superbloc
    char *disk_image;   // Pointeur vers l'image disque (fichier binaire)
} FileSystem;

typedef struct {
    char name[MAX_NAME_LEN];     // Nom du fichier
    uint32_t size;               // Taille en octets
    uint32_t start_block;        // Position dans le disque
    uint8_t used;                // 1 = utilisé, 0 = libre
    time_t created_at;           // Date de création (timestamp UNIX)
    time_t modified_at;          // Date de dernière modification
    uint16_t permissions;        // Permissions style UNIX
    char owner[16];              // Propriétaire du fichier
} FileEntry;

#endif // FS_TYPE

