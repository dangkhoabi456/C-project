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

//hàm kiểm tra biển số
void Check_license_plate(char *a) {
    while (1) {
        int count = 0;

        // Kiểm tra cú pháp biển số
        if (strlen(a) == 10) { // Kiểm tra độ dài
            if (isdigit(a[0]) && isdigit(a[1])) count++;  // 2 chữ số đầu
            if (isalpha(a[2])) count++;  // 1 chữ cái
            if (a[3] == '-') count++;  // dấu '-'
            if (isdigit(a[4]) && isdigit(a[5]) && isdigit(a[6])) count++;  // 3 chữ số sau dấu '-'
            if (a[7] == '.') count++;  // dấu '.'
            if (isdigit(a[8]) && isdigit(a[9])) count++;  // 2 chữ số sau dấu '.'
        }

        // Nếu tất cả các điều kiện đúng
        if (count == 6) {
            return;  // Biển số hợp lệ
        } else {
            printf("Invalid license plate format! Please enter again (format: XXA-XXX.XX): ");
            scanf("%s", a);  // Yêu cầu nhập lại biển số
        }
    }
}
// Thêm xe mới
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
	//check xem biển có tồn tại chưa
    for (int i=0;i<num_vehicles;i++){
    		if (strcmp(new_vehicle.license_plate,vehicle_list[i].license_plate)==0){
    			printf("This license plate already exists!\n");
    			return;
			}
	}
    new_vehicle.entry_time = time(NULL);
    new_vehicle.fee = 0;
    vehicle_list[num_vehicles++] = new_vehicle;
    save_to_file(&new_vehicle);
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
        printf("%-5d %-20s %-25s %-25d\n", i + 1, vehicle_list[i].license_plate, time_str,vehicle_list[i].fee);
    }
}

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
