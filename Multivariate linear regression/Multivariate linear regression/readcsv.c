#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#define row 4898
#define column 12

int readcsv(char* filename,int beginning, float data[row][column])
{
	FILE *fp = NULL;
	char* line;
	char* record;
	char buffer[1024];
	int i, j;
	if ((fp = fopen(filename, "at+")) != NULL)
	{
		fseek(fp, beginning, SEEK_SET);
		char delims[] = ",";
		int k = 0;
		int l = 0;
		while ((line = fgets(buffer, sizeof(buffer), fp)) != NULL)//��û�ж�ȡ���ļ�ĩβʱѭ������,fgets����ֵ�ǻ�������ַ��buffer�ǻ�������ַ,��ȡsizeof(buffer)�п��ܲ���һ�У�Ҳ�п��ܳ���һ�У�����һ����ֻ��һ�У������ڶ���bufferʱע�Ᵽ֤���С����һ��
		{
			record = strtok(line, ",");//���ر��ָ�Ƭ�εĵ�һ���ַ��ĵ�ַ
			while (record != NULL)//��ȡÿһ�е�����
			{
				//printf("%s  ", record);//����ȡ����ÿһ�����ݴ�ӡ����������ַ���ʱ������%s��printf��Ҫ��ָ������������������߼�����Ϊ��ָ���������洢�����ݣ������ṩ�ַ����׵�ַ�����������%c���������������ݾͿ��԰���������ָ������*p��
				data[k][l] = atof(record);
				//printf("datastr[%d][%d]=%f\n",k,l, data[k][l]);			
				record = strtok(NULL, ",");
				l += 1;
			}
			//printf("\n");
			l = 0;
			k += 1;
		}
		fclose(fp);
		/*for (i = 0; i < row; i++) {
			for (j = 0; j < column; j++) {
				printf("i=%d,j=%d,datastr=%f\n", i, j, data[i][j]);
			}
		}*/
	}
	return 0;
}