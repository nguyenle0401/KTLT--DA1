#ifndef CSV_READER_H
#define CSV_READER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sinhvien.h"

// Hàm loại bỏ dấu nháy kép và khoảng trắng thừa
void trimQuotes(char *str)
{
    int len = strlen(str);
    int start = 0, end = len - 1;

    // Bỏ khoảng trắng đầu
    while (str[start] == ' ' || str[start] == '\t')
        start++;

    // Bỏ khoảng trắng cuối
    while (end > start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n' || str[end] == '\r'))
    {
        str[end] = '\0';
        end--;
    }

    // Bỏ dấu nháy kép
    if (str[start] == '"' && str[end] == '"')
    {
        start++;
        str[end] = '\0';
    }

    // Dịch chuỗi về đầu
    if (start > 0)
    {
        memmove(str, str + start, strlen(str + start) + 1);
    }
}

// Hàm tách một dòng CSV thành các trường
int parseCSVLine(char *line, char fields[][MAX_STRING], int maxFields)
{
    int fieldCount = 0;
    int i = 0;
    int fieldIndex = 0;
    int inQuotes = 0;
    int len = strlen(line);

    fields[fieldCount][0] = '\0';

    for (i = 0; i < len && fieldCount < maxFields; i++)
    {
        char c = line[i];

        // Xử lý dấu nháy kép
        if (c == '"')
        {
            inQuotes = !inQuotes;
            continue;
        }

        // Xử lý dấu phẩy (ngăn cách trường)
        if (c == ',' && !inQuotes)
        {
            fields[fieldCount][fieldIndex] = '\0';
            trimQuotes(fields[fieldCount]);
            fieldCount++;
            fieldIndex = 0;
            fields[fieldCount][0] = '\0';
        }
        // Ký tự bình thường
        else if (c != '\r' && c != '\n')
        {
            fields[fieldCount][fieldIndex++] = c;
        }
    }

    // Xử lý trường cuối cùng
    if (fieldIndex > 0 || fieldCount > 0)
    {
        fields[fieldCount][fieldIndex] = '\0';
        trimQuotes(fields[fieldCount]);
        fieldCount++;
    }

    return fieldCount;
}

// Hàm đọc file CSV
int readCSV(const char *filename, SinhVien danhSach[], int maxSinhVien)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Khong mo duoc file: %s\n", filename);
        return 0;
    }

    char line[4096];
    int count = 0;

    // Bỏ qua dòng header
    fgets(line, sizeof(line), file);

    // Đọc từng dòng
    while (fgets(line, sizeof(line), file) && count < maxSinhVien)
    {
        // Bỏ qua dòng trống
        if (strlen(line) <= 1)
            continue;

        char fields[20][MAX_STRING];
        int fieldCount = parseCSVLine(line, fields, 20);

        // Kiểm tra đủ số trường tối thiểu
        if (fieldCount < 7)
        {
            printf("Dong khong hop le (thieu truong): %s\n", line);
            continue;
        }

        // Gán dữ liệu vào struct
        SinhVien *sv = &danhSach[count];
        strcpy(sv->mssv, fields[0]);
        strcpy(sv->hoTen, fields[1]);
        strcpy(sv->khoa, fields[2]);
        sv->khoaHoc = atoi(fields[3]);
        strcpy(sv->ngaySinh, fields[4]);
        strcpy(sv->hinhAnh, fields[5]);
        strcpy(sv->moTa, fields[6]);

        // Lấy các sở thích (từ cột 7 trở đi)
        sv->soLuongSoThich = 0;
        for (int i = 7; i < fieldCount && sv->soLuongSoThich < MAX_SOTHICH; i++)
        {
            if (strlen(fields[i]) > 0)
            {
                strcpy(sv->soThich[sv->soLuongSoThich], fields[i]);
                sv->soLuongSoThich++;
            }
        }

        count++;
    }

    fclose(file);
    printf("Doc thanh cong %d sinh vien!\n", count);

    return count;
}

#endif