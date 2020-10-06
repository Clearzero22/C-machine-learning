#include <stdlib.h>
#include <stdio.h>

double  ***cross_validation_split(double **dataset, int row, int n_folds, int fold_size,int col)
{
    srand(10);//����
    double ***split;
    int i,j=0,k=0;
    int index;
    split=(double***)malloc(n_folds*sizeof(double**));
    for(i=0;i<n_folds;i++)
    {
        split[i] = (double**)malloc(fold_size * sizeof(double*));
        while(j<fold_size)
        {
            split[i][j] = (double*)malloc(col * sizeof(double));
            index=rand()%row;
            split[i][j] = dataset[index];
            
            for(k=index;k<row-1;k++)//forѭ��ɾ����������б�randȡ����Ԫ��
            {
                dataset[k]=dataset[k+1];
            }
            row--;//ÿ�����ȡ��һ����������-1����֤�����ظ�ȡĳһ��
            j++;
        }
        j=0;//����j
    }
    return split;
}
