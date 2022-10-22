#include<stdio.h>
#include<memory.h>
#include<math.h>
#include<windows.h>
#include <time.h>

int tmp;
#define swap(a,b,str)	tmp = str[a];	str[a] = str[b];	str[b] = tmp;
#define M 30
#define display	for(int i = 0 ; i < 12 ; i++){printf("%d ", map[i]);	if(i%4 == 3)	printf("\n");}	printf("\n");	

int mapGoal[12] = {
	1,3,2,0,
	2,-1,1,3,
	2,3,1,1
};
int node[12] = {0}, check[12] = {0};
int link[12][4] = {0};
int map[12] = {
	3,0,2,3,
	1,-1,1,0,
	3,2,0,2
};
int block = 5, sort_min, min, flg = 0, max_min, matchPuck;
int g_route[M] = {0}, route[M] = {0}, g_sort_route[M][2] = {0}, sort_route[M][2] = {0};

void Link(int p, int *str){
   memset(str,-1,sizeof(int) * 4);
   
   if(p / 4 > 0) str[0] = p-4;
   if(p % 4 > 0) str[1] = p-1;
   if(p / 4 < 2) str[2] = p+4;
   if(p % 4 < 3) str[3] = p+1;
}

void DFS(int p, int goal, int dp){
	if(dp >= min)	return;
	
	route[dp] = p;
	if(p == goal){
		min = dp;
		memcpy(g_route,route,sizeof(route));
		return;
	}
	
	check[p] = 1;
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np < 0 || check[np] || np == block || (np!=goal && map[np]))	continue;
		DFS(np,goal,dp+1);
	}
	check[p] = 0;
}

int checkDFS(int start, int goal){
	min = M;
	memset(check,0,sizeof(check));
	DFS(start,goal,0);
	return M!=min;
}

int GetPuckPoint(int* getPos, int *count, int p){
	*count = 0;

	int q[12], chk[12] = { 0 };
	int head = 0, tail = 0;

	q[tail++] = p;
	chk[p] = 1;

	while (head < tail) {
		int node = q[head++];

		for (int i = 0; i < 4; i++) {
			int next = link[node][i];
			if (next >= 0 && !chk[next] && next != block)
			{
				chk[next] = 1;
				if (!map[next])
					q[tail++] = next;
				else if (map[next] > 0)
					getPos[(*count)++] = next;
			}
		}
	}
}

int PutPuckPoint(int* putPos, int *count, int p){
	*count = 0;

	int q[12], prevQ[12], chk[12] = { 0 };
	int head = 0, tail = 0;

	q[tail++] = p;
	chk[p] = 1;

	while (head < tail) {
		int node = q[head++];

		int flag = 0;
		for (int i = 0; i < 4; i++) {
			int next = link[node][i];
			if (next >= 0 && !chk[next] && !map[next] && next != block)
			{
				q[tail++] = next;

				chk[next] = 1;
				flag = 1;
			}
		}

		if ((!flag || map[p] == mapGoal[node]) && node != p) {
			putPos[(*count)++] = node;
		}
	}
}

int matchCount(){
	int cnt = 0;
	for(int i = 0 ; i < 12 ; i++){
		if(map[i] == 0 || block == i)	continue;
		if(map[i] == mapGoal[i])	cnt++;
	}
	return cnt;
}

void Sort(int lp, int robot, int dp){
	if(dp >= max_min)	return;
	
	int match = matchCount();
	if(match > matchPuck){
		printf("--%d--\n", match);
	//	display;
	//	system("pause");
		matchPuck = match;
		if(match == 8)	flg = 1;
		for(int i = 0 ; i < dp ; i++)
			for(int j = 0 ; j < 2 ; j++)
				g_sort_route[i][j] = sort_route[i][j];
		sort_min = dp;
	}
	
	
	int puck[12] = {0},	puck_cnt = 0;
	GetPuckPoint(puck, &puck_cnt, robot);
	for(int i = 0 ; i < puck_cnt && !flg ; i++){
		int start = puck[i];
		if(start == lp)	continue;
		int bang[12] = {0}, bang_cnt = 0;
		PutPuckPoint(bang, &bang_cnt, start);
		for(int j = 0 ; j < bang_cnt && !flg ; j++){
			int goal = bang[j];
			if(start == goal)	continue;
			
			sort_route[dp][0] = start;
			sort_route[dp][1] = goal;
			
			checkDFS(start,goal);
			swap(start,goal,map);
			Sort(goal, g_route[min-1], dp+1);
			swap(start,goal,map);
		}
	}
}

int main(){
	for(int i = 0 ; i < 12 ; i++)
		Link(i, link[i]);

	clock_t startTime = clock();
	flg = 0;
	matchPuck = 0;
	
		max_min = 10;
		Sort(-1, 1, 0);
		
/*	memcpy(node,map,sizeof(map));
	for(int i = 0 ; i < 15 && !flg ; i++){
		memcpy(map,node,sizeof(map));
		max_min = i;
		Sort(-1, 2, 0);
		printf("Sort %d End\n", i);
	}*/
	
	printf("%lfÃÊ\n\n", (double)(clock() - startTime)/CLOCKS_PER_SEC); 
	
	printf("min : %d\n", sort_min);
	for(int i = 0 ; i < sort_min ; i++)
		printf("%2d -> %2d\n", g_sort_route[i][0], g_sort_route[i][1]);

}
