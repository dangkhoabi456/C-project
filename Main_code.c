#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Đọc dữ liệu từ file
void read_from_file() {
    FILE *pt = fopen("parking_data.txt", "r");
    if (pt == NULL) return;

    vehicle temp;
    while (fscanf(pt, "%s %d %ld", temp.license_plate, &temp.fee, &temp.entry_time) == 3) {
        if (num_vehicles < MAX_SLOTS) {
            vehicle_list[num_vehicles++] = temp;
        }
    }
    fclose(pt);
}

// Kiểm tra còn chỗ không
int has_available_slot() {
    return num_vehicles < MAX_SLOTS;
}
void save_to_file(vehicle*new_vehicle){
	FILE *pt = fopen("parking_data.txt", "a");
    if (pt == NULL) {
        printf("Unable to open file to write!\n");
        return;
    }

    fprintf(pt, "%s %d %ld\n", new_vehicle->license_plate, new_vehicle->fee, new_vehicle->entry_time);
    fclose(pt);
}
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
