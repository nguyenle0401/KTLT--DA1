#ifndef SINHVIEN_H
#define SINHVIEN_H

#define MAX_STRING 1000
#define MAX_SOTHICH 10

typedef struct
{
    char mssv[11];  // Tối đa 10 ký tự + null
    char hoTen[31]; // Tối đa 30 ký tự + null
    char khoa[31];
    int khoaHoc;
    char ngaySinh[11]; // dd/mm/yyyy
    char hinhAnh[MAX_STRING];
    char moTa[1001];                       // Tối đa 1000 ký tự + null
    char soThich[MAX_SOTHICH][MAX_STRING]; // Tối đa 10 sở thích
    int soLuongSoThich;                    // Đếm số sở thích
} SinhVien;

#endif