#include <stdio.h>
#include <stdlib.h>
#include "sinhvien.h"
#include "reader_csv.h"
#include "gen_html.h"

#define MAX_SINH_VIEN 1000
#define TEMPLATE_SIZE 50000
#define HTML_SIZE 50000

int main()
{
    // CẤP PHÁT ĐỘNG thay vì khai báo trên stack
    SinhVien *danhSach = (SinhVien *)malloc(MAX_SINH_VIEN * sizeof(SinhVien));
    char *templateHTML = (char *)malloc(TEMPLATE_SIZE * sizeof(char));
    char *html = (char *)malloc(HTML_SIZE * sizeof(char));

    // Kiểm tra cấp phát thành công
    if (!danhSach || !templateHTML || !html)
    {
        printf("Loi: Khong du bo nho!\n");
        if (danhSach)
            free(danhSach);
        if (templateHTML)
            free(templateHTML);
        if (html)
            free(html);
        return 1;
    }

    int soLuongSV;

    printf("=== CHUONG TRINH TAO PROFILE HTML TU CSV ===\n\n");

    // Bước 1: Đọc template
    printf("Dang doc template...\n");
    if (readFile("template.html", templateHTML, TEMPLATE_SIZE) == 0)
    {
        printf("Loi: Khong tim thay file template.html!\n");
        free(danhSach);
        free(templateHTML);
        free(html);
        return 1;
    }

    // Bước 2: Đọc file CSV
    printf("Dang doc file CSV...\n");
    soLuongSV = readCSV("data/students.csv", danhSach, MAX_SINH_VIEN);

    if (soLuongSV == 0)
    {
        printf("Loi: Khong co du lieu sinh vien!\n");
        free(danhSach);
        free(templateHTML);
        free(html);
        return 1;
    }

    // Bước 3: Tạo thư mục profiles
    printf("Dang tao thu muc profiles...\n");
    createDirectory("profiles");

    // Bước 4: Tạo HTML cho mỗi sinh viên
    printf("Dang tao cac trang profile...\n\n");
    for (int i = 0; i < soLuongSV; i++)
    {
        SinhVien *sv = &danhSach[i];

        // Tạo HTML
        generateHTML(sv, templateHTML, html);

        // Tạo tên file
        char filename[256];
        sprintf(filename, "profiles/%s.html", sv->mssv);

        // Ghi file
        writeFile(filename, html);

        printf("  [%d/%d] Tao thanh cong: %s\n", i + 1, soLuongSV, filename);
    }

    printf("\n=== HOAN THANH ===\n");
    printf("Da tao %d trang profile trong thu muc 'profiles/'\n", soLuongSV);

    // Giải phóng bộ nhớ
    free(danhSach);
    free(templateHTML);
    free(html);

    return 0;
}