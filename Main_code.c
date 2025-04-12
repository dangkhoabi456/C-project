#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define HOURLY_RATE 5000  // đơn giá gửi xe mỗi giờ

typedef struct {
    char license_plate[20];   // biển số xe
    int fee;                  // số tiền
    time_t entry_time;        // thời gian vào
    clock_t clock_start;// thời gian bắt đầu tính giờ riêng
    int floor; 
} vehicle;

#define MAX_SLOTS 100
vehicle vehicle_list[MAX_SLOTS];
int num_vehicles = 0;
double total=0; // biến toàn cục
// Đọc dữ liệu từ file
void read_from_file() {
    FILE *pt = fopen("parking_data.txt", "r");
    if (pt == NULL) return;

    vehicle temp;
    while (fscanf(pt, "%s %d %ld", temp.license_plate, &temp.fee, &temp.entry_time) == 3) {
        if (num_vehicles < MAX_SLOTS) {
            temp.clock_start = 0; // clock_start không được lưu trong file, đặt tạm là 0
            vehicle_list[num_vehicles++] = temp;
        }
    }
    fclose(pt);
}

// Kiểm tra còn chỗ không
int has_available_slot() {
    return num_vehicles < MAX_SLOTS;
}

void save_to_file(vehicle* new_vehicle){
    FILE *pt = fopen("parking_data.txt", "a");
    if (pt == NULL) {
        printf("Unable to open file to write!\n");
        return;
    }

    // Chuyển entry_time sang chuỗi ngày giờ
    char time_str[26];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&new_vehicle->entry_time));

    // Ghi biển số, phí, và thời gian vào (dạng dễ đọc)
    fprintf(pt, "%s %d %s\n", new_vehicle->license_plate, new_vehicle->fee, time_str);

    fclose(pt);
}

//hàm kiểm tra biển số
void Check_license_plate(char *a) {
    while (1) {
        int count = 0;

        // Kiểm tra cú pháp biển số
        if (strlen(a) == 10) {
            if (isdigit(a[0]) && isdigit(a[1])) count++;
            if (isalpha(a[2])) count++;
            if (a[3] == '-') count++;
            if (isdigit(a[4]) && isdigit(a[5]) && isdigit(a[6])) count++;
            if (a[7] == '.') count++;
            if (isdigit(a[8]) && isdigit(a[9])) count++;
        }

        if (count == 6) {
            return;
        } else {
            printf("Invalid license plate format! Please enter again (format: XXA-XXX.XX): ");
            scanf("%s", a);
        }
    }
}

void add_vehicle() {
    if (!has_available_slot()) {
        printf("The parking lot is full!\n");
        return;
    }

    vehicle new_vehicle;
    printf("Note: License plate format should be 2 digits + 1 letter + '-' + 3 digits + '.' + 2 digits.\n");
    printf("Enter license plate: ");
    scanf("%s", new_vehicle.license_plate);
    Check_license_plate(new_vehicle.license_plate);

    for (int i = 0; i < num_vehicles; i++){
        if (strcmp(new_vehicle.license_plate, vehicle_list[i].license_plate) == 0){
            printf("This license plate already exists!\n");
            return;
        }
    }

    new_vehicle.entry_time = time(NULL);
    new_vehicle.clock_start = clock(); // bắt đầu tính giờ riêng cho xe
    new_vehicle.fee = 0;
    vehicle_list[num_vehicles++] = new_vehicle;
    save_to_file(&new_vehicle);

    printf("Stopwatch started for this vehicle.\n");
    printf("Vehicle added successfully!\n");
}

void display_vehicle_list() {
    if (num_vehicles == 0) {
        printf("The parking lot is currently empty!\n");
        return;
    }

    printf("%-5s %-20s %-25s %-25s\n", "No.", "License Plate", "Entry Time","Fee");
    for (int i = 0; i < num_vehicles; i++) {
        char time_str[26];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&vehicle_list[i].entry_time));
        printf("%-5d %-20s %-25s %-25d\n", i + 1, vehicle_list[i].license_plate, time_str, vehicle_list[i].fee);
    }
}

vehicle* find_vehicle(const char *license_plate) {
    for (int i = 0; i < num_vehicles; i++) {
        if (strcmp(vehicle_list[i].license_plate, license_plate) == 0) {
            return &vehicle_list[i];
        }
    }
    return NULL;
}

void remove_vehicle(const char *license_plate) {
    vehicle *veh = find_vehicle(license_plate);
    if (veh == NULL) {
        printf("Khong tim thay xe!\n");
        return;
    }

    // Dừng tính giờ và tính thời gian đỗ xe
    clock_t clock_end = clock();
    double elapsed_seconds = (double)(clock_end - veh->clock_start) / CLOCKS_PER_SEC;
    int hours = (int)(elapsed_seconds / 3600);
    int minutes = ((int)(elapsed_seconds) % 3600) / 60;
    int seconds = (int)elapsed_seconds % 60;

    int total_hours = (elapsed_seconds + 3599) / 3600; // Làm tròn lên nếu còn dư phút
    veh->fee = total_hours * HOURLY_RATE;

    printf("Elapsed time for vehicle %s: %02d:%02d:%02d (hh:mm:ss)\n", license_plate, hours, minutes, seconds);
    printf("Total fee: %d VND\n", veh->fee);

    for (int i = 0; i < num_vehicles; i++) {
        if (strcmp(vehicle_list[i].license_plate, license_plate) == 0) {
            for (int j = i; j < num_vehicles - 1; j++)
                vehicle_list[j] = vehicle_list[j + 1];
            num_vehicles--;
            break;
        }
    }
    printf("Vehicle removed.\n");
}

void start_stopwatch() {
    printf("Press ENTER to start the stopwatch...\n");
    getchar();
    clock_t start = clock();
    printf("Stopwatch started! Press ENTER to stop...\n");
    getchar();
    clock_t end = clock();
    double elapsed_seconds = (double)(end - start) / CLOCKS_PER_SEC;
    int hours = (int)(elapsed_seconds / 3600);
    int minutes = ((int)(elapsed_seconds) % 3600) / 60;
    int seconds = (int)elapsed_seconds % 60;
    printf("Elapsed time: %02d:%02d:%02d (hh:mm:ss)\n", hours, minutes, seconds);
}

int main() {
    read_from_file();
    int choice;
    char license_plate[20];

    while (1) {
        printf("\n=== Quan ly bai do xe ===\n");
        printf("1. Them xe vao bai\n");
        printf("2. Xe roi bai\n");
        printf("3. Xem danh sach xe\n");
        printf("4. Dong ho dem gio doc lap\n");
        printf("5. Thoat\n");
        printf("Chon chuc nang: ");
        scanf("%d", &choice);
        getchar(); // Clear buffer sau scanf

        switch (choice) {
            case 1:
                add_vehicle();
                break;
            case 2:
                printf("Nhap bien so xe: ");
                scanf("%s", license_plate);
                remove_vehicle(license_plate);
                break;
            case 3:
                display_vehicle_list();
                break;
            case 4:
                start_stopwatch();
                break;
            case 5:
                return 0;
            default:
                printf("Lua chon khong hop le!\n");
        }
    }
}
