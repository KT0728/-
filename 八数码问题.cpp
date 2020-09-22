#include<stdio.h>
#include<math.h>
#include<time.h>
#define SIZE 10000
int start[3][3]={0};
int end[3][3]={0};
struct node 
{
	int index;//������
    int p_index;//�������� 
    int matrix[3][3];// ������״̬
    int h_function;//����ʽ����ֵ
};
node open[SIZE]; //����Ѿ����ɵ�δ����Ľڵ�
int openlength=0;
int openlast=0;//open�����һ�����ݵ�λ��
node closed[SIZE]; //����Ѿ�������ýڵ�
int closedlength=0;
int closedlast=0;//closed�����һ�����ݵ�λ��
int fail=0;//ʧ�ܱ�ǣ�fail=1������ʧ��
int n_index=0;//�ڵ������
int extend[3][3]={0};
int n_root=0;
struct Root{
	node resultRoot[SIZE];
	int length;
};
void init(Root &r){
	r.length=0;
};
void read(){
	printf("�����ʼ�ڵ㣺\n");
	int i,j;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			scanf("%d",&start[i][j]);
		}
	}
	printf("����Ŀ��ڵ㣺\n");
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			scanf("%d",&end[i][j]);
		}
	}
	printf("\n");
}
int isEqual(int a[][3],int b[][3]){//�жϽڵ��Ƿ���Ŀ��ڵ���ͬ
	int i,j;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(a[i][j]!=b[i][j]){
				return 0;
			}
		}
	}
	return 1;
}
int arouse(int a[][3]){//��������·������������
	int distance=0;
	int i,j;
	int locate(int m[][3],int n);
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			int location=locate(end,a[i][j]);
			int i1=location/3;
			int j1=location%3;
			distance+=(int)fabs(i1-i)+(int)fabs(j1-j);
		}
	}
	return distance;
}
void copy_matrix(int a[][3],int b[][3]){
	int i,j;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			a[i][j]=b[i][j];
		}
	}
}
void inopen(int a[][3]){//�ڵ����open��ͬʱ�ڵ�����0������ָ�򸸽ڵ��ָ�룬ͬʱopenlength��openlast��һ
	copy_matrix(open[openlast].matrix,extend);//���ڶ�����������copy����һ��
	open[openlast].index=0;//����open���У����Ϊ0
	open[openlast].p_index=n_index;
	open[openlast].h_function=arouse(extend);
	openlength++;
	openlast++;
}
void cut(){//��open[0]����closed����
	closed[closedlast].index=n_index;
	closed[closedlast].p_index=open[0].p_index;
	closed[closedlast].h_function=open[0].h_function;
	copy_matrix(closed[closedlast].matrix,open[0].matrix);
	open[0].index=-1;
	openlength--;
	closedlength++;
	closedlast++;
}
int locate(int a[][3],int b){//����0����λ��
	int i,j;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(a[i][j]==b)
				return i*3+j;
		}
	}
	return -1;
}
void exchange(int *a,int *b){
	int t;
	t=*a;
	*a=*b;
	*b=t;
}
void copy(node &a,node &b){
	a.index=b.index;
	a.p_index=b.p_index;
	a.h_function=b.h_function;
	copy_matrix(a.matrix,b.matrix);
}
void adjust(){//����open���closed��
	int i;
	for(i=0;i<openlast;i++){
		if(open[i].index==-1){
			copy(open[i],open[openlast-1]);
			openlast--;
		}
	}
	for(i=0;i<closedlast;i++){
		if(closed[i].index==-1){
			copy(closed[i],closed[closedlast]);
			closedlast--;
		}
	}
}
void exchangeNode(int i,int j){
	node a;
	copy(a,open[i]);
	copy(open[i],open[j]);
	copy(open[j],a);
}
void sort(){//��open����Ԫ������������
	int i,j;
	for(i=openlength-1;i>0;i--){
		for(j=0;j<i;j++){
			if(open[j].h_function>open[j+1].h_function){
				exchangeNode(j,j+1);
			}
		}
	}
}
int seekfather(node a){
	int i;
	for(i=0;i<closedlast;i++){
		if(closed[i].index==a.p_index)
			return i;
	}
	return -1;
}
int judge(){
	int i;
	for(i=0;i<openlast;i++){
		if(open[i].index!=-1&&isEqual(open[i].matrix,extend))
			return 0;
	}
	for(i=0;i<closedlast;i++){
		if(closed[i].index!=-1&&isEqual(closed[i].matrix,extend))
			return 0;
	}
	return 1;
}
void search(){
	//��ʼ�ڵ����open��
	copy_matrix(extend,start);
	inopen(extend);
	open[0].p_index=-1;
	for(;;){
		if(openlength==0){//��open��Ϊ�գ�������ʧ�ܣ�����
			fail=1;
			return;
		}
		cut();
		if(isEqual(closed[closedlast-1].matrix,end)){//��open���е�һ���ڵ����Ŀ��ڵ㣬��ɹ�������
			return;
		}
		//��չopen[0]����		
		int now[3][3];
		copy_matrix(now,closed[closedlast-1].matrix);
		int location=locate(now,0);
		int i,j;
		i=location/3;
		j=location%3;
		copy_matrix(extend,now);
		if(i>0){//�ո�����
			int *p=&extend[i][j];
			int *q=&extend[i-1][j];
			exchange(p,q);
			if(judge()){
				inopen(extend);
			}
		}
		copy_matrix(extend,now);
		if(i<2){//�ո�����
			exchange(&extend[i][j],&extend[i+1][j]);
			if(judge()){
				inopen(extend);
			}
		}
		copy_matrix(extend,now);
		if(j>0){//�ո�����
			exchange(&extend[i][j],&extend[i][j-1]);
			if(judge()){
				inopen(extend);
			}
		}
		copy_matrix(extend,now);
		if(j<2){//�ո�����
			exchange(&extend[i][j],&extend[i][j+1]);
			if(judge()){
				inopen(extend);
			}
		}
		n_index++;
		if(n_index>10000){
			fail=1;
			return;
		}
		adjust();
		sort();
	}
}
void print_matrix(int a[][3]){
	int i,j;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			printf("%d ",a[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
void print(clock_t &start_t,clock_t &finish_t){
	finish_t=clock();
	Root result;
	init(result);
	int i=closedlast-1;
	while(closed[i].p_index!=-1){
		copy_matrix(result.resultRoot[result.length].matrix,closed[i].matrix);
		result.length++;
		i=seekfather(closed[i]);
		n_root++;
	}
	copy_matrix(result.resultRoot[result.length].matrix,start);
	int j;
	for(j=result.length;j>=0;j--){
		print_matrix(result.resultRoot[j].matrix);
	}
	printf("������ʱ%.3f����\n",difftime(finish_t,start_t));
	printf("����չ%d��,����Ϊ%d\n",closedlast,n_root);
}
int countContray(int a[][3]){
	int num=0;
	int i,j;
	int i1,i2,j1,j2;
	for(i=1;i<9;i++){
		i1=i/3;
		i2=i%3;
		for(j=0;j<i;j++){
			j1=j/3;
			j2=j%3;
			if(a[i1][i2]!=0&&a[j1][j2]!=0&&a[i1][i2]<a[j1][j2]){
				num++;
			}
		}
	}
	return num;
}
int canSolve(){
	int n_start,n_end;
	n_start=countContray(start);
	n_end=countContray(end);
	if(n_start%2==n_end%2){
		return 1;
	}
	else
		return 0;
} 
int main(){
	read();
	if(!canSolve()){
		printf("�����벻�ɽ⣡\n");
	}
	else{
		clock_t start_t,finish_t;
		start_t=clock();
		search();
		if(!fail)
			print(start_t,finish_t);
		else
			printf("search fail!\n");
	}
	return 0;
}