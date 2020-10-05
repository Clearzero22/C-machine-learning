#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<math.h>

extern int get_row(char *filename);
extern int get_col(char *filename);
extern void get_two_dimension(char *line, double **dataset, char *filename);
extern void normalize_dataset(double **dataset, int row, int col);
extern void evaluate_algorithm(double **dataset, int row, int col, int n_folds, int num_neighbors);

void QuickSort(double** arr, int L, int R) {
	int i = L;
	int j = R;
	//֧��
	int kk = (L + R) / 2;
	double pivot = arr[kk][0];
	//�������˽���ɨ�裬ֻҪ���˻�û�н��棬��һֱɨ��
	while (i <= j) {//Ѱ��ֱ����֧������
		while (pivot > arr[i][0])
		{
			i++;
		}//Ѱ��ֱ����֧��С����
		while (pivot < arr[j][0])
		{
			j--;
		}//��ʱ�Ѿ��ֱ��ҵ��˱�֧��С����(�ұ�)����֧������(���)�����ǽ��н���
		if (i <= j) {
			double* temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			i++; j--;
		}
	}//����һ��while��֤�˵�һ������֧�����߱�֧��С��֧����ұ߱�֧����ˡ�
	//����ߡ���������ֱ�����ʣ��һ����(�ݹ����)
	if (L < j)
	{
		QuickSort(arr, L, j);
	}
	//���ұߡ���������ֱ���ұ�ʣ��һ����(�ݹ����)
	if (i < R)
	{
		QuickSort(arr, i, R);
	}
}
// Calculate the Euclidean distance between two vectors
double euclidean_distance(double* row1, double* row2, int col) {
	double distance = 0;
	for (int i = 0; i < col - 1; i++) {
		distance += pow((row1[i] - row2[i]), 2);
	}
	return sqrt(distance);
}
// Locate the most similar neighbors
double* get_neighbors(double** train_data, int train_row, int col, double* test_row, int num_neighbors) {
	double* neighbors = (double*)malloc(num_neighbors * sizeof(double));
	double** distances = (double**)malloc(train_row * sizeof(double*));
	for (int i = 0; i < train_row; i++) {
		distances[i] = (double*)malloc(2 * sizeof(double));
		distances[i][0] = euclidean_distance(train_data[i], test_row, col);
		distances[i][1] = train_data[i][col - 1];
	}
	QuickSort(distances, 0, train_row - 1);
	for (int i = 0; i < num_neighbors; i++) {
		neighbors[i] = distances[i][1];
	}
	return neighbors;
}
double predict(double** train_data, int train_row, int col, double* test_row, int num_neighbors) {
	double* neighbors = get_neighbors(train_data, train_row, col, test_row, num_neighbors);
	double result = 0;
	for (int i = 0; i < num_neighbors; i++) {
		result += neighbors[i];
	}
	return result / num_neighbors;
}

void main() {
	char filename[] = "abalone.csv";
	char line[1024];
	int row = get_row(filename);
	int col = get_col(filename);
	//printf("row = %d, col = %d\n", row, col);
	double **dataset;
	dataset = (double **)malloc(row * sizeof(double *));
	for (int i = 0; i < row; ++i) {
		dataset[i] = (double*)malloc(col * sizeof(double));
	}
	get_two_dimension(line, dataset, filename);
	normalize_dataset(dataset, row, col);
	int k_fold = 5;
	int num_neighbors = 5;
	evaluate_algorithm(dataset, row, col, k_fold, num_neighbors);
}

