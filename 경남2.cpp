#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <time.h>

#define M 15
int tmp;
#define swap(a,b,area)	tmp = area[a];	area[a] = area[b];	area[b] = tmp;
#define display(area) for(int i = 0  ; i < 20 ; i++){printf("%d ", area[i]);	if(i%4==3)	printf("\n");}

//int seat[3] = {2,7,8};
int link[20][4] = {0};
int map[20] = {
	0,0,0,2,
	2,0,1,0,
	0,1,3,2,
	0,1,0,0,
	0,0,0,0
};
int mapGoal[20] = {0};
int node[20] = {0}, check[20] = {0};
int min = M, sort_min = 0;
int g_route[20] = {0}, route[20] = {0}, sort_route[20][2] = {0}, g_sort_route[20][2] = {0};
int flg = 0;

void Link(int p, int *str){
   memset(str,-1,sizeof(int) * 4);
   
   if(p / 4 > 0) str[0] = p-4;
   if(p % 4 > 0) str[1] = p-1;
   if(p / 4 < 4) str[2] = p+4;
   if(p % 4 < 3) str[3] = p+1;
}
void DFS(int p, int wp, int dp){
	if(dp >= min) return;
	route[dp] = p;
	
	if(p == wp){
		min = dp;
		memcpy(g_route,route,sizeof(route));
		return;
	}
	
	check[p] = 1;
	
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np < 0 || (node[np] && np != wp) || check[np]) continue;
		DFS(np,wp,dp+1);
	}
	check[p] = 0;
}

int checkDFS(int p, int wp){
	memset(check,0,sizeof(check));
	min = M;
	DFS(p,wp,0);
	return min != M;
}

int Sorted(){
	for(int i = 0 ; i < 20 ; i++){
		if(!node[i]) continue;
		if(node[i] != mapGoal[i]) return 0;
	}
	return 1;
}
int getStart(int p, int dir){
	int y = p/4, x = p%4;
	switch(dir){
		case 0:
			if(y+1 > 5)	return -1;
			return p+4;
		case 1:
			if(x+1 > 4)	return -1;
			return p+1;
		case 2:
			if(y-1 < 0)	return -1;
			return p-4;
		case 3:
			if(x-1 < 0)	return -1;
			return p-1;
	}
	return -1;
}
int getGoal(int p, int dir, int dis){
	int y = p/4, x = p%4;
	switch(dir){
		case 0:
			if(y-dis < 0 || node[p-4*dis])	return -1;
			return p-4*dis;
		case 1:
			if(x-dis < 0 || node[p-dis])	return -1;
			return p-dis;
		case 2:
			if(y+dis > 4 || node[p+4*dis])	return -1;
			return p+4*dis;
		case 3:
			if(x+dis > 3 || node[p+dis])	return -1;
			return p+dis;
	}
	return -1;
}

void SortDFS(int dp, int robotpoint){
	if(dp >= sort_min) return;

	if(Sorted()){
		sort_min = dp;
		
		for(int j = 0 ; j < sort_min ; j++)
			for(int i = 0 ;i < 2 ; i++)
				g_sort_route[j][i] = sort_route[j][i];
		
		flg = 1;
		return;
	}
	
	for(int i = 16 ; i < 20 ; i++) 
		if(node[i])	return;
	if(!mapGoal[0] && node[0])	return;
	if(!mapGoal[3] && node[3])	return;
	
	for(int i = 0 ; i < 16 && !flg ; i++){	//옮길 퍽 위치 
		if(node[i] != 1 || node[i] == mapGoal[i]) continue;
		
		for(int j = 0 ; j < 4 && !flg; j++){	//i 위치 기준 4방향 
			for(int k = 1 ; k <= 4 ; k++){
				int start = getStart(i, j);
				if(start == -1 || node[start] || !checkDFS(robotpoint, start))	break;
				int goal = getGoal(i, j, k);
				if(goal == -1)	break;
				
				sort_route[dp][0] = start;
				sort_route[dp][1] = goal;
				min = 5;
				memset(check,0,sizeof(check));
				DFS(start,goal,0);
				swap(i,goal,node);
				SortDFS(dp+1,g_route[min-1]);
				swap(i,goal,node);
			}
		}
	}
	return;
}

int main(){
	
	for(int i = 0; i < 20; i++){
		if(map[i] == 2){
			mapGoal[i] = 1;
			map[i] = 0;
		}
		if(map[i] == 3)	mapGoal[i] = 3;
	}
	
	for(int i = 0 ; i < 20 ; i++)
		Link(i, link[i]);
	
	clock_t startTime = clock();
	for(int i = 3 ; i <= 10 && !flg; i++){
		memcpy(node,map,sizeof(node));
		sort_min = i;	
		SortDFS(0, 16);
	
	}
	
	printf("\n%lf초\n\n", (double)(clock() - startTime)/CLOCKS_PER_SEC);
	
	for(int i = 0 ; i < sort_min ; i++){
		printf("%2d %2d\n", g_sort_route[i][0], g_sort_route[i][1]);
	}
}
