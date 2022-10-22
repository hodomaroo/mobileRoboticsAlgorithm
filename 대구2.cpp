#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <time.h>

#define M 30
int tmp = 0;
#define swap(a,b,area)	tmp = area[a];	area[a] = area[b];	area[b] = tmp;
#define display(area) for(int i = 0  ; i < 18 ; i++){printf("%d ", area[i]);	if(i%3==2)	printf("\n");}printf("\n");

//int line[6] = {0,3,1,2,0,0};
int line[6] = {0,3,1,2,0,0};
int link[18][4] = {0};
int way[5][2] = {
	0,1,
	1,0,
	0,1,
	1,0,
	0,1
};
int map[18] = {
	0,2,0,
	0,1,0,
	0,3,0,
	0,2,0,
	0,1,0,
	0,3,0
};
int mapGoal[18] = {0}, storage[18] = {0}, side[18] = {0};
int node[18] = {0}, check[18] = {0};
int min = 0, flg = 0, sort_min = 0;
int g_route[M] = {0}, route[M] = {0}, g_sort_route[M][2], sort_route[M][2];

void getGoal(){
	for(int i = 0 ; i < 6 ; i++){
		storage[i*3+1] = 1;
		if(!line[i])	continue;
		for(int j = 0 ; j < 3 ; j++){
			storage[i*3+j] = 1;
			if(j != 1)	mapGoal[i*3+j] = line[i];
		}
	}
}

void wing(){
	for(int i = 0 ; i < 5 ; i++){
		for(int j = 0 ; j < 2 ; j++){
			if(!way[i][j])	continue;
			link[i*3+j*2][2] = i*3+j*2+3;
			link[i*3+j*2+3][0] = i*3+j*2;
		}
	}
}

void Link(int p, int *str){
	memset(str,-1,sizeof(int) * 4);

	if(p % 3 > 0) str[1] = p-1;
	if(p % 3 < 2) str[3] = p+1;
	if(p%3 == 1){
		if(p / 3 < 5) str[2] = p+3;
		if(p / 3 > 0) str[0] = p-3;
	}
}

void setSide(){
	for(int i = 0 ; i < 18 ; i++)
		for(int j = 0 ; j < 4 ; j++)
			if(link[i][j] > -1)	side[i]++;
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

int front(int p){
	int end = p/3;
	if(p%3 != 1)	end+=1;
	for(int i = 0 ; i < end ; i++)
		if(!node[i*3+1])	return checkDFS(p,i*3+1);
	return 0;
}

int back(int p){
	int end = p/3;
	if(p%3 != 1)	end-=1;
	for(int i = 5 ; i > end ; i--)
		if(!node[i*3+1])	return checkDFS(p,i*3+1);
	return 0;
}

int findGoal(int col, int p){
	for(int i = 0 ; i < 6 ; i++){
		if(line[i] != col)	continue;
		for(int j = 0 ; j < 2 ; j++){
			int goal = i*3+j*2;
			if(goal == p)	continue;
			if(!node[goal] && checkDFS(p, goal))	return 1;
		}
		break;
	}
	return 0;
}

int findTempGoal(int col, int p){
	int x[3] = {0,2,1};
	for(int i = 0 ; i < 6 ; i++){
		if(line[i] == col)	continue;
	//	if(i < p/3 && node[i*3+1])	continue;
		for(int j = 0 ; j < 2 ; j++){
			int goal = i*3 + x[j];
			if(p == goal || !storage[goal] || node[goal] || !checkDFS(p,goal))	continue;
			return 1;
		}
	}
	return 0;
}

int Sorted(){
	for(int i = 0 ; i < 18 ; i++)
		if(node[i] != mapGoal[i]) return 0;
	return 1;
}

void SortDFS(int lp, int robot_point, int dp){
	if(dp >= sort_min || side[robot_point] <= 1) return;
//	display(node);
	if(Sorted()){
		sort_min = dp;
	
		for(int i = 1 ; i < sort_min ; i++){
			g_sort_route[i][0] = sort_route[i][0];
			g_sort_route[i][1] = sort_route[i][1];
		}
		flg = 1;
		return;
	}
	
	for(int m = 0 ; m < 3 ; m++){
//		printf("============= %d ==============\n", m);
		for(int i = 0 ; i < 36 && !flg; i++){//출발지점 
			if(i/18 == 0 && i%3 != 1)	continue;
			if(i/18 == 1 && i%3 == 1)	continue;
			int start = i%18, goal;
			if(start == lp || !node[start] || node[start] == mapGoal[start] || !checkDFS(robot_point,start))	continue;
			sort_route[dp][0] = start;
			if((m == 0 && findGoal(node[start], start)) || (m == 1 && findTempGoal(node[start], start)) || (m == 2 && front(start)) || (m == 2 && back(start))){
				goal = g_route[min];
				sort_route[dp][1] = goal;
				swap(start,goal,node);
//				printf("robot : %2d, lp : %2d, start : %2d, goal : %2d, dp : %2d\n", robot_point, lp, start, goal, dp); 
//				display(node);
				SortDFS(goal,g_route[min-1],dp+1);
				swap(start,goal,node);
			//	if(m == 0)	return;
			//	if(m == 0)	m+=1;
			}
		}
	}
	return;
}

int main(){	
	for(int i = 0 ; i < 18 ; i++)	Link(i, link[i]);
	wing();
	getGoal();
	setSide();
	
	memcpy(node,map,sizeof(node));//초기 세팅 
//	display(storage);
	
	clock_t startTime = clock();
	for(int i = 0 ; i < 18 ; i++){
		if(!storage[i] || node[i] || !checkDFS(1,i))	continue;
		g_sort_route[0][0] = 1;
		g_sort_route[0][1] = i;
		
		swap(1,i,map);
		break;
	}
	
	flg = 0;
	int lp = -1;
	int now = 1;
	for(int i = 9 ; i < 20 && !flg ; i+=10){
		memcpy(node,map,sizeof(node));
		sort_min = i;	
		SortDFS(lp,now,0);
	}
	printf("\n%lf초\n\n", (double)(clock() - startTime)/CLOCKS_PER_SEC);
	
	for(int i = 0 ; i < sort_min ; i++)
		printf("%d %d\n", g_sort_route[i][0], g_sort_route[i][1]);
}
