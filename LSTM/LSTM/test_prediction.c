#define innode  2       //��������
#define hidenode  12   //���ؽ����
#define cell_num 8  //LMTMϸ���� 

#define uniform_plus_minus_one ( (double)( 2.0 * rand() ) / ((double)RAND_MAX + 1.0) - 1.0 )  //��������ֲ� 

#include "math.h"
#include "stdlib.h"
#include "time.h"
#include "assert.h"
#include "string.h"
#include "stdio.h" 
//�����
double sigmoid(double x) 
{
    return 1.0 / (1.0 + exp(-x));
}
 
//������ĵ�����yΪ�����ֵ
double dsigmoid(double y)
{
    return y * (1.0 - y);  
}           
 
//tanh�ĵ�����yΪtanhֵ
double dtanh(double y)
{
    y = tanh(y);
    return 1.0 - y * y;  
}
 
//��һ��10��������ת��Ϊ2������
void int2binary(int n, int *arr)
{
    int i = 0;
    while(n)
    {
        arr[i++] = n % 2;
        n /= 2;
    }
    while(i < cell_num)
        arr[i++] = 0;
}

//ѵ��ģ�Ͳ����Ԥ��ֵ 
int* get_test_prediction(double **train, double **test, double l_rate, int n_epoch, int train_size,int test_size,int col){
	int epoch,i, j, k, m, p;
	int x[innode];
	double y;
	
	double W_I[innode][hidenode];     //���������������㵥Ԫ�������ŵ�Ȩֵ����
    double U_I[hidenode][hidenode];   //������һ��������뱾�����㵥Ԫ�������ŵ�Ȩֵ����
    double W_F[innode][hidenode];     //���������������㵥Ԫ�������ŵ�Ȩֵ����
    double U_F[hidenode][hidenode];   //������һ�������뱾�����㵥Ԫ�������ŵ�Ȩֵ����
    double W_O[innode][hidenode];     //���������������㵥Ԫ�������ŵ�Ȩֵ����
    double U_O[hidenode][hidenode];   //������һ������������ʱ�̵��������Ȩֵ����
    double W_G[innode][hidenode];     //���ڲ����¼����Ȩֵ����
    double U_G[hidenode][hidenode];   //���ڲ����¼����Ȩֵ����
    double W_out[hidenode];  //����������������Ȩֵ����
    
    // ��ʼ�� 
    for(i=0;i<innode;i++){
    	for(j=0;j<hidenode;j++){
    		W_I[i][j] = uniform_plus_minus_one;
    		W_F[i][j] = uniform_plus_minus_one;
    		W_O[i][j] = uniform_plus_minus_one;
    		W_G[i][j] = uniform_plus_minus_one;
		}
	}
	for(i=0;i<hidenode;i++){
		for(j=0;j<hidenode;j++){
			U_I[i][j] = uniform_plus_minus_one;
			U_F[i][j] = uniform_plus_minus_one;
			U_O[i][j] = uniform_plus_minus_one;
			U_G[i][j] = uniform_plus_minus_one;
		}
		W_out[i] = uniform_plus_minus_one;
	}
	
    for(epoch=0;epoch<n_epoch;epoch++){
    	for(i=0;i<train_size;i++){
    		double **I_vector = (double **)malloc((cell_num)*sizeof(double *));
			double **F_vector = (double **)malloc((cell_num)*sizeof(double *));
			double **O_vector = (double **)malloc((cell_num)*sizeof(double *));    
		    double **G_vector = (double **)malloc((cell_num)*sizeof(double *));      
		    double **M_vector = (double **)malloc((cell_num+1)*sizeof(double *));     
		    double **h_vector = (double **)malloc((cell_num+1)*sizeof(double *));     
		    double y_delta[cell_num];    //����������������ƫ��
		
			for(j=0;j<cell_num;j++){
		        M_vector[j] = (double *)malloc(hidenode*sizeof(double));
		        h_vector[j] = (double *)malloc(hidenode*sizeof(double));
		        I_vector[j] = (double *)malloc(hidenode*sizeof(double));
		        F_vector[j] = (double *)malloc(hidenode*sizeof(double));
		        O_vector[j] = (double *)malloc(hidenode*sizeof(double));
		        G_vector[j] = (double *)malloc(hidenode*sizeof(double));
			}
			M_vector[cell_num] = (double *)malloc(hidenode*sizeof(double));
		    h_vector[cell_num] = (double *)malloc(hidenode*sizeof(double));
 
        	int predict[cell_num];     //����ÿ�����ɵ�Ԥ��ֵ
 
        
        	double M[hidenode];     //����ֵ
        	double h[hidenode];     //���ֵ
        
        	for(j=0; j<hidenode; j++)  
        	{
            	M[j] = 0;
            	h[j] = 0;
            	M_vector[0][j] = 0;
        		h_vector[0][j] = 0;
        	}
        	
        	double a_int = train[i][0];
        	int a[cell_num];
        	double b_int = train[i][1];
        	int b[cell_num];
        	double c_int = train[i][2];
        	int c[cell_num];
        	int2binary(a_int, a);
			int2binary(b_int, b);
			int2binary(c_int, c);
        	
        	for(p=0;p<cell_num;p++){
	    		x[0]=a[p];
	    		x[1]=b[p];
	    		double t = (double)c[p];      //ʵ��ֵ
	    		double in_gate[hidenode];     //������
	            double out_gate[hidenode];    //�����
	            double forget_gate[hidenode]; //������
	            double g_gate[hidenode];      //C`t 
	            double memory[hidenode];       //����ֵ
	            double h[hidenode];           //�������ֵ
	            
	            double *h_pre = h_vector[p];
	            double *memory_pre = M_vector[p];
	    		
	    		for(k=0; k<hidenode; k++)
	            {   
	                //�����ת��������
	                double inGate = 0.0;
	                double outGate = 0.0;
	                double forgetGate = 0.0;
	                double gGate = 0.0;
	                double s = 0.0;
	                

	 
	                for(m=0; m<innode; m++) 
	                {
	                    inGate += x[m] * W_I[m][k]; 
	                    outGate += x[m] * W_O[m][k];
	                    forgetGate += x[m] * W_F[m][k];
	                    gGate += x[m] * W_G[m][k];
	                }
	                
	                for(m=0; m<hidenode; m++)
                	{
                    	inGate += h_pre[m] * U_I[m][k];
                    	outGate += h_pre[m] * U_O[m][k];
                    	forgetGate += h_pre[m] * U_F[m][k];
                    	gGate += h_pre[m] * U_G[m][k];
                	}
	 
					
	                in_gate[k] = sigmoid(inGate);
	                out_gate[k] = sigmoid(outGate);
	                forget_gate[k] = sigmoid(forgetGate);
	                g_gate[k] = sigmoid(gGate);
	 
	                double m_pre = memory_pre[k];
	                memory[k] = forget_gate[k] * m_pre + g_gate[k] * in_gate[k];
	                
	                h[k] = out_gate[k] * tanh(memory[k]);
	                
					I_vector[p][k] = in_gate[k];
					F_vector[p][k] = forget_gate[k];
					O_vector[p][k] = out_gate[k];
					M_vector[p+1][k] = memory[k];
					G_vector[p][k] = g_gate[k];
					h_vector[p+1][k] = h[k];
	            }

	            //���ز㴫���������
	            double out = 0.0;
	            for(j=0; j<hidenode; j++){
	                out += h[j] * W_out[j]; 
				}
             
	            y = sigmoid(out);               //��������Ԫ���
	            predict[p] = floor(y + 0.5);   //��¼Ԥ��ֵ
	            
				//�����׼������������ƫ��
	            y_delta[p] = (t - y) * dsigmoid(y);

			}
			//���򴫲�
 
	        //������ƫ�ͨ����ǰ֮��һ��ʱ�������������͵�ǰ������������
	        double h_delta[hidenode];  
	        double O_delta[hidenode];
	        double I_delta[hidenode];
	        double F_delta[hidenode];
	        double G_delta[hidenode];
	        double memory_delta[hidenode];
	        //��ǰʱ��֮���һ�����ز����
	        double O_future_delta[hidenode]; 
	        double I_future_delta[hidenode];
	        double F_future_delta[hidenode];
	        double G_future_delta[hidenode];
	        double memory_future_delta[hidenode];
	        double forget_gate_future[hidenode];
	        for(j=0; j<hidenode; j++)
	        {
	            O_future_delta[j] = 0.0;
	            I_future_delta[j] = 0.0;
	            F_future_delta[j] = 0.0;
	            G_future_delta[j] = 0.0;
	            memory_future_delta[j] = 0.0;
	            forget_gate_future[j] = 0.0;
	        }
	        
	        for(p=cell_num-1; p>=0 ; p--)
	        {
	            x[0] = a[p];
	            x[1] = b[p];
	 
	            //��ǰ���ز�

				double in_gate[hidenode];     //������
	            double out_gate[hidenode];    //�����
	            double forget_gate[hidenode]; //������
	            double g_gate[hidenode];      //C`t
	            double memory[hidenode];     //����ֵ
	            double h[hidenode];         //�������ֵ
				for(k=0;k<hidenode;k++){
					in_gate[k] = I_vector[p][k];
					out_gate[k] = O_vector[p][k];
					forget_gate[k] = F_vector[p][k]; //������
	            	g_gate[k] = G_vector[p][k];      //C`t 
	            	memory[k] = M_vector[p+1][k];     //����ֵ
	            	h[k] = h_vector[p+1][k];         //�������ֵ
				}
	            //ǰһ�����ز�
	            double *h_pre = h_vector[p];   
	            double *memory_pre = M_vector[p];
	 
	            //����������������֮�������Ȩ
	            for(j=0; j<hidenode; j++){
	            	W_out[j] += l_rate * y_delta[p] * h[j];  
				}
	                
	            //����������ÿ�����ص�Ԫ����������������Ȩֵ
	            for(j=0; j<hidenode; j++) 
	            {
	                h_delta[j] = y_delta[p] * W_out[j];
	                for(k=0; k<hidenode; k++)
	                {
	                    h_delta[j] += I_future_delta[k] * U_I[j][k];
	                    h_delta[j] += F_future_delta[k] * U_F[j][k];
	                    h_delta[j] += O_future_delta[k] * U_O[j][k];
	                    h_delta[j] += G_future_delta[k] * U_G[j][k];
	                }
	 
	                O_delta[j] = 0.0;
	                I_delta[j] = 0.0;
	                F_delta[j] = 0.0;
	                G_delta[j] = 0.0;
	                memory_delta[j] = 0.0;
	 
	                //�������У�����
	                O_delta[j] = h_delta[j] * tanh(memory[j]) * dsigmoid(out_gate[j]);
	                memory_delta[j] = h_delta[j] * out_gate[j] * dtanh(memory[j]) +
	                                 memory_future_delta[j] * forget_gate_future[j];
	                F_delta[j] = memory_delta[j] * memory_pre[j] * dsigmoid(forget_gate[j]);
	                I_delta[j] = memory_delta[j] * g_gate[j] * dsigmoid(in_gate[j]);
	                G_delta[j] = memory_delta[j] * in_gate[j] * dsigmoid(g_gate[j]);
	                
	                O_future_delta[j] = O_delta[j];
	            	F_future_delta[j] = F_delta[j];
	            	I_future_delta[j] = I_delta[j];
	            	G_future_delta[j] = G_delta[j];
	            	memory_future_delta[j] = memory_delta[j];
	            	forget_gate_future[j] = forget_gate[j];	
	 
	                //����ǰһ�������������������֮���Ȩֵ
	                for(k=0; k<hidenode; k++)
	                {
	                	U_I[k][j] += l_rate * I_delta[j] * h_pre[k];
	                    U_F[k][j] += l_rate * F_delta[j] * h_pre[k];
	                    U_O[k][j] += l_rate * O_delta[j] * h_pre[k];
	                    U_G[k][j] += l_rate * G_delta[j] * h_pre[k];
	                }
	 
	                //����������������֮�������Ȩ
	                for(k=0; k<innode; k++)
	                {
	                    W_I[k][j] += l_rate * I_delta[j] * x[k];
	                    W_F[k][j] += l_rate * F_delta[j] * x[k];
	                    W_O[k][j] += l_rate * O_delta[j] * x[k];
	                    W_G[k][j] += l_rate * G_delta[j] * x[k];
	                }
	            }
	        }
			free(I_vector);
	        free(F_vector);
	        free(O_vector);
	        free(G_vector);
	        free(M_vector);
	        free(h_vector);
		}
	}
	int *predictions=(int*)malloc(test_size*sizeof(int));
	// Ԥ��
	for(i=0;i<test_size;i++){
		double **M_vector = (double **)malloc((cell_num+1)*sizeof(double *));     
		double **h_vector = (double **)malloc((cell_num+1)*sizeof(double *));
		for(j=0;j<cell_num+1;j++){
		    M_vector[j] = (double *)malloc(hidenode*sizeof(double));
		    h_vector[j] = (double *)malloc(hidenode*sizeof(double));
		}
		
		
		int predict[cell_num];               //����ÿ�����ɵ�Ԥ��ֵ
        // memset(predict, 0, sizeof(predict));
        
		double M[hidenode];     //����ֵ
        double h[hidenode];     //���ֵ
        
        for(j=0; j<hidenode; j++)  
        {
            M[j] = 0;
            h[j] = 0;
            M_vector[0][j] = 0;
        	h_vector[0][j] = 0;
        }

        	
        double a_int = test[i][0];
        int a[cell_num];
        double b_int = test[i][1];
        int b[cell_num];
        double c_int = test[i][2];
        int c[cell_num];

		int2binary(a_int, a);
		int2binary(b_int, b);
		int2binary(c_int, c);
        
        	
        for(p=0;p<cell_num;p++){
	    	x[0]=a[p];
	    	x[1]=b[p];
	    	double in_gate[hidenode];     //������
	        double out_gate[hidenode];    //�����
	        double forget_gate[hidenode]; //������
	        double g_gate[hidenode];      //C`t 
	        double memory[hidenode];       //����ֵ
	        double h[hidenode];           //�������ֵ
	    		
	    	for(k=0; k<hidenode; k++)
	        {   
	            //�����ת��������
	            double inGate = 0.0;
	            double outGate = 0.0;
	            double forgetGate = 0.0;
	            double gGate = 0.0;
	            double s = 0.0;
	 
	 
	            double *h_pre = h_vector[p];
	            double *memory_pre = M_vector[p];
	                
	            for(m=0; m<innode; m++) 
	            {
	                inGate += x[m] * W_I[m][k]; 
	                outGate += x[m] * W_O[m][k];
	                forgetGate += x[m] * W_F[m][k];
	                gGate += x[m] * W_G[m][k];
	            }
	                
	            for(m=0; m<hidenode; m++)
                {
                    inGate += h_pre[m] * U_I[m][k];
                    outGate += h_pre[m] * U_O[m][k];
                    forgetGate += h_pre[m] * U_F[m][k];
                    gGate += h_pre[m] * U_G[m][k];
                }
	 
	
	            in_gate[k] = sigmoid(inGate);   
	            out_gate[k] = sigmoid(outGate);
	            forget_gate[k] = sigmoid(forgetGate);
	            g_gate[k] = sigmoid(gGate);
	 
	            double m_pre = memory_pre[k];
	            memory[k] = forget_gate[k] * m_pre + g_gate[k] * in_gate[k];
	            h[k] = out_gate[k] * tanh(memory[k]);
	            
	            M_vector[p+1][k] = memory[k];
				h_vector[p+1][k] = h[k];
	        }

	        //���ز㴫���������
	        double out = 0.0;
	        for(j=0; j<hidenode; j++){
	            out += h[j] * W_out[j];
			}
	        y = sigmoid(out);     //��������Ԫ���
	        predict[p] = floor(y + 0.5);
	        
	    }
	    free(M_vector);
	    free(h_vector);
	    
	    int out=0;
		for(k=cell_num-1; k>=0; k--){
            out += predict[k] * pow(2, k);
		}		
		predictions[i] = out;
	}

	return predictions;
}
