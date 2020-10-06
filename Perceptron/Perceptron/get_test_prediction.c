#include<stdlib.h>
#include<stdio.h>

extern void train_weights(double **data, int col, double *weights, double l_rate, int n_epoch, int train_size);
extern double predict(int col, double *array, double *weights);
double* get_test_prediction(double** train, double** test,int row,int col, double l_rate, int n_epoch, int n_folds) {
	double* weights = (double*)malloc(col * sizeof(double));
	int i;
	for (i = 0; i < col; i++) {
		weights[i] = 0.0;
	}
	int fold_size = (int)row / n_folds;
	int train_size = fold_size * (n_folds - 1);
	//coefficients_sgd(train, coef, l_rate, n_epoch, train_size);//�����㷨ִ�в���
	train_weights(train, col,weights, l_rate, n_epoch, train_size);//�����㷨ִ�в���
	double* predictions = (double*)malloc(fold_size * sizeof(double));//��Ϊtest_size��fold_sizeһ����
	for (i = 0; i < fold_size; i++) {//��Ϊtest_size��fold_sizeһ����
		predictions[i] = predict(col,test[i], weights);
	}
	return predictions;//���ض�test��Ԥ������
}