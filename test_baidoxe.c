#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
//Tạo ra một list thông tin cơ bản cần có của một chiếc xe
typedef struct{
    char bien_so[20];
    char kieu_xe[20];
    char hang_xe[20];
    int so_tien;
    time_t intime;
    }Xe;
// Khởi tạo số lượng xe ở bãi
#define Max_slot 100
Xe slot_de_xe[Max_slot];
int dem = 0;
//Tạo hàm đọc file
void DocDuLieuTuFile() {
    FILE *f = fopen("parking_data.txt", "r");
    if (f == NULL) return;

    Xe temp;
    while (fscanf(f, "%s %s %s %d %ld", temp.bien_so, temp.kieu_xe, temp.hang_xe, &temp.so_tien, &temp.intime) == 5) {
        if (dem < Max_slot) {
            slot_de_xe[dem++] = temp;
        }
    }
    fclose(f);
}
// Khởi tạo hàm kiểm tra xem còn chỗ gửi xe hay không
int KiemTraChoTrong (){
    return (dem < Max_slot);
}
//Khởi tạo hàm thêm xe mới vào bãi
void ThemXeMoi (){
    if (!KiemTraChoTrong()){
         printf ("Bai xe da het cho!\n");
         return;// thoát ra khi bãi xe đầy!
     }
    Xe xe_moi;// thêm biến phụ xe mới
    printf ("Nhap vao bien so xe: \n");
    scanf ("%s",xe_moi.bien_so);
    printf ("Nhap vao kieu xe (oto/xe may): \n");
    scanf ("%s",xe_moi.kieu_xe);
    printf ("Nhap vao hang xe: \n");
    scanf ("%s",xe_moi.hang_xe);
    if (islower(xe_moi.hang_xe[0])) {
    xe_moi.hang_xe[0] = toupper(xe_moi.hang_xe[0]);
    }
    xe_moi.intime =  time(NULL); // lưu thời gian vào bãi
    xe_moi.so_tien=0; //Lúc này chưa tính tiền
    FILE *f = fopen("parking_data.txt", "a");
    if (f == NULL) {
        printf("Khong the mo file de ghi!\n");
        return;
    }

    // Ghi thông tin xe vào file (theo đúng thứ tự)
    fprintf(f, "%s %s %s %d %ld\n",
            xe_moi.bien_so,
            xe_moi.kieu_xe,
            xe_moi.hang_xe,
            xe_moi.so_tien,
            xe_moi.intime);

    fclose(f);

    slot_de_xe[dem++] = xe_moi;

    printf("Da them xe thanh cong!\n");
    
}
void HienThiXeTrongBai(){
    if (dem==0){
        printf ("Bai xe hien dang trong!\n");
        return;
    }
    printf ("%-10d %-20s %-20s %-20s %-25s\n","STT","Bien so","Kieu xe","Hang xe","Thoi gian vao");
    for (int i =0;i<dem;i++){
        char timeStr[26]; // Chuỗi lưu để lưu thời gian
        strftime(timeStr/*chuỗi lưu*/ , sizeof(timeStr), "%Y-%m-%d %H:%M:%S"/*Hiện thị giờ và ngày tháng*/,
         localtime(&slot_de_xe[i].intime));
         // dùng localtime để chuyển thành dữ liệu giờ mà mình có thể đọc được
         //thời gian xe hiện ra mỗi xe thông qua vòng lặp i
         printf ("%-10d %-20s %-20s %-20s %-25s\n",i,slot_de_xe[i].bien_so,slot_de_xe[i].kieu_xe,slot_de_xe[i].hang_xe, timeStr);
    }
}
int main (){
	DocDuLieuTuFile(); // Đọc file khi bắt đầu chương trình
    int choice;
    do{
        printf ("CHAO MUNG TOI HE THONG GUI & GIU XE! \n");
        printf ("Hay chon lua chon cua ban: \n");
        printf ("1.Them xe moi\n");
        printf ("2.Hien thi danh sach xe\n");
        printf ("3.Thoat!\n");
        printf ("Nhap vao lua chon cua ban: \n");
        scanf ("%d",&choice);
        
        switch (choice){
            case 1:
                ThemXeMoi();
                break;
            case 2:
                HienThiXeTrongBai();
                break;
            case 3:
                printf ("Tam biet!\n");
                break;
            default:
                printf("Lựa chọn không hợp lệ!\n");
        }
    } while (choice != 3);

    return 0;
}
    

