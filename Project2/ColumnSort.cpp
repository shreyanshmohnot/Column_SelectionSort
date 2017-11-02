#include "utility.h"
#include "timer.h"

using namespace std;

void selection_sort(int[], int);
bool is_sorted(int[], int);

int flag = 1; // flag is used to check the array is sorted or not

int main()
{
	// file name for test case file
	string file_name = "Part1Data";
	
	int number, i = 0; // number represents the variable to fetch line by line from file
	
	double total_time;

	int rows = 4000;
	int columns = 25;
	
	// Dyanmic allocation for 2D Column Array
	
	int** column_array = new int*[rows];
	for (int i = 0; i < rows; i++) {
		column_array[i] = new int[columns];
	}

	// Stream file to read data from it line by line
	
	ifstream read_stream;
	file_name.append(".txt");
	read_stream.open(file_name);

	if (read_stream.fail()) {
		cout << "Error Opening File" << endl;
		exit(0);
	}

	// Add values from file into column array
	
	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; j++) {
			read_stream >> number;
			column_array[j][i] = number;
		}
	}
	
	Timer time = Timer();

	/*
	*
	* First Step - Sorting values in each column
	*
	*/
	
	int *temp_array = new int[rows];
	
	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; j++) {
			temp_array[j] = column_array[j][i];
		}
		selection_sort(temp_array,rows);
		for (int j = 0; j < rows; j++) {
			column_array[j][i] = temp_array[j];
		}
	}
	
	/*
	*
	* Second Step - Transpose matrix by column to row major
	*
	*/
	
	int prows = rows;
	int pcolumns = columns;

	// Dyanmic allocation for 2D Transpose Array
	int** transpose_column_array = new int*[rows];
	for (int i = 0; i < rows; i++) {
		transpose_column_array[i] = new int[columns];
	}
	
	int c_rows = 0, c_columns = 0;
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (c_rows >= prows) {
				c_rows = 0;
				c_columns++;
			}
			transpose_column_array[i][j] = column_array[c_rows++][c_columns];
		}
	}
	
	/*
	*
	* Third Step - Sort Transpose matrix by column
	*
	*/

	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; j++) {
			temp_array[j] = transpose_column_array[j][i];
		}
		selection_sort(temp_array, rows);
		for (int j = 0; j < rows; j++) {
			transpose_column_array[j][i] = temp_array[j];
		}
	}
	
	/*
	*
	* Fourth Step - Transpose the transposed matrix by row to column major
	*
	*/

	int n_columns = columns + 1;
	
	// Dyanmic allocation for 2D New Transpose Array
	
	int** next_transpose_column_array = new int*[rows];
	for (int i = 0; i < rows; i++) {
		next_transpose_column_array[i] = new int[n_columns];
	}
	c_rows = 0;
	c_columns = 0;
	
	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; j++) {
			if (c_columns >= pcolumns) {
				c_columns = 0;
				c_rows++;
			}
			next_transpose_column_array[j][i] = transpose_column_array[c_rows][c_columns++];
		}
	}

	/*
	*
	* Fifth Step - Sort the Transpose matrix by columns
	*
	*/

	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; j++) {
			temp_array[j] = next_transpose_column_array[j][i];
		}
		selection_sort(temp_array, rows);
		for (int j = 0; j < rows; j++) {
			next_transpose_column_array[j][i] = temp_array[j];
		}
	}

	/*
	*
	* Sixth Step - Shift Transpose matrix by [r/2] values bottom and add new values in first and last column
	*
	*/

	for (int i = columns; i >= 0; i--) {
		int k = rows / 2;
		int m = 0;
		for (int j = 0; j < (rows / 2); j++) {
			if (i == 0)
				next_transpose_column_array[j][i] = -32767;
			else
				next_transpose_column_array[j][i] = next_transpose_column_array[k++][i - 1];
		}
		for (int j = (rows / 2); j < rows; j++) {
			if (i == columns)
				next_transpose_column_array[j][i] = 32767;
			else
				next_transpose_column_array[j][i - 1] = next_transpose_column_array[m++][i - 1];
		}
	}

	/*
	*
	* Seventh Step - Sort the Shifted Transpose matrix by column
	*
	*/

	for (int i = 0; i < n_columns; i++) {
		for (int j = 0; j < rows; j++) {
			temp_array[j] = next_transpose_column_array[j][i];
		}
		selection_sort(temp_array, rows);
		for (int j = 0; j < rows; j++) {
			next_transpose_column_array[j][i] = temp_array[j];
		}
	}

	/*
	*
	* Eighth Step - Shift Transpose matrix by [r/2] values up and remove values in first and last column.
	* The Column Array is sorted accordingly. 
	*
	*/

	for (int i = 0; i < columns; i++) {
		int k = rows / 2;
		int m = 0;
		for (int j = 0; j < (rows / 2); j++) {
			next_transpose_column_array[j][i] = next_transpose_column_array[k++][i];
		}
		for (int j = (rows / 2); j < rows; j++) {
			next_transpose_column_array[j][i] = next_transpose_column_array[m++][i + 1];
		}
	}

	total_time = time.elapsed_time();

	/*
	*
	* Nineth Step - Check Result is sorted or not.
	*
	*/

	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; j++) {
			temp_array[j] = next_transpose_column_array[j][i];
		}
		if (!is_sorted(temp_array, rows)) {
			flag = 0;
			cout << " Arrays Sorted" << endl;
		}
	}
	
	/*
	*
	* Tenth Step - Add the result array into new file.
	*
	*/

	// file name for test case file

	string result_file = "Result.txt";
	
	// Stream file to write data to it line by line

	ofstream input_stream;
	input_stream.open(result_file, std::ofstream::out | std::ofstream::trunc);
	if (input_stream.fail()) {
		cout << "Error Opening File" << endl;
		exit(1);
	}

	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; j++) {
			number = next_transpose_column_array[j][i];
			input_stream << number << endl;
		}
	}

	input_stream.close();
	
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(3);
	cout << "Total Time for column Sort : " << total_time << endl;

	system("PAUSE");
	return 0;
}

void selection_sort(int sort_array[], int rows)
{
	int j;
	int min_value;

	for (int i = 0; i < rows - 1; i++) {
		min_value = i;
		for (j = i + 1; j < rows; j++) {
			if (sort_array[j] < sort_array[min_value]) {
				min_value = j;
			}
		}
		
		// Swapping minimum element with the first element
		
		int temp = sort_array[min_value];
		sort_array[min_value] = sort_array[i];
		sort_array[i] = temp;
	}
}

bool is_sorted(int A[], int n)
{
	// Array has one or no element
	if (n == 0 || n == 1)
		return true;

	for (int i = 1; i < n; i++)

		// Unsorted pair found
		if (A[i - 1] > A[i])
			return false;

	// No unsorted pair found
	return true;
}