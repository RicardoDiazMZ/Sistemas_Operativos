#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

//Este programa obtiene el área de un rectángulo cuya base y altura es dada por el usuario
int main(int argc, char *argv[]) 
{
	float base, height, area;
	
	printf("input base ");
	scanf("%f", &base);
	printf("Base of the rectangle = %0.3f\n", base);
	
	printf("input height ");
	scanf("%f", &height);
	printf("height of the rectangle = %0.3f\n", height);
	
	area = base * height;
	printf("Area of the rectangle = %0.3f", area);
	return 0;
}
