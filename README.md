# Matrix-Multiplication
These programs (source and multiply) that together multiply an nxp matrix by a pxm matrix 
and display the result. 

 

1. **The program source:**

	a. reads two matrices A (nxp) and B (pxm) into a shared memory 

	b. displays the input matrices A and B (properly formatted matrices) 

	c. creates n*m processes, one for each element of the product matrix 			AxB. Each process is created using execvp and runs the multiply program 		described in paragraph 2 below. 

	d. waits for all of the processes created in step c to complete. After 			each wait displays the process number of the process that terminated 			(i.e. process nnnn terminated). After completion of all processes 				displays the result (properly formatted product matrix, AxB) 

 

2. **The multiply program** 

  a. receives a row number (i) and column number (j) form its command line 
  argument (i.e. in argv[1] and argv[2]) 

  b. displays its process id and the values of i and j (i.e. process nnnn received i, j) 

  c. computes elementi,j of the product matrix and puts the result in the product 
  matrix 

  d. displays its process id and the value it computed in step c (i.e. process nnnn computed y) 

  e. exits with code 0 

 
 
**The input to the program is in the following form**
		
        Enter the number of rows and columns: n p

		Please enter p numbers in n rows:

		row 1 

		row 2 

		… 

		row n



        Enter the number of rows and columns: m p 

        Please enter p numbers in m rows:

        row 1 

        row 2 

        … 

        row p 


and is read from the standard input., For example 

        3 4 

        1 2 3 4 

        8 1 12 15 

        1 9 10 11 



        4 3 

        1 2 3 

        1 1 1 

        2 2 2 

        3 2 1 
