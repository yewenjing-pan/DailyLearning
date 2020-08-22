#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/****************************Ŀ�꺯���ı�׼��ʽ��Ŀ�꺯�������ֵ����ʽ**********************************/
/*****************************����Ⱥ����Ҫ�Ĳ�����������С�ٶȣ����ڲ�ͬ�����⣬�����С�ٶȿ��ܻ�仯������ٶ�̫�󣬻�ɳ���������̫С������ֲ����Ž�******************************************/

#define generation 50//��������
#define N 30//����������һ��ȡ20-40���Խ��ѻ��ض������������ȡ��100��200��
#define D 13//δ֪������
float V[N][D];//�����ٶ�
float X[N][D];//����λ��
float Vmax = 10;//����ٶ�
float Vmin = -10;//������С�ٶ�
float local_best[N][D];//���ӵ���ʷ����λ��
float local_value[N];//���ӵ���ʷ����λ�õ�Ŀ��ֵ
float global_best[D];//Ⱥ���ȫ������λ��
float best_value=0;//��õĽ�
//float Upbound[D] = { 5 };//ȡֵ�Ͻ�
//float Lowbound[D] = { -5 };//ȡֵ�½�
float Upbound[D] = { 1,1,1,1,1,1,1,1,1,3,3,3,1 };
float Lowbound[D] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };


void Initialization();//��ʼ��
void Evaluation();//����ÿ�����ӵ���Ӧ��
void Update(int gen);//���¾ֲ����ź�ȫ������
float Objective_function(int gen);//Ŀ�꺯��
float W_change(int gen);//wֵ�ı�
int Constraint_check( int gen);//Լ�����

int Constraint_check(int gen)//Լ�����
{
	float g[9];
	g[1] = 2 * X[gen][0] + 2 * X[gen][1] + X[gen][9] + X[gen][10] - 10;
	g[2] = 2 * X[gen][0] + 2 * X[gen][2] + X[gen][9] + X[gen][11] - 10;
	g[3] = 2 * X[gen][1] + 2 * X[gen][2] + X[gen][10] + X[gen][11] - 10;
	g[4] = -8 * X[gen][0] + X[gen][9];
	g[5] = -8 * X[gen][1] + X[gen][10];
	g[6] = -8 * X[gen][2] + X[gen][11];
	g[7] = -2 * X[gen][3] - X[gen][4] + X[gen][9];
	g[8] = -2 * X[gen][5] - X[gen][6] + X[gen][10];
	g[0] = -2 * X[gen][7] - X[gen][8] + X[gen][11];
	for (size_t i = 0; i < 9; i++)
		printf("g[%d]=%f\n", i, g[i]);
	if (g[1] <= 0 && g[2] <= 0 && g[3] <= 0 && g[4] <= 0 && g[5] <= 0 && g[6] <= 0 && g[7] <= 0 && g[8] <= 0 && g[0] <= 0)
		return 1;
	return 0;
}



void Initialization()  //��Ⱥ��ʼ��
{
	int flag;
	for (size_t i = 0; i < N; i++)
	{
    	mark:
		for (size_t j = 0; j < D; j++)
		{
			X[i][j] = Lowbound[j] + (Upbound[j] - Lowbound[j])*rand() / (RAND_MAX + 1.0);
			V[i][j] = Vmax * rand() / (RAND_MAX + 1.0);
			printf("x[%d][%d]=%f\t", i, j, X[i][j]);
			local_best[i][j] = X[i][j];
		}
		flag = Constraint_check(i);//�����ʼ���õ������ݲ�����Լ�����������³�ʼ��
		if (flag == 0)
			goto  mark;
		local_value[i] = Objective_function(i);
	}
}

float Objective_function(int x)  //Ŀ�꺯��ֵ����
{
	float f = 0;
	/*for (size_t i = 0; i < D; i++)
	{
		f = f + X[x][i] * X[x][i];
	}*/
	//f = X[x][0] * X[x][1] - X[x][2];
	//f = X[x][0] + 10 * sin(5 * X[x][0]) + 7 * cos(4 * X[x][0]);
	//f = 5*sin(X[x][0] * X[x][1])+X[x][0] * X[x][0] + X[x][1] * X[x][1];
	//f= 20 * pow((X[x][0] * X[x][0] - X[x][1] * X[x][1]), 2) - pow((1 - X[x][1]), 2) - 3 * pow((1 + X[x][1]), 2) ;
	f = -(5 * (X[x][0] + X[x][1] + X[x][2] + X[x][3]) - 5 * (pow(X[x][0], 2) + pow(X[x][1], 2) + pow(X[x][2], 2) + pow(X[x][3], 2)) - (X[x][4] + X[x][5] + X[x][6] + X[x][7] + X[x][8] + X[x][9] + X[x][10] + X[x][11] + X[x][12]));
	return f;
}

void Evaluation()   //����ÿ��������ʷ���ֵ����Ⱥ���ֵ
{
	float f;
	float max=0;
	int flag_best;
	for (size_t i = 0; i < N; i++)
	{
		f = Objective_function(i);
		if (f > local_value[i])//���ֵ
		{
			local_value[i] = f;
			for (size_t j = 0; j < D; j++)
				local_best[i][j]= X[i][j];
		}
	}
	for (size_t i = 0; i < N; i++)
	{
		if (max < local_value[i])//���ֵ
		{
			max = local_value[i];
			flag_best = i;
		}
	}
	for (size_t i = 0; i < D; i++)
		global_best[i] = local_best[flag_best][i];
	//printf("global_best=%f\n", max);
}

float W_change(int gen)  //W����Ӧ����
{
	float W;
	double W_int = 0.9;
	double W_end = 0.4;
	W = (W_int - W_end)*(generation - gen) / generation + W_end;
	return W;
}

void Update(int gen)  //������Ⱥ
{
	int c1 = 2;
	int c2 = 2;
	float w;
	int flag;
	float rand_1;
	float rand_2;
	for (size_t i = 0; i < N; i++)
	{
		w=W_change(gen);
		mark:
		rand_1 = rand() / (RAND_MAX + 1.0);
		rand_2 = rand() / (RAND_MAX + 1.0);
		for (size_t j = 0; j < D; j++)
		{

			V[i][j] = w * V[i][j] + c1 * rand_1*(local_best[i][j] - X[i][j]) + c2 * rand_2*(global_best[j] - X[i][j]);
			if (V[i][j] > Vmax)
				V[i][j] = Vmax;
			if (V[i][j] < Vmin)
				V[i][j] = Vmin;
			X[i][j] = X[i][j] + V[i][j];
			if (X[i][j] > Upbound[j] )
				X[i][j] = Upbound[j];
			if (X[i][j] < Lowbound[j])
				X[i][j] = Lowbound[j];
		}
		flag = Constraint_check(i);
		if (flag == 0)
			goto mark;
		printf("gen=%d\n", gen);
		printf("����%d\n", i);
	}
}

void main()
{
	Initialization();
	for (size_t i = 0; i < generation; i++)
	{
		Evaluation();
		Update(i);
	}
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < D; j++)
			printf("x[%d][%d]=%f\t", i, j, X[i][j]);
		printf("f=%f\n", Objective_function(i));
	}
}