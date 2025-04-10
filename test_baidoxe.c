#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char license_plate[20];   // biển số xe
    int fee;                  // số tiền
    time_t entry_time;        // thời gian vào
} vehicle;

#define MAX_SLOTS 100
vehicle vehicle_list[MAX_SLOTS];
int num_vehicles = 0;

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
// Thêm xe mới
void add_vehicle() {
    if (!has_available_slot()) {
        printf("The parking lot is full!\n");
        return;
    }

    vehicle new_vehicle;
    printf("Enter license plate: ");
    scanf("%s", new_vehicle.license_plate);

    new_vehicle.entry_time = time(NULL);
    new_vehicle.fee = 0;
    vehicle_list[num_vehicles++] = new_vehicle;
    save_to_file(&new_vehicle);
    printf("Vehicle added successfully!\n");
}

// Hiển thị danh sách xe
void display_vehicle_list() {
    if (num_vehicles == 0) {
        printf("The parking lot is currently empty!\n");
        return;
    }

    printf("%-5s %-20s %-25s %-25s\n", "No.", "License Plate", "Entry Time","Fee");
    for (int i = 0; i < num_vehicles; i++) {
        char time_str[26];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&vehicle_list[i].entry_time));
        printf("%-5d %-20s %-25s %-25d\n", i + 1, vehicle_list[i].license_plate, time_str,vehicle_list[i].fee);
    }
}

// Main
int main() {
    read_from_file(); // Load data when program starts

    int choice;
    do {
        printf("\n=== VEHICLE PARKING MANAGEMENT SYSTEM ===\n");
        printf("1. Add new vehicle\n");
        printf("2. Show vehicle list\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_vehicle();
                break;
            case 2:
                display_vehicle_list();
                break;
            case 3:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 3);

    return 0;
}
