#define _CRT_SECURE_NO_WARNINGS
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
//Displays the Matrix
void display(int matrix[MAX_SIZE][MAX_SIZE], int row, int col){
	int r = 0;
	int c = 0;

	for (r = 0; r < row; r++)
	{
		for (c = 0; c < col; c++)
		{
			printf("%d ", matrix[r][c]);
		}
		printf("\n");
	}
}
//it gets the matrix's dimension and elements
void readMatrix(int matrix[MAX_SIZE][MAX_SIZE], int *rows, int *cols)
{
	int r = 0;
	int c = 0;
	int row = 0;
	int col = 0;
	int count = 1;
	
	printf("Enter the number of rows and columns: ");
	scanf("%d %d", &row, &col);
	
	
    printf("Please enter %d numbers in %d rows:\n", col, row);
	//matrix = (int **)malloc(sizeof(int*)* row);
	for (r = 0; r < row; r++)
	{
		printf("row %d: ",count);
		count++;
		//matrix[r] = (int*)malloc(sizeof(int)* col);
		for (c = 0; c < col; c++)
		{
			scanf("%d", &matrix[r][c]);	
		}
	}
	

	*rows = row;
	*cols = col;
}

void  parse(char *line, char **argv)
{
	while (*line != '\0') {       /* if not the end of line ....... */
		while (*line == ' ' || *line == '\t' || *line == '\n')
			*line++ = '\0';     /* replace white spaces with 0    */
		*argv++ = line;          /* save the argument position     */
		while (*line != '\0' && *line != ' ' &&
			*line != '\t' && *line != '\n')
			line++;             /* skip the argument until ...    */
	}
	*argv = '\0';                 /* mark the end of argument list  */
}

void  execute(char **argv)
{
	pid_t  pid;
	int	status;

	if ((pid = fork()) < 0) {     /* fork a child process           */
		printf("*** ERROR: forking child process failed\n");
		exit(1);
	}
	else if (pid == 0) {          /* for the child process:         */
		if (execvp(*argv, argv) < 0) {     /* execute the command  */
			printf("*** ERROR: exec failed\n");
			exit(1);
		}
	}
	else {                                  /* for the parent:      */
		while (wait(&status) != pid)       /* wait for completion  */
			;
	}
}

void multiply(struct Memory *shared_ptr)
{
	int r, c;
	char *command[64];
	char line[1024];

	for (r = 0; r < shared_ptr->rM; r++) {
		for (c = 0; c < shared_ptr->cM; c++) {
			sprintf(line, "%s %d %d", "./multiply", r, c);
			parse(line, command);
			// create n*m processes one for each element
			execute(command);

		}
	}
}

int main() {

	struct Memory *shm_ptr;

	int shm_id = 0;

	//Allocates a shared memory segment
	//returns a shared memory id
	shm_id = shmget(544, sizeof(struct Memory), IPC_CREAT | 0666);
	if (shm_id < 0)
		printf("shmget failed!\n");
	else {
		shm_ptr = (struct Memory*) shmat(shm_id, NULL, 0);
		if (!shm_ptr)
			printf("shmat failed!\n");
		else {
			// read matrix A from standard input
			printf("\n*************Matrix A*************\n");
			readMatrix(shm_ptr->matrixA, &shm_ptr->rA, &shm_ptr->cA);
			// read matrix B from standard input
			printf("\n*************Matrix B*************\n");
			readMatrix(shm_ptr->matrixB, &shm_ptr->rB, &shm_ptr->cB);
			shm_ptr->rM = shm_ptr->rA;
			shm_ptr->cM = shm_ptr->cB;

			// display input matrix A and B			
			printf("\nMatrix A:\n");
			display(shm_ptr->matrixA, shm_ptr->rA, shm_ptr->cA);
			printf("\nMatrix B:\n");
			display(shm_ptr->matrixB, shm_ptr->rB, shm_ptr->cB);
			multiply(shm_ptr);
			printf("\nResult:\n");
			display(shm_ptr->multiple, shm_ptr->rM, shm_ptr->cM);
		}
	}
	if (shmdt(shm_ptr) != 0)
		printf("shared memory detach failed\n");
}


