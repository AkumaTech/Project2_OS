/*
Miguel Hernandez 
Miguel Manuel
Jose Avina

Project 2:
Suduko Solution Validator
*/
//#include <pch.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE              9
#define NUMBER_OF_THREADS 27

/* structure for passing data to threads */
typedef struct
{
int row;
int column;

} parameters;

void *validColumn(void *params);// thread that checks if the columns are valid
void *validRow(void *params);// thread that checks if the rows are valid
void *validGrid(void *params);// thread that checks if a 3x3 grid is valid

// Array that holds the validity of the sudoku solution.
int result[NUMBER_OF_THREADS] = {0};//initialized all to 0

//The sudoku board given
int sudoku[SIZE][SIZE] = {
	{6,2,4,5,3,9,1,8,7},
	{5,1,9,7,2,8,6,3,4},
	{8,3,7,6,1,4,2,9,5},
	{1,4,3,8,6,5,7,2,9},
	{9,5,8,2,4,7,3,6,1},
	{7,6,2,3,9,1,4,5,8},
	{3,7,1,9,5,6,8,4,2},
	{4,9,6,1,8,2,5,7,3},
	{2,8,5,4,7,3,9,1,6}
};

int main ()
{
int i,j ;// used to indicate the rows and columns of the sudoku board
int workerI = 0;//the worker index
pthread_t workers[NUMBER_OF_THREADS];// creates the 27 workers

//loops through the sudoku board
for (i = 0; i < SIZE; i++)// in total we should have 27 workers working on validating the solution
	{
	for (j = 0; j < SIZE; j++)
		{
		if (i%3 == 0 && j%3 == 0)//creates a thread if the section is a 3x3 area
			{
			parameters *data = (parameters *) malloc(sizeof(parameters));
			data -> row = i;
			data -> column = j;
			pthread_create(&workers[workerI++], NULL, validGrid, data);
			}
		if (i == 0)// creates a thread if we are at a column
			{
			parameters *data = (parameters *) malloc(sizeof(parameters));
			data -> row = i;
			data -> column = j;
			pthread_create(&workers[workerI++], NULL, validColumn, data); 
			}
		if (j == 0)//creates a section if we are at a row
			{
			parameters *data = (parameters *) malloc(sizeof(parameters));
			data -> row = i;
			data -> column = j;
			pthread_create(&workers[workerI++], NULL, validRow, data);
			}
		}
	}
for ( i = 0; i < NUMBER_OF_THREADS; i++)//this for loop waits for the threads to finish
	pthread_join(workers[i], NULL);

int count = 0;// used to determine if the sudoku board solution is valid
for (i = 0; i < NUMBER_OF_THREADS; i++)// goes through the results array to check for a 0
	{
	if(result[i] == 0)//outputs not valid if a zero is found
		{
		printf("Sudoku solution is not valid.\n");
		break;
		}
	else
		count++;	
	}

if (count == 27)//this should always work if the solution is correct
	printf("Sudoku solution is valid.\n");

return 0; 
}
/*
Column thread
This thread is to check the columns of a suduko board.
*/
void *validColumn(void *params)
{
parameters *param = (parameters*) params;
int row = param -> row;
int column = param -> column;

int validNum[SIZE] = {0};//holds the validity of the column
int i;
for (i = 0; i < SIZE; i++)// this function checks to see if the column has numbers 1-9
	{
	int num = sudoku[i][column];
	if(num < 1 || num > 9 || validNum[num - 1] == 1)
		pthread_exit(NULL);
	else
		validNum[num - 1] = 1;
	}
result[18 + column] = 1;// worker has made certain that the column is valid
pthread_exit(NULL);
}
/*
Row thread
This thread is to check the rows of the sudoko board to see if they are valid
*/
void *validRow(void *params)
{
parameters *param = (parameters*) params;
int row = param -> row;
int column = param -> column;

int validNum[SIZE] = {0};//holds the validity of the row
int i;
for (i = 0; i < SIZE; i++)// checks to see if the row has numbers 1-9
	{
	int num = sudoku[row][i];
	if (num < 1 || num > 9 || validNum[num - 1] == 1)
		pthread_exit(NULL);
	else
		validNum[num - 1] = 1;
	}
result[9 + row] = 1;//the worker has made cetain that the row is valid
pthread_exit(NULL);
}
/*
Grid thread
This thread checks to see if the grids of the suduko board are valid.
*/
void *validGrid(void *params)
{
parameters *param = (parameters*) params;
int row = param -> row;
int column = param -> column;

int validNum[SIZE] = {0};// holds the validity of the grid
int i, j;
for (i = row; i < row + 3; i++)// checks to see if the grid has numbers 1-9
	{
	for (j = column; j < column + 3; j++)
		{
		int num = sudoku[i][j];
		if (num < 1 || num > 9 || validNum[num - 1] == 1)
			pthread_exit(NULL);
		else
			validNum[num - 1] = 1;
		} 
	}
result[row + column/3] = 1;// worker has made certain that the grid is valid.
pthread_exit(NULL);
	
}





