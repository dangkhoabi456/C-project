#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define HOURLY_RATE 5000  // đơn giá gửi xe mỗi giờ

typedef struct {
    char license_plate[20];
    int fee;
    time_t entry_time;
    int floor; 
} vehicle;

#define MAX_SLOTS 100
vehicle vehicle_list[MAX_SLOTS];
int num_vehicles = 0;
double doanh_thu = 0;  // Thay vì dùng total

void Cal_total(double fee);
void save_doanh_thu();
void load_doanh_thu();



void read_from_file() {
    FILE *pt = fopen("parking_data.txt", "r");
    if (pt == NULL) return;

    vehicle temp;
    char time_str[26];

    while (fscanf(pt, "%s %d %25[^\n] %d", temp.license_plate, &temp.fee, time_str, &temp.floor) == 4) {
        struct tm tm_time;
        if (sscanf(time_str, "%4d-%2d-%2d %2d:%2d:%2d", 
                   &tm_time.tm_year, &tm_time.tm_mon, &tm_time.tm_mday,
                   &tm_time.tm_hour, &tm_time.tm_min, &tm_time.tm_sec) == 6) {
            tm_time.tm_year -= 1900;
            tm_time.tm_mon -= 1;
            temp.entry_time = mktime(&tm_time);
            if (num_vehicles < MAX_SLOTS) {
                vehicle_list[num_vehicles++] = temp;
            }
        }
    }
    fclose(pt);
}

int has_available_slot() {
    return num_vehicles < MAX_SLOTS;
}

void save_to_file(vehicle* new_vehicle) {
    FILE *pt = fopen("parking_data.txt", "a");
    if (pt == NULL) {
        printf("Unable to open file to write!\n");
        return;
    }

    char time_str[26];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&new_vehicle->entry_time));
    fprintf(pt, "%s %d %s %d\n", new_vehicle->license_plate, new_vehicle->fee, time_str, new_vehicle->floor);
    fclose(pt);
}

void Check_license_plate(char *a) {
    while (1) {
        int count = 0;
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

void log_action(const char *license_plate, const char *action) {
    FILE *log = fopen("log.txt", "a");
    if (!log) {
        printf("Không thể mở log.txt để ghi!\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *time_info = localtime(&now);
    char time_str[30];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);
    fprintf(log, "%s %s %s\n", license_plate, action, time_str);
    fclose(log);
}

void add_vehicle() {
    if (!has_available_slot()) {
        printf("The parking lot is full!\n");
        return;
    }

    vehicle new_vehicle;
    printf("Note: License plate format should be 2 digits + 1 letter + '-' + 3 digits + '.' + 2 digits.\n");
    printf("Enter license plate: ");
    fgets(new_vehicle.license_plate, sizeof(new_vehicle.license_plate), stdin);
    new_vehicle.license_plate[strcspn(new_vehicle.license_plate, "\n")] = '\0';
    Check_license_plate(new_vehicle.license_plate);

    printf("Enter floor number (1/2): ");
    scanf("%d", &new_vehicle.floor);
    getchar();

    for (int i = 0; i < num_vehicles; i++) {
        if (strcmp(new_vehicle.license_plate, vehicle_list[i].license_plate) == 0) {
            printf("This license plate already exists!\n");
            return;
        }
    }

    new_vehicle.entry_time = time(NULL);
    new_vehicle.fee = 0;
    vehicle_list[num_vehicles++] = new_vehicle;
    save_to_file(&new_vehicle);

    printf("Vehicle added successfully!\n");
    log_action(new_vehicle.license_plate, "in");
}

void display_vehicle_list() {
    if (num_vehicles == 0) {
        printf("The parking lot is currently empty!\n");
        return;
    }

    printf("%-5s %-20s %-25s %-10s %-6s\n", "No.", "License Plate", "Entry Time", "Fee", "Floor");
    for (int i = 0; i < num_vehicles; i++) {
        char time_str[26];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&vehicle_list[i].entry_time));
        printf("%-5d %-20s %-25s %-10d %-6d\n", 
               i + 1, 
               vehicle_list[i].license_plate, 
               time_str, 
               vehicle_list[i].fee, 
               vehicle_list[i].floor);
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

    time_t current_time = time(NULL);
    double elapsed_seconds = difftime(current_time, veh->entry_time);
    int hours = (int)(elapsed_seconds / 3600);
    int minutes = ((int)(elapsed_seconds) % 3600) / 60;
    int seconds = (int)elapsed_seconds % 60;

    int total_hours = (elapsed_seconds + 3599) / 3600;
    veh->fee = total_hours * HOURLY_RATE;

    printf("Elapsed time for vehicle %s: %02d:%02d:%02d (hh:mm:ss)\n", license_plate, hours, minutes, seconds);
    printf("Total fee: %d VND\n", veh->fee);
    Cal_total(veh->fee);
    log_action(license_plate, "out");

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
void save_doanh_thu() {
    FILE *f = fopen("doanh_thu.txt", "w");
    if (f) {
        fprintf(f, "%.0f", doanh_thu);
        fclose(f);
    }
}

void load_doanh_thu() {
    FILE *f = fopen("doanh_thu.txt", "r");
    if (f) {
        fscanf(f, "%lf", &doanh_thu);
        fclose(f);
    }
}
void Cal_total(double fee) {
    doanh_thu += fee;
    save_doanh_thu();
}

void show_total() {
    printf("\nTong tien da thu tu cac xe roi bai: %.0f VND\n", doanh_thu);
}

void vehicle_sum() {
    FILE *pt = fopen("log.txt", "r");
    if (pt == NULL) {
        printf("Không thể mở file log.txt!\n");
        return;
    }

    int vehicle_sum_in = 0;
    int vehicle_sum_out = 0;
    char license[20], action[10];
    char time_str[30];

    while (fscanf(pt, "%s %s %[^\n]", license, action, time_str) == 3) {
        if (strcmp(action, "in") == 0) {
            vehicle_sum_in++;
        } else if (strcmp(action, "out") == 0) {
            vehicle_sum_out++;
        }
    }

    fclose(pt);
    printf("Tong so xe vao: %d\n", vehicle_sum_in);
    printf("Tong so xe ra : %d\n", vehicle_sum_out);
}

int main() {
    load_doanh_thu(); // mới thêm
    read_from_file();
    int choice;
    char license_plate[20];

    while (1) {
        printf("\n=== Quan ly bai do xe ===\n");
        printf("1. Them xe vao bai\n");
        printf("2. Xe roi bai\n");
        printf("3. Xem danh sach xe\n");
        printf("4. Tong doanh thu\n");
        printf("5. Tong so xe ra/vao\n");
        printf("6. Thoat\n");
        printf("Chon chuc nang: ");
        scanf("%d", &choice);
        getchar();

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
                show_total();
                break;
            case 5:
                vehicle_sum();
                break;
            case 6:
                return 0;
            default:
                printf("Lua chon khong hop le!\n");
        }
    }
}
