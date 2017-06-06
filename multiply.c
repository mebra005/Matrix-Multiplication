#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

#define MAX_SIZE 256

struct Memory
{
	int rA, rB, rM; /*Rows     */
	int cA, cB, cM; /* Columns */
	
	int matrixA[MAX_SIZE][MAX_SIZE];  /* Two-dimensional Arrays  A     */
	int matrixB[MAX_SIZE][MAX_SIZE];  /* Two-dimensional Arrays  B     */
	int multiple[MAX_SIZE][MAX_SIZE]; /* Two-dimensional Arrays  Result*/
};

int main(int argc, char *argv[]) {

	int k = 0;
	int sum = 0;
	int i = 0;
	int j = 0;

	int shm_id;
	struct Memory *shm_ptr;

	if(argc != 3)
	{
		printf("usage: %s i j\n",argv[0]);
		
	}
	else	
	{
		//Allocates a shared memory segment
		//returns a shared memory id
		shm_id = shmget(544, sizeof(struct Memory), IPC_CREAT | 0666);

		if (shm_id < 0)
			printf("shmget failed!\n");
		else {
			//attach shared memory
			shm_ptr = (struct Memory*) shmat(shm_id, NULL, 0);
			if (!shm_ptr)
				printf("shmat failed!\n");
			else {
				// read command line arguments
				i = atoi(argv[1]);
				j = atoi(argv[2]);
				
				
				printf("process %d received %d, %d\n", getpid(), i, j);
				// i and j should be less than number of rows of matrix A and number of columns of matrix B
				if (i < shm_ptr->rA && j < shm_ptr->cB)
				{
					// caclulate sum for position i j
					for (k = 0; k < shm_ptr->cA; k++) {
						sum = sum + shm_ptr->matrixA[i][k] * shm_ptr->matrixB[k][j];
					}

					shm_ptr->multiple[i][j] = sum;
		
					printf("process %d computed %d\n", getpid(), sum);
				}
			}
		}
		// close shared memory
		if (shmdt(shm_ptr) != 0)
			printf("shared memory detach failed\n");
	}
	return 0;
}


