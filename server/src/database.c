#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

#define DATABASE_FOLDER "database/"  // Đường dẫn đến thư mục lưu file

void create_text_file(const char *filename) {
    // Tạo đường dẫn đầy đủ cho file trong thư mục "database"
    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%s%s", DATABASE_FOLDER, filename);

    // Tạo một file text rỗng
    FILE *file = fopen(full_path, "w");
    if (!file) {
        perror("Failed to create text file");
        printf("Filename attempted: %s\n", full_path);  // Log the attempted filename
        exit(1);
    }
    fclose(file);
}

void write_to_text_file(const char *filename, const char *data) {
    // Tạo đường dẫn đầy đủ cho file trong thư mục "database"
    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%s%s", DATABASE_FOLDER, filename);

    // Ghi dữ liệu vào file text
    FILE *file = fopen(full_path, "a");
    if (!file) {
        perror("Failed to open text file");
        printf("Filename attempted: %s\n", full_path);  // Log the attempted filename
        return;
    }
    fprintf(file, "%s\n", data);
    fclose(file);
}
