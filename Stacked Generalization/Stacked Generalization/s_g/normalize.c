#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void normalize_dataset(double **dataset, int row, int col)
{
	// �� ����ѭ��
	double maximum, minimum;
	for (int i = 0; i < col - 1; i++)
	{
		// ��һ��Ϊ���⣬ֵΪ0�����ܲ�����������Сֵ
		maximum = dataset[0][i];
		minimum = dataset[0][i];
		//�� ����ѭ��
		for (int j = 0; j < row; j++)
		{
			maximum = max(dataset[j][i], maximum);
			minimum = min(dataset[j][i], minimum);
		}
		// ��һ������
		for (int j = 0; j < row; j++)
		{
			dataset[j][i] = (dataset[j][i] - minimum) / (maximum - minimum);
		}
	}
}