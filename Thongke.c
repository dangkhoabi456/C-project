#include <stdio.h>
#include <stdlib.h>
#include <math.h>
double total=0; // biến toàn cục
char vehicle_list[100];
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

void Cal_total(double fee){
	total += fee;
}//gọi hàm Cal_total(fee) ở trong hàm void remove_vehicle(const char *license_plate)
void show_total() {
    printf("\nTong tien da thu tu cac xe roi bai: %.2f VND\n", total);
}
int main(){
	double fee;
	show_total();
	vehicle_sum();
}
