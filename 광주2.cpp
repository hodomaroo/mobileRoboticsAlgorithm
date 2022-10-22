#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <time.h>

#define M 30
int tmp = 0;
#define swap(a,b,area)	tmp = area[a];	area[a] = area[b];	area[b] = tmp;
#define display(area) for(int i = 0  ; i < 9 ; i++){printf("%d ", area[i]);	if(i%3==2)	printf("\n");}

int seat[3][2] = {
{0,3},
{4,1},
{7,2}
};
int link[9][4] = {0};
int map[9] = {//rocate,color
	0,1,3,
	3,0,1,
	2,0,2
};
int mapGoal[9] = {0};
int node[9] = {0};
int min = M, flg = 0;
int g_route[M] = {0}, route[M] = {0};

void Link(int p, int *str){
   memset(str,-1,sizeof(int) * 4);
   
   if(p / 3 > 0) str[0] = p-3;
   if(p % 3 > 0) str[1] = p-1;
   if(p / 3 < 2) str[2] = p+3;
   if(p % 3 < 2) str[3] = p+1;
}
void getGoal(){
	for(int i = 0 ; i < 3 ; i++){
		int y = seat[i][0] / 3;
		for(int j = 0 ; j < 3 ; j++){
			int p = y*3+j;
			if(p == seat[i][0])	continue;
			mapGoal[p] = seat[i][1];
		}
	}
}

int Sorted(){
	for(int i = 0 ; i < 9 ; i++)
		if(node[i] != mapGoal[i]) return 0;
	return 1;
}

void SortDFS(int lp, int p, int dp){
	if(dp >= min) return;

	route[dp] = p;
	if(Sorted()){
		min = dp;
		
		for(int i = 0 ; i <= min ; i++)
			g_route[i] = route[i];
		
		flg = 1;
		return;
	}
		
	for(int i = 0 ; i < 4 && !flg; i++){
		int np = link[p][i];
		if(np == lp || np < 0)	continue;
		swap(p,np,node);
		SortDFS(p,np,dp+1);
		swap(p,np,node);
	}
	return;
}

int main(){
	for(int i = 0 ; i < 9 ; i++)	Link(i, link[i]);
	clock_t startTime = clock();
	getGoal();
//	display(mapGoal);
	
	flg = 0;
	for(int i=0 ; i<M&&!flg ; i++){
		for(int j=0 ; j<3&&!flg ; j++){
			memcpy(node,map,sizeof(node));
			min = i;	
			SortDFS(-1,seat[j][0],0);
		}
	}
	printf("\n%lfÃÊ\n\n", (double)(clock() - startTime)/CLOCKS_PER_SEC);
	
	printf("min : %2d\n", min);
	for(int i = 0 ; i <= min ; i++)
		printf("%d ", g_route[i]);
}
