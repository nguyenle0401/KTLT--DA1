#ifndef HTML_GENERATOR_H
#define HTML_GENERATOR_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/types.h>
#endif
#include "sinhvien.h"

// Hàm đọc toàn bộ file
int readFile(const char *filename, char *buffer, int maxSize)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Khong mo duoc file: %s\n", filename);
        return 0;
    }

    int totalRead = 0;
    int bytesRead;

    while ((bytesRead = fread(buffer + totalRead, 1, maxSize - totalRead - 1, file)) > 0)
    {
        totalRead += bytesRead;
    }

    buffer[totalRead] = '\0';
    fclose(file);

    return totalRead;
}

// Hàm thay thế placeholder
void replacePlaceholder(char *text, const char *placeholder, const char *value)
{
    char search[100];
    sprintf(search, "{{%s}}", placeholder);

    char *pos;
    char temp[50000];
    int searchLen = strlen(search);
    int valueLen = strlen(value);

    while ((pos = strstr(text, search)) != NULL)
    {
        // Copy phần trước placeholder
        int beforeLen = pos - text;
        strncpy(temp, text, beforeLen);
        temp[beforeLen] = '\0';

        // Thêm value
        strcat(temp, value);

        // Thêm phần sau placeholder
        strcat(temp, pos + searchLen);

        // Copy lại vào text
        strcpy(text, temp);
    }
}

// Hàm tạo danh sách sở thích HTML
void generateHobbiesList(const SinhVien *sv, char *output)
{
    output[0] = '\0';

    if (sv->soLuongSoThich == 0)
    {
        strcpy(output, "<li>Chua cap nhat</li>");
        return;
    }

    for (int i = 0; i < sv->soLuongSoThich; i++)
    {
        strcat(output, "<li>");
        strcat(output, sv->soThich[i]);
        strcat(output, "</li>\n        ");
    }
}

// Hàm sinh HTML cho một sinh viên
void generateHTML(const SinhVien *sv, const char *templateHTML, char *output)
{
    char khoaHocStr[20];
    char hobbiesList[5000];
    char imagePathFixed[MAX_STRING];

    // Copy template
    strcpy(output, templateHTML);

    // Chuyển số thành chuỗi
    sprintf(khoaHocStr, "%d", sv->khoaHoc);

    // Tạo danh sách sở thích
    generateHobbiesList(sv, hobbiesList);
    // Sửa đường dẫn ảnh
    if (strncmp(sv->hinhAnh, "Images/", 7) == 0)
    {
        sprintf(imagePathFixed, "../%s", sv->hinhAnh);
    }
    else if (strncmp(sv->hinhAnh, "http", 4) == 0)
    {
        strcpy(imagePathFixed, sv->hinhAnh);
    }
    else
    {
        sprintf(imagePathFixed, "../%s", sv->hinhAnh);
    }
    // Thay thế các placeholder
    replacePlaceholder(output, "MSSV", sv->mssv);
    replacePlaceholder(output, "HO_TEN", sv->hoTen);
    replacePlaceholder(output, "KHOA", sv->khoa);
    replacePlaceholder(output, "KHOA_HOC", khoaHocStr);
    replacePlaceholder(output, "NGAY_SINH", sv->ngaySinh);
    // replacePlaceholder(output, "HINH_ANH", sv->hinhAnh);
    replacePlaceholder(output, "HINH_ANH", imagePathFixed); // Dùng đường dẫn đã sửa

    replacePlaceholder(output, "MO_TA", sv->moTa);
    replacePlaceholder(output, "SO_THICH_LIST", hobbiesList);
}

// Hàm ghi file
void writeFile(const char *filename, const char *content)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Khong tao duoc file: %s\n", filename);
        return;
    }

    fprintf(file, "%s", content);
    fclose(file);
}

// Hàm tạo thư mục
void createDirectory(const char *path)
{
#ifdef _WIN32
    _mkdir(path);
#else
    mkdir(path, 0777);
#endif
}

#endif