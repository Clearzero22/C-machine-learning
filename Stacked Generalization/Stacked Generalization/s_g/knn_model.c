#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<math.h>


void QuickSort(double **arr, int L, int R) {
	int i = L;
	int j = R;
	//֧��
	int kk = (L + R) / 2;
	double pivot = arr[kk][0];
	//�������˽���ɨ�裬ֻҪ���˻�û�н��棬��һֱɨ��
	while (i <= j) {
		//Ѱ��ֱ����֧������
		while (pivot > arr[i][0])
		{
			i++;
		}
		//Ѱ��ֱ����֧��С����
		while (pivot < arr[j][0])
		{
			j--;
		}
		//��ʱ�Ѿ��ֱ��ҵ��˱�֧��С����(�ұ�)����֧������(���)�����ǽ��н���
		if (i <= j) {
			double *temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			//double temp2 = arr[i][1];
			//arr[i][1] = arr[j][1];
			//arr[j][1] = temp2;
			i++;
			j--;
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
double euclidean_distance(double *row1, double *row2, int col) {
	double distance = 0;
	for (int i = 0; i < col - 1; i++) {
		distance += pow((row1[i] - row2[i]), 2);
	}
	return distance;
}
double* get_neighbors(double **train_data, int train_row, int col, double *test_row, int num_neighbors) {
	double *neighbors = (double *)malloc(num_neighbors * sizeof(double));
	double **distances = (double **)malloc(train_row * sizeof(double *));
	for (int i = 0; i < train_row; i++) {
		distances[i] = (double *)malloc(2 * sizeof(double));
		distances[i][0] = euclidean_distance(train_data[i], test_row, col);
		distances[i][1] = train_data[i][col - 1];
	}
	QuickSort(distances, 0, train_row - 1);
	for (int i = 0; i < num_neighbors; i++) {
		neighbors[i] = distances[i][1];
	}
	return neighbors;
}
double knn_single_predict(double **train_data, int train_row, int col, double *test_row, int num_neighbors) {
	double* neighbors = get_neighbors(train_data, train_row, col, test_row, num_neighbors);
	double result = 0;
	for (int i = 0; i < num_neighbors; i++) {
		result += neighbors[i];
	}
	return round(result / num_neighbors);
}


double* knn_predict(double **train, int train_size, double **test, int test_size, int col, int num_neighbors)
{
	double* predictions = (double*)malloc(test_size * sizeof(double));
	for (int i = 0; i < test_size; i++)
	{
		predictions[i] = knn_single_predict(train, train_size, col, test[i], num_neighbors);
	}
	return predictions;//���ض�test��Ԥ������
}

