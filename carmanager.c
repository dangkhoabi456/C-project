#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char license_plate[20];
    time_t entry_time;
} Vehicle;

Vehicle vehicles[100];  
int num_vehicles = 0;    


//ham bo tro cho ham chinh
void save_to_file() {
    FILE *file = fopen("parking.txt", "w");
    if (!file) return;
    for (int i = 0; i < num_vehicles; i++)
        fprintf(file, "%s %ld\n", vehicles[i].license_plate, vehicles[i].entry_time);
    fclose(file);
} //luu thong tin ve xe(bien so, thoi diem vao tram) moi khi co thay doi

void load_from_file() {
    FILE *file = fopen("parking.txt", "r");
    if (!file) return;
    num_vehicles = 0;
    while (fscanf(file, "%s %ld", vehicles[num_vehicles].license_plate, &vehicles[num_vehicles].entry_time) != EOF)
        num_vehicles++;
    fclose(file);
}//Doc thong tin cac xe da duoc luu truoc do trong file parking.txt va nap vao chuong trinh 

Vehicle* find_vehicle(const char *license_plate) {
    for (int i = 0; i < num_vehicles; i++)
        if (strcmp(vehicles[i].license_plate, license_plate) == 0)
            return &vehicles[i];
    return NULL;
}// tim kiem xe dua tren bien so

//ham chinh
void add_vehicle(const char *license_plate) {
    if (find_vehicle(license_plate) != NULL) {
        printf("Xe da co trong bai!\n");
        return;
    }
    strcpy(vehicles[num_vehicles].license_plate, license_plate);
    vehicles[num_vehicles].entry_time = time(NULL);
    num_vehicles++;
    save_to_file();
    printf("Them xe %s thanh cong.\n", license_plate);
}//them xe da vao bai do

void remove_vehicle(const char *license_plate) {
    Vehicle *vehicle = find_vehicle(license_plate);
    if (vehicle == NULL) {
        printf("Khong tim thay xe!\n");
        return;
    }

    for (int i = 0; i < num_vehicles; i++) {
        if (strcmp(vehicles[i].license_plate, license_plate) == 0) {
            for (int j = i; j < num_vehicles - 1; j++)
                vehicles[j] = vehicles[j + 1];
            num_vehicles--;
            break;
        }
    }
    save_to_file();
}//xe roi bai 

void vehicle_list() {
    printf("\nDanh sach xe trong bai:\n");
    for (int i = 0; i < num_vehicles; i++)
        printf("%d. Bien so: %s - Thoi gian vao: %ld\n", i + 1, vehicles[i].license_plate, vehicles[i].entry_time);
}//hien thi toan bo danh sach xe trong bai

int main() {
    load_from_file();
    int choice;
    char license_plate[20];

    while (1) {
        printf("\n=== Quan ly bai do xe ===\n");
        printf("1. Them xe vao bai\n");
        printf("2. Xe roi bai\n");
        printf("3. Xem danh sach xe\n");
        printf("4. Thoat\n");
        printf("Chon chuc nang: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Nhap bien so xe: ");
                scanf("%s", license_plate);
                add_vehicle(license_plate);
                break;
            case 2:
                printf("Nhap bien so xe: ");
                scanf("%s", license_plate);
                remove_vehicle(license_plate);
                break;
            case 3:
                vehicle_list();
                break;
            case 4:
                return 0;
            default:
                printf("Lua chon khong hop le!\n");
        }
    }
}
