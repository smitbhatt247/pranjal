#include <stdio.h>
#include "2048.h"
#include "zombie.h"

int main(void) {
	puts("Enter 1 for 2048, 2 for zombie");
	int choice;
	scanf("%i", &choice);
	switch (choice) {
		case 1:
			main_2048();
			break;
		case 2:
			main_zombie();
			break;
		default:
			puts("Invalid Input");
	}
}
