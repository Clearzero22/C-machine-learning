#include<stdlib.h>
#include<stdio.h>

extern void coefficients(float** data, float* coef, int length);
float* get_test_prediction(int col,int row,float** train, float** test, int n_folds) {
	float* coef = (float*)malloc(col * sizeof(float));
	int i;
	for (i = 0; i < col; i++) {
		coef[i] = 0.0;
	}
	int fold_size = (int)row / n_folds;
	int train_size = fold_size * (n_folds - 1);
	coefficients(train, coef, train_size);//�����㷨����
	float* predictions = (float*)malloc(fold_size * sizeof(float));//��Ϊtest_size��fold_sizeһ����
	for (i = 0; i < fold_size; i++) {//��Ϊtest_size��fold_sizeһ����
		predictions[i] = coef[0] + coef[1] * test[i][0];
	}
	return predictions;//���ض�test��Ԥ������
}