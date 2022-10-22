#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <time.h>

#define M 15
int tmp;
#define swap(a,b,area)	tmp = area[a];	area[a] = area[b];	area[b] = tmp;
#define display(area) for(int i = 0  ; i < 9 ; i++){printf("%d ", area[i]);	if(i%3==2)	printf("\n");}	printf("\n");

int link[20][4] = {0};
int subMap[25] = {
	1,0,2,
	0,2,0,
	2,0,1
};
int node[20] = {0};
int sort_min = 0;
int sort_route[20][2] = {0}, g_sort_route[20][2] = {0};//시작위치에 퍽이 없으면 앞을로 미는거, 퍽이 있으면 옆으로 미는거 
 
int flg = 0, color[2] = {0};

void set_color(){
	int blue = 0;
	for(int i = 0 ; i < 9 ; i++){
		if(subMap[i] == 1)	blue++;
	}
	if(blue == 3){color[0] = 1;	color[1] = 2;}
	else{color[0] = 2;	color[1] = 1;}
}

void subLink(int p, int *str){
   memset(str,-1,sizeof(int) * 4);
   
   if(p / 3 > 0) str[0] = p-3;
   if(p % 3 > 0) str[1] = p-1;
   if(p / 3 < 2) str[2] = p+3;
   if(p % 3 < 2) str[3] = p+1;
}

int max_check(){
	for(int i = 0 ; i < 3 ; i++){
		if(node[i] == color[0] && node[i+3] == color[0] && node[i+6] == color[0])	return 1;
		if(node[i*3] == color[0] && node[i*3+1] == color[0] && node[i*3+2] == color[0])	return 1;
	}	
	return 0;
}

int min_check(){
	int surrund[2] = {0};
	for(int i = 0 ; i < 9 ; i++){
		if(node[i] != color[1])	continue;
		for(int j = 0 ; j < 4 ; j++){
			int p = link[i][j];
			if(p == -1)	continue;
			int col = node[p];
			
			if(col == color[0])	surrund[0] = 1;
			if(col == color[1])	surrund[1] = 1;
		}
		if(!surrund[1])	return 0;
	}
	return surrund[0] && surrund[1];
}

int subSorted(){
	if(max_check() && min_check())	return 1;
	return 0;
}

int getSubGoal(int p, int dir, int dis){
	int y = p/3, x = p%3;
	switch(dir){
		case 0:
			if(y-dis < 0 || node[p-3*dis])	return -1;
			return p-3*dis;
		case 1:
			if(x-dis < 0 || node[p-dis])	return -1;
			return p-dis;
		case 2:
			if(y+dis > 2 || node[p+3*dis])	return -1;
			return p+3*dis;
		case 3:
			if(x+dis > 2 || node[p+dis])	return -1;
			return p+dis;
	}
	return -1;
}

void subSortDFS(int dp){
	if(dp >= sort_min) return;

	if(subSorted()){
		sort_min = dp;
		
		for(int i = 0 ; i < sort_min ; i++){
			g_sort_route[i][0] = sort_route[i][0];
			g_sort_route[i][1] = sort_route[i][1];
		}	
		flg = 1;
		return;
	}
	

	for(int i = 0 ; i < 9 && !flg ; i++){	//옮길 퍽 위치 	
		if(!subMap[i]) continue;
		for(int j = 0 ; j < 4 && !flg; j++){	//i 위치 기준 4방향 
			if(link[i][(j+2)%4] < 0)	continue;
			int robot_point = link[i][(j+2)%4];
			if(node[robot_point])	continue;
			for(int k = 1 ; k <= 2 ; k++){
				int goal = getSubGoal(i, j, k);
				if(goal == -1)	break;
				
				sort_route[dp][0] = robot_point;
				sort_route[dp][1] = goal;
				
				swap(i,goal,node);
				subSortDFS(dp+1);
				swap(i,goal,node);
			}
		}
		
		for(int j = 0 ; j < 4 && !flg; j++){	//i 위치 기준 4방향 
			if(i/3 == 1 && j%2 == 1)	continue;
			if(i%3 == 1 && j%2 == 0)	continue;
			for(int k = 1 ; k <= 2 ; k++){
				int goal = getSubGoal(i, j, k);
				if(goal == -1)	break;
				
				sort_route[dp][0] = i;
				sort_route[dp][1] = goal;
				
				swap(i,goal,node);
				subSortDFS(dp+1);
				swap(i,goal,node);
			}
		}
	}
}

int main(){
	clock_t startTime = clock();
	set_color();
	for(int i = 0 ; i < 9 ; i++)
		subLink(i, link[i]);
		
	flg = 0;	
	for(int i = 1 ; i < M && !flg ; i++){
		memcpy(node,subMap,sizeof(subMap));
		sort_min = i;
		subSortDFS(0);
	}
	
	printf("\n%lf초\n\n", (double)(clock() - startTime)/CLOCKS_PER_SEC);
	
	printf("flg : %d\n", flg);
	for(int i = 0 ; i < sort_min ; i++){
		printf("%2d - %2d\n", g_sort_route[i][0], g_sort_route[i][1]);
	}
}
