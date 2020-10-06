#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

//#include <iostream>
//#include <vector>
//#include <string>
//#include <fstream>
//#include <sstream>

#define OUT_COUT  3     //�������ά��
#define IN_COUT   7     //��������ά��
#define COUT      150    //ѵ���������������Ѿ���׼�� 
#define TEST_COUNT  50    //���Լ������������Ѿ���׼�� 

typedef struct {       //bp�˹�������ṹ
    int h;             //ʵ��ʹ����������
    double v[IN_COUT][50];   //���ز�Ȩ����i,����ڵ��������Ϊ100
    double w[50][OUT_COUT];   //�����Ȩ����
    double a;          //ѧϰ��
    double b;          //���ȿ��Ʋ���
    int LoopCout;      //���ѭ������
} bp_nn;


//*******************************************************************************************************************************************

//Sigmoid����
double fnet(double net) { 
    return 1/(1+exp(-net));
}

//��ʼ��bp����
int InitBp(bp_nn *bp) { 
   
    printf("����������ڵ����������Ϊ100������Ϊ5����\n");   
    scanf("%d", &(*bp).h);
   
    printf("������ѧϰ�ʣ�����Ϊ0.3����\n");
    scanf("%lf", &(*bp).a);    

    printf("�����뾫�ȿ��Ʋ�����\n");
    scanf("%lf", &(*bp).b);

    printf("���������ѭ������������Ϊ500����\n");
    scanf("%d", &(*bp).LoopCout);


	//**��ʼ������(1)
	// **
    int i, j;
    srand((unsigned)time(NULL));
    for (i = 0; i < IN_COUT; i++)
        for (j = 0; j < (*bp).h; j++)
            (*bp).v[i][j] = rand() / (double)(RAND_MAX);   //�����Ȩ�ؾ��� 
    for (i = 0; i < (*bp).h; i++)
        for (j = 0; j < OUT_COUT; j++)
            (*bp).w[i][j] = rand() / (double)(RAND_MAX);   //�����Ȩ�ؾ��� 
   //**
   
   
    return 1;
}

//ѵ��bp���磬����Ϊx���������Ϊy

//*ѵ�����磨������ѵ������trainBP(5)   ����������� ��ʼ������ �����Ծ�� ǰ�򴫲� ���㷴�򴫲���� ����Ȩ�� 
//** 
int TrainBp(bp_nn *bp, double x[COUT][IN_COUT], int y[COUT][OUT_COUT]) {
    double f = (*bp).b;                      //���ȿ��Ʋ���
    double a = (*bp).a;                      //ѧϰ��
    int h = (*bp).h;                         //����ڵ���
    double v[IN_COUT][50], w[50][OUT_COUT]; //Ȩ�ؾ���
    double Ch_v[IN_COUT][50], Ch_w[50][OUT_COUT]; //Ȩ�ؾ����޸���
    double ChgH[50], ChgO[OUT_COUT];         //�޸�������
    double O1[50], O2[OUT_COUT];             //���������������
    int LoopCout = (*bp).LoopCout;           //���ѭ������
    int i, j, k, n;
    double temp;

    for (i = 0; i < IN_COUT; i++)            // weight
        for (j = 0; j < h; j++)
            v[i][j] = (*bp).v[i][j];
    for (i = 0; i < h; i++)
        for (j = 0; j < OUT_COUT; j++)
            w[i][j] = (*bp).w[i][j];
   
    double e = f + 1;
    for (n = 0; e > f && n < LoopCout; n++) { //��ÿ������ѵ������
        
		e = 0;
        for (j = 0; j < OUT_COUT; j++)
            ChgO[j] = 0;
        for (j = 0; j < h; j++)
            ChgH[j] = 0;
        for (j = 0; j < h; j++)         
            for (k = 0; k < OUT_COUT; k++)
                Ch_w[j][k] = 0;
        for (j = 0; j < IN_COUT; j++)   
            for (k = 0; k < h; k++)
                Ch_v[j][k] = 0;
        
		for (i= 0; i < COUT; i++) {
            
            
            //*�����Ծ��activate(2)
			//** 
			for (k= 0; k < h; k++) {          //���������������
                temp = 0;
                for (j = 0; j < IN_COUT; j++)
                    temp = temp + x[i][j] * v[j][k];   
                O1[k] = fnet(temp);
            }
            //**
            
            //*ǰ�򴫲�forward_propagate(3) 
            //**
            for (k = 0; k < OUT_COUT; k++) { //����������������
                temp = 0;
                for (j = 0; j < h; j++)
                    temp = temp + O1[j] * w[j][k];
                O2[k] = fnet(temp);
            }
            //**
            
            //*���㷴�򴫲����backward_propagate_error(3)
            //**
            for (j = 0; j < OUT_COUT ; j++)   //����������
                e = e + (y[i][j] - O2[j]) * (y[i][j] - O2[j]);
            for (j = 0; j < OUT_COUT; j++)   
                ChgO[j] = O2[j] * (1 - O2[j]) * (y[i][j] - O2[j]);
            for (j = 0; j < h; j++)         
                for (k = 0; k < OUT_COUT; k++)
                    Ch_w[j][k] += a * O1[j] * ChgO[k]; //�ۼ���������ѵ����ĸı���
            for (j = 0; j < h; j++) {     
                temp = 0;
                for (k = 0; k < OUT_COUT; k++)
                    temp = temp + w[j][k] * ChgO[k];
                ChgH[j] = temp * O1[j] * (1 - O1[j]);
            }
            for (j = 0; j < IN_COUT; j++)   
                for (k = 0; k < h; k++)
                    Ch_v[j][k] += a * x[i][j] * ChgH[k]; //�ۼ���������ѵ����ĸı�������������˳��Ӱ��
            //**
            
        }
        
        //*����Ȩ��update_weights(4)
        //**
        for (j = 0; j < h; j++)           //�޸������Ȩ����
            for (k = 0; k < OUT_COUT; k++)
                w[j][k] = w[j][k] + Ch_w[j][k];
        for (j = 0; j < IN_COUT; j++)     //�޸����ز�Ȩ����
            for (k = 0; k < h; k++)
                v[j][k] = v[j][k] + Ch_v[j][k];
        //**
    }
    printf("�ܹ�ѭ��������%d\n", n);
    printf("�����������Ȩ����\n");
    for (i = 0; i < IN_COUT; i++) {   
        for (j = 0; j < h; j++)
            printf("%f    ", v[i][j]);   
        printf("\n");
    }
    printf("������������Ȩ����\n");
    for (i = 0; i < h; i++) {
        for (j = 0; j < OUT_COUT; j++)
            printf("%f    ", w[i][j]);   
        printf("\n");
    }
    for (i = 0; i < IN_COUT; i++)             //�ѽ�����ƻؽṹ��
        for (j = 0; j < h; j++)
            (*bp).v[i][j] = v[i][j];
    for (i = 0; i < h; i++)
        for (j = 0; j < OUT_COUT; j++)
            (*bp).w[i][j] = w[i][j];
    printf("BP����ѵ��������\n");

    return 1;
}
//** 

//ʹ��bp����useBP(6)

//*��ѵ�������������Ԥ��ֵ
//**
int UseBp(bp_nn *bp) {    
    double Input[TEST_COUNT][IN_COUT];
    int Output[TEST_COUNT][1];
    int count=0;
    int flag = 0;
    
	double O1[50];
    double O2[OUT_COUT]; //O1Ϊ�������,O2Ϊ��������
    int num=0;
    
	ReadFromCSV("test_data.csv",Input,num);    //���ļ���x�� 
	ReadFromCSV1("test_data0.csv",Output,num); //���ļ����������y�� 
	
    for(int a=0;a<TEST_COUNT;a++) {              
		int i, j;
	    double temp;
        
		for (i = 0; i < (*bp).h; i++) {
            temp = 0;
            for (j = 0; j < IN_COUT; j++)
                temp += Input[a][j] * (*bp).v[j][i];
            O1[i] = fnet(temp);
        }
        for (i = 0; i < OUT_COUT; i++) {
            temp = 0;
            for (j = 0; j < (*bp).h; j++)
                temp += O1[j] * (*bp).w[j][i];
            O2[i] = fnet(temp);
        }
        //printf("����%.4f  %.4f %.4f\n",Input[a][0],Input[a][1],Input[a][2]);
        printf("�����   ");
            printf("%.3f  %.3f %.3f",O2[0],O2[1],O2[2]);    //������� 
        printf("\n");
        
        //���з��� 
		if((O2[1]-O2[0])>0.0001) {
			if((O2[2]-O2[1]>0.0001)){
				flag = 3; 
				printf("������\n\n");
			}
			else{
				flag = 2;
				printf("�ڶ���\n\n");
			} 
		}else{
			flag = 1;
			printf("��һ��\n\n");
		}
		
		//����׼ȷ�� 
		if(flag == Output[a][1]) count++;
    }
    printf("Accuracy Rate %.4f\n",double(count)/double(TEST_COUNT));
    return 1;
    
}

//**


//
double x[COUT][IN_COUT];
int y[COUT][OUT_COUT];
int main(int argc, char const *argv[])
{
    
	int num=0;
	ReadFromCSV("training_data.csv",x,num);   //���ļ���x�� 

	int num0=0;
	ReadFromCSV0("training_data0.csv",y,num0); //���ļ����������y�� 

    bp_nn bp;

    InitBp(&bp);                    //��ʼ��bp����ṹ
    TrainBp(&bp, x, y);             //ѵ��bp������
    UseBp(&bp);                     //����bp������
	
    return 1;
} 

//////////////////////////////////////////// main�������ø�ʽ /////////////////////////////////////////////////////////////
extern int get_row(char *filename);
extern int get_col(char *filename);
extern void get_two_dimension(char *line, double **dataset, char *filename);
extern void normalize_dataset(float **dataset,int row, int col);
extern float rmse_metric(float *actual, float *predicted, int fold_size);
extern float accuracy_metric(float *actual, float *predicted, int fold_size);
extern double  cross_validation_split(double **dataset, int row, int n_folds, int fold_size);
extern float evaluate_algorithm(float **dataset, int n_folds, int fold_size, float l_rate, int n_epoch) ;
extern float get_test_prediction(float **train, float **test, float l_rate, int n_epoch, int fold_size);


int main()
{
	//��ȡcsv 
	char filename[] = "seeds_data.csv";
    char line[1024];
    row = get_row(filename);
    col = get_col(filename);
    dataset = (double **)malloc(row*sizeof(int *));
	for (int i = 0; i < row; ++i){
		dataset[i] = (double *)malloc(col*sizeof(double));
	}//��̬�����ά����	
	get_two_dimension(line, dataset, filename);
	
	//������Ӧ���� 
	float l_rate = 0.1;		//ѧϰ�� 
	int epoch = 200;		//ѭ������ 
	int n_folds = 5;		//���ݷ����� 
    fold_size=(int)(row/n_folds);		//ÿ��������� 
    
	//��һ������
	normalize_dataset(dataset, row, col) 
	
	//�����ݻ���Ϊ k ��
	cross_validation_split(dataset, row, n_folds, fold_size)	
	 
    //�����ֵ�k�۽�����֤����Ԥ������׼ȷ��
    evaluate_algorithm(dataset, n_folds, fold_size, l_rate, n_epoch) 
    
    
}
 
