#ifndef __LOADCSV_H__
#define __LOADCSV_H__


void print_two_dimension(double** data, int row, int col);
int get_row(char *filename);
int get_col(char *filename);
int load_csv(char *filename,double**data);
//double **data;

int load_csv(char *filename,double**data)
{
    //char filename[] = filename;
    char line[1024];
	FILE* stream = fopen(filename, "r");
	int i = 0;
	while (fgets(line, 1024, stream))//���ж�ȡ
    {
    	int j = 0;
    	char *tok;
        char* tmp = strdup(line);
        for (tok = strtok(line, ","); tok && *tok; j++, tok = strtok(NULL, ",\n")){
        	data[i][j] = atof(tok);//ת���ɸ�����
		}//�ַ�����ֲ���
        i++;
        free(tmp);
    }
    fclose(stream);//�ļ��رղ���
	return 0;
}


void print_two_dimension(double** data, int row, int col)
{
	int i, j;
	for(i=0; i<row; i++){
		for(j=0; j<col; j++){
			printf("%f  ", data[i][j]);
		}
		printf("\n");
	}
}

int get_row(char *filename)
{
	char line[1024];
	int i = 0;
	FILE* stream = fopen(filename, "r");
	while(fgets(line, 1024, stream)){
		i++;
	}
	fclose(stream);
	return i;
}

int get_col(char *filename)
{
	char line[1024];
	int i = 0;
	FILE* stream = fopen(filename, "r");
	fgets(line, 1024, stream);
	char* token = strtok(line, ",");
	while(token){
		token = strtok(NULL, ",");
		i++;
	}
	fclose(stream);
	return i;
}



#endif // LOAD_CSV_H
