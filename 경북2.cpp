#include <stdio.h>
#include <memory.h>
#include <windows.h>

int tmp;
#define swap(a,b,str) tmp = str[a]; str[a] = str[b]; str[b] = tmp;
#define M 20
#define display	for(int i = 0 ; i < 32 ; i++){	printf("%d ", map[i]);	if(i%4 == 3)	printf("\n");}	system("pause");	system("cls");
int robot_point=0;
int map[32] = {
	0,0,0,0,
	0,1,0,3,
	0,2,0,0,
	0,2,2,0,
	0,0,0,0,
	
	0,0,0,0,
	1,0,0,2,
	1,0,0,1,
};

int mapGoal[20] = {0};
int color[3] = {0};
int gate = 16;
int g_route[20], route[20], check[32], min = 20;
int link[32][4];

int usedAllColor(){ // setMapGoal exit condition.
	for(int i = 1; i < 3; i++)
		if(!color[i]) return 0;
	return 1;
}

int getColor(){ // Get color.
	for(int i = 1; i < 3; i++){
		if(color[i]) continue;
		color[i] = 1;
		return i;
	}
}

int isBlank(int idx){ // Find empty space.
	int cube[4] = {idx,idx+1,idx+4,idx+5};
	for(int i = 0; i < 4; i++)
		if(mapGoal[cube[i]]) return 1;
	return 0;
}
	
void setMapGoal(){
	for(int i = 14; i >= 0; i--){
		if(usedAllColor()) return;
		if(mapGoal[i] || i % 4 == 3) continue; // Yellow puck skip || Out of range skip.
		if(isBlank(i)) continue; // None of the four spaces are empty.
		
		int cube[4] = {i,i+1,i+4,i+5}; // Set the square position. 
		int colorValue = getColor();
		for(int j = 0; j < 4; j++)
			mapGoal[cube[j]] = colorValue;
	}
}

void Link(int p, int *str){
	memset(str, -1, sizeof(int) * 4);
	if(p / 4 > 0) str[0] = p-4;
	if(p % 4 > 0) str[1] = p-1;
	if(p / 4 < 7) str[2] = p+4;
	if(p % 4 < 3) str[3] = p+1;
	
	if(p / 4 == 4 && gate != p) str[2] = -1;
	if(p / 4 == 5 && gate != p-4) str[0] = -1;
}
//--------------------
void scan_B(){
	for(int i = 0 ; i < 4 ; i++){
		//Scan
		//Scan
		if(map[24+i] && !map[28+i]){
			map[28+i] = map[24+i];
			map[24+i] = 0;
		//swap(24+i, 28+i, map);
		}
	}
}

int find_B_goal(int col){
	for(int i = 24 ; i < 28 ; i++)
		if(map[i+4] == col && !map[i])	return i;	//가능하면 한 줄에 같은색이 있도록 
		
	for(int i = 31 ; i >= 24 ; i--)					//일단 B구역에 배치 
		if(!map[i])	return i;
}

void DFS(int p, int goal, int dp){
	if(dp >= min)	return;
	route[dp] = p;
	
	if(p == goal){
		min = dp;
		for(int i = 0 ; i <= min ; i++)
			g_route[i] = route[i];
		return;
	}
	
	check[p] = 1;
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np == -1 || check[np] || map[np] == 3 || (map[np] && np != goal))	continue;
		DFS(np,goal,dp+1);
	}
	check[p] = 0;
}
int checkDFS(int p, int goal){
	min = M;
	DFS(p,goal,0);
	return min!=M;
}
int find_col(int col){
	for(int i = 20 ; i < 32 ; i++){
		if(map[i] != col)	continue;
		if(checkDFS(i,gate))	return i;
	}
	return -1;
}
int surround(int p, int col){
//	printf("wc : %d\n", col);
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np >= 0 && map[np] == col)	return 1;
	}
	return 0;
}

void clean(int wantCol){
	for(int i = 0 ; i < 32 ; i++){
		printf("%d ", map[i]);
		if(i%4 == 3)	printf("\n");
	}
	int p, side[5] = {23,28,31,24,27};
	for(int i = 0 ; i < 8 ; i++){
		if(i %4 == 3)	continue;
		p = 24+i;
   	 	if(map[p] && map[p] != wantCol && surround(p, wantCol) && checkDFS(gate,p))	break;
   	 	if(i == 6)	p = 27;
	}
	
/*	for(int i = 0 ; i < 6 ; i++){
   	 	p = i%3+24+4*(i/3);
   	 	if(map[p] && map[p] != wantCol && surround(p, wantCol) && checkDFS(gate,p))	break;
   	 	if(i == 5)	p = 27;
	}*/
	
//	printf("P:%d\n", p);

	for(int i = 0 ; i < 5 ; i++){
		int goal = side[i];
		if(p%4 == goal%4 || map[goal] || !checkDFS(p,goal))	continue;
		swap(p,goal,map);
		printf("\n===================================\n");
		for(int j = 0 ; j <= min ; j++)
			printf("%d ", g_route[j]);
		printf("\n===================================\n");
		break;
	}
	for(int i = 0 ; i < 32 ; i++){
		printf("%d ", map[i]);
		if(i%4 == 3)	printf("\n");
	}
	system("pause");
}

int check_end(){
	for(int i = 0 ; i < 20 ; i++)
		if(map[i] != mapGoal[i])	return 0;
	return 1;
}

void step1(){
	int q[25], prevQ[25], chk[20] = { 0 };
	int head = 0, tail = 0;

	q[tail++] = gate;
	chk[gate] = 1;
	
	while (head < tail) {
		int point = q[head++];
		for (int i = 0; i < 4; i++) {
			int next = link[point][i];
			if(next < 20 && next >= 0 && !chk[next] && map[next]!=3){
				chk[next] = 1;
				q[tail++] = next;
			}
		}
	}
	
	for(int i = 0 ; i < tail ; i++){
		int nowPoint = q[i];
		if(map[nowPoint] == 0 || map[nowPoint] == 3)	continue;
		int goal = find_B_goal(map[nowPoint]);
		checkDFS(nowPoint,goal);
		display;
		for(int j = 0; j <= min; j++)
			printf("%d ", g_route[j]);
		printf("\n");
		swap(nowPoint,goal,map);
	}
	printf("END_STEP1\n");
}

void step2(){
	int q[25], prevQ[25], chk[20] = { 0 };
	int head = 0, tail = 0;

	q[tail++] = gate;
	chk[gate] = 1;
	
	while (head < tail) {
		int point = q[head++];
		for (int i = 0; i < 4; i++) {
			int next = link[point][i];
			if(next < 20 && next >= 0 && !chk[next] && map[next]!=3){
				chk[next] = 1;
				q[tail++] = next;
			}
		}
	}
	
	for(int i = tail-1 ; i >= 0 ; i--){
		int re = 0;
		retry:
		int nowPoint = q[i];
		if(mapGoal[nowPoint] == 0 || mapGoal[nowPoint] == 3)	continue;
		int p = find_col(mapGoal[nowPoint]);
		if(p == -1){
			clean(mapGoal[nowPoint]);
			re++;
			if(re<4)	goto retry;
		}
		if(p == -1) continue;
		display;
		min = M;
		DFS(p, nowPoint, 0);
		swap(p,nowPoint,map);
		for(int j = 0 ; j <= min ; j++)
			printf("%d ", g_route[j]);
		printf("\n");
		
		if(check_end())	return;
	}
}

//--------------------
int main(){
	scan_B();
	for(int i = 0; i < 32; i++)
		Link(i, link[i]);
	
	for(int i = 0 ; i < 20 ; i++)
		if(map[i] == 3) mapGoal[i] = 3;
	setMapGoal();
	
	step1();
	//end_step 1
	step2();
	//end_step 2
	printf("final map\n");
	for(int i = 0 ; i < 32 ; i++){
		printf("%d ", map[i]);
		if(i%4 == 3)	printf("\n");
	}
	printf("\ngoal map\n");
	for(int i = 0 ; i < 20 ; i++){
		printf("%d ", mapGoal[i]);
		if(i%4 == 3)	printf("\n");
	}
}
