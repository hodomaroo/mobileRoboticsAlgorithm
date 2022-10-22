#include <stdio.h>
#include <windows.h>
#include <time.h> 

int line[3] = {1,8,6};
int seq[3] = {1,2,3};//시트지 순서 
int tmp = 0, flg = 0;
#define swap(a,b,str)	tmp = str[a]; str[a] = str[b];	str[b] = tmp;
#define display(area)	for(int i = 0 ; i < 3 ; i++){for(int j = 0 ; j < 5 ; j++)	printf("%d ", area[i*5+j]);	printf("\n");}
#define M 30
int way[M][2] = {0}, way_cnt = 0;
int route[M][2] = {0}, min=0;
int map[15] = {
	3,1,2,3,0,	// 0  1  2  3  4
	1,2,1,3,0,	// 5  6  7  8  9 
	2,1,2,3,0	//10 11 12 13 14
};
int mapGoal[15] = {0}, node[15] = {0};
int target[15] = {0};

int variable(){
	if(map[3] == 3 && map[8] == 3 && map[13] == 3)	return 1;
	return 0;
}

int sort(){
	int max = 0, min = 0, mid = 0;
	for(int i = 1 ; i < 3 ; i++){
		if(line[i] > line[max])	max = i;
		if(line[i] < line[min])	min = i;
	}
	mid = 3-min-max;
	return max*100+mid*10+min;
}

void make_mapGoal(int n, int *sample){	
	int cnt = 0;
	for(int i = 0 ; i < 3; i++){
		int nc = seq[i];
		for(int j = 1 ; j < sample[0] ; j++){
			if(sample[j] != nc)	continue;
			mapGoal[n*5 + cnt++] = sample[j];
		}
	}
	
}

void set_target(int *col){
	printf("%d %d %d", col[1], col[2], col[3]);
	for(int i = 3 ; i >= 0 ; i--){
		for(int j = 0 ; j < 3 ; j++){
			int p = j*5+i;
			int color = map[p];
			if(col[color] == 0)	continue;
			col[color]--;
			target[p] = 1;
		}
	}
	return;
}

void set_mapGoal(){	
	int turn[3] = {0}, cnt[3] = {0}, col[4] = {0,4,4,4};
	int sample_map[3][5] = {{1},{1},{1}};
	int bit = sort(), color = 3;
	for(int i = 2 ; i >= 0 ; i--){
		turn[i] = bit%10;
		bit/=10;
	}
//	if(variable())col[3] -= 1;
	//---초기 세팅  
	for(int m = 0 ; m < 6 ; m++){
		for(int i = 0 ; i < 3 ; i++){
			if(col[color]<=0)	break;
			int myturn = turn[i];
			if(line[myturn] < cnt[myturn] + color)	continue;
			cnt[myturn] += color;
			col[color]--;
			sample_map[myturn][sample_map[myturn][0]++] = color;
		}
		if(m%2==1)	color--;
		if(color <= 0)	break;
	}
	
	for(int i = 0 ; i < 3 ; i++)
		make_mapGoal(i, sample_map[i]);
	
	set_target(col);
	return;
}

int outside(int n, int *area){
	for(int i = 0 ; i < 5 ; i++)
		if(!area[n*5+i])	return n*5+i;
	return -1;
}

int inside(int n, int *area){
	for(int i = 4 ; i >= 0 ; i--)
		if(area[n*5+i])	return n*5+i;
	return -1;
}

int find_bang(int n){
	int bang_point = -1;
	for(int i = 0 ; i < 3 ; i++){
		if(i == n)	continue;
		bang_point = outside(i, map);
		if(bang_point > -1)	return bang_point;
	}
}

void clear(int p){
	int x = p % 5, y = p/5;
	for(int i = 4 ; i > x ; i--){
		int np = y*5+i;
		if(!map[np])	continue;
		way[way_cnt][0] = np;
		way[way_cnt++][1] = find_bang(y);
		swap(way[way_cnt-1][0],way[way_cnt-1][1],map); 
	}
}

void throw_away(){
	for(int m = 4 ; m >= 0 ; m--){
		for(int i = 0 ; i < 3 ; i++){
			for(int j = m ; j >= 0 ; j--){
				int p = i*5+j;
				if(target[p] == 0)	break;
				if(map[p] == 0)	continue;
				int col = map[p];
				clear(p);
				map[p] = 0;
				target[p] = 0;
				way[way_cnt][0] = p;
				way[way_cnt++][1] = -col;
			}
		}
	}	
}

int check(){
	for(int i = 0 ; i < 3 ; i++)
		for(int j = 0 ; j < 4 ; j++)
			if(node[i*5+j] != mapGoal[i*5+j])
				return 0;
	return 1;
}
int plan_B_check(int n){
	for(int i = 0 ; i < 3 ; i++){
		int p = n+i*5;
		if(mapGoal[p] == 0)	continue;
		if(node[p] == mapGoal[p] && !target[p])
			return 1;
	}
	return 0;
}
int break_for(int n){
	for(int i = 0 ; i < 3 ; i++){
		if(mapGoal[n+i*5] == 0)	continue;
		if(node[n+i*5] != mapGoal[n+i*5])
			return 0;
	}
	return 1;
}

void sort(int lp, int dp, int mode){
	if(dp >= min)	return;
	if(mode == -1 ? check() : plan_B_check(mode)){
		min = dp;
		flg = 1;
		for(int i = 0 ; i < min ; i++){
			way[i][0] = route[i][0];
			way[i][1] = route[i][1];
		}
		return;
	}	
	
	for(int i = 0 ; i < 9 ; i++){
		int st = inside(i%3,node), goal = outside(i/3,node);
		if(st == lp || st/5 == goal/5 || st == -1 || goal == -1)	continue;
		if(mode >= 0 && target[st])	continue;
		
		route[dp][0] = st;
		route[dp][1] = goal;
		
		swap(st,goal,node);
	/*	display(node);
		printf("\n");*/
		sort(goal,dp+1,mode);
		if(flg)	return;
		swap(st,goal,node);
	}
}
void plan_B(){
	for(int i = 0 ; i < 15 ; i++){
		node[i] = map[i];
		target[i] = 0;
	}
	for(int m = 0 ; m < 5 ; m++){
		
		for(int i = 0 ; i < 3 ; i++)	
			if(mapGoal[m+i*5] && mapGoal[m+i*5] == node[m+i*5])	target[m+i*5] = 1;
			
		for(int n = 0 ; n < 3 ; n++){
			printf("\n==== %d - %d ====\n", m, n);
			printf("map\n");
			display(node);
			printf("mapGoal\n");
			display(mapGoal);
			printf("target\n");
			display(target);
			printf("\n");
			flg = 0;
			for(int i = 1 ; i < M ; i++){
				min = i;
				sort(-1, 0, m);
				if(flg)	break;
			}
			for(int i = 0 ; i < min ; i++)
				printf("%2d %2d \n", way[i][0], way[i][1]);
				
			for(int i = 0 ; i < 3 ; i++)
				if(mapGoal[m+i*5] && mapGoal[m+i*5] == node[m+i*5])	target[m+i*5] = 1;
			if(break_for(m))	break;
			if(check())	return;
		}
		if(check())	return;
	}
	if(check())	return;
}

main(){
	clock_t startTime = clock();
	set_mapGoal();
	
	printf("\n==========================\n");
	
	for(int i = 0 ; i < 3 ; i++){
		printf("%d : ", i);
		for(int j = 0 ; j < 5 ; j++)
			printf("%d ", target[i*5+j]);
		printf("\n");
	}
	display(mapGoal);
	throw_away(); 
	
	printf("\n==========================\n");
	
	for(int i = 0 ; i < way_cnt ; i++)
		printf("%2d %2d \n", way[i][0], way[i][1]);
	printf("\n==========================\n");
	
	
	for(int i = 0 ; i < 15 ; i++){
		node[i] = map[i];
		target[i] = 0;
	}
	printf("map\n");
	display(node);
	printf("mapGoal\n");
	display(mapGoal);

	flg = 0;
	for(int i = 1 ; i <= 10 ; i++){
		min = i;
		sort(-1, 0, -1);
		if(flg)	break;
	}
	
	if(flg){				
		for(int i = 0 ; i < min ; i++){
			printf("%2d %2d \n", way[i][0], way[i][1]);		
		/*	swap(way[i][0], way[i][1], map);
			display(map);	
			printf("\n");*/
		}
		
	}else{
		plan_B();
	}
	
	printf("\n%lf\n", (double)(clock()-startTime)/CLOCKS_PER_SEC);
}
