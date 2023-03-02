#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
	pid_t pid;
	int fd, n, m;
	int data[] = {1, 2, 3 ,4 ,5};
	int asize = sizeof(data) / sizeof(int);

	pid = fork();

	// Código del hijo
	if( pid == 0)
	{
	   printf("Soy el hijo y mi pid = %d\n", pid);
	   
	   fd = creat("datos.txt", 777);
    
           for(int i = 0; i < asize; i++)
           {
    	      n = write(fd, &data[i], sizeof(float));
	   }
	}

	// Código del padre
	sleep(3);
	if( pid > 0)
	{
	   printf("Soy el padre y mi pid = %d\n", pid);
	   
	   fd = open("datos.txt", O_RDONLY);

	   m = read(fd, data, sizeof(data));

	   printf("El arreglo del archivo es:\n");
           for (int i = 0; i < asize; i++) 
	   {
              printf("%d\n", data[i]);
           }
	}

	return 0;
}

