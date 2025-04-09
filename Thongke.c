#include <stdio.h>
#include <stdlib.h>
#include <math.h>
double total=0; // biến toàn cục
char vehicle_list[100];
void vehicle_sum(){
	FILE *pt = fopen("parking.txt", "r");
    if (pt == NULL) {
        printf("Cannot open file!\n");
        return;
    }
	int vehicle_sum_in = 0;
	int vehicle_sum_out = 0;
	while (fgets(vehicle_list, sizeof(vehicle_list),pt)){
		//kiem tra xe vao
		if (strstr(vehicle_list, "in")!= NULL){
			vehicle_sum_in++;
		}
		//kiem tra xe ra
		else if (strstr(vehicle_list,"out")!= NULL){
			vehicle_sum_out++;
		}
	}
	fclose(pt);
	printf("Tổng số xe vào: %d\n",vehicle_sum_in);
	printf("Tổng số xe ra : %d\n",vehicle_sum_out);
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
