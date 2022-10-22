#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

//#define R 1
//#define B 2
//#define Y 3

int link[13][8] = {
	-1,-1, 1, 3,-1,-1,-1,-1, // 0
	-1,-1, 2, 4,-1, 3, 0,-1, // 1
	-1,-1,-1,-1,-1, 4, 1,-1, // 2
	-1, 1, 4, 6,-1, 5,-1, 0, // 3
	-1, 2,-1, 7,-1, 6, 3, 1, // 4
	-1, 3, 6, 8,-1,-1,-1,-1, // 5
	-1, 4, 7, 9,-1, 8, 5, 3, // 6
	-1,-1,-1,-1,-1, 9, 6, 4, // 7
	-1, 6, 9,11,-1,10,-1, 5, // 8
	-1, 7,-1,12,-1,11, 8, 6, // 9
	-1, 8,11,-1,-1,-1,-1,-1, // 10
	-1, 9,12,-1,-1,-1,10, 8, // 11
	-1,-1,-1,-1,-1,-1,11, 9  // 12
};
int goal[13] = {
	1, 2, 3,
	 2, 3,
	1, 2, 2,
	 3, 3,
	1, 2, -1
};
int map[13] = {
	2, 3, 2,	// 0 1 2
	 3, 0,		//  3 4
	2, 0, 0,	// 5 6 7
	 3, 1,		//  8 9
	1, 1, -1		// 0 1 2
};
int clear[13] = { 0 };
int robot = 0;

int RunStep(int mode);
void ConnectB1();
void ConnectB2();
void ConnectB3();
void ConnectB4();

int GetPathTo(int* path, int* pathLen, int from, int to);
void GetPathToDfs(int* path, int* pathLen, int* st, int stLen, int from, int to);

int GetCanGoPlacePos(int* canGoPos, int* prevCanGoPos, int* count, int p);
int GetCanGoPuckPos(int* canGoPos, int* count, int p);

int IsMatch(int* rgbCount, int index);

int GetConditionResult(int mode);
void FixPos(int index);
void FixClear(int mode);
int Search(int mode, int* fromPath, int* toPath, int* pathLen, int robot);
int SearchDfs(int mode, int* fromPath, int* toPath, int maxDepth, int robot, int depth);
void ExecutePath(int* robot, int* fromPath, int* toPath, int pathLen);

void PrintMap();
void PrintGoal();

int main() {
	ConnectB1();
	ConnectB4();

	// 로봇 시작위치 설정.
	robot = 6;

	int success[5] = { 0 };

	for(int mode = 0; mode < 5; mode++) {
		success[mode] = RunStep(mode);

		// 만약 이전 모드가 완료되지 않았을 시.
		if (mode > 0 && !success[mode - 1])
			success[mode - 1] = RunStep(mode - 1);
	}

	PrintMap();
	PrintGoal();

	return 0;
}

int RunStep(int mode) {
	int fromPath[8], toPath[8], pathLen;
	if (Search(mode, fromPath, toPath, &pathLen, robot)) {
		ExecutePath(&robot, fromPath, toPath, pathLen);
		FixClear(mode);
		printf("%d step success=============================\n", mode);
		system("pause");

		return 1;
	}
	else {
		printf("%d step failed=============================\n", mode);
		system("pause");

		return 0;
	}
}

void ConnectB1() {
	link[7][4] = 12;
	link[12][0] = 7;
}

void ConnectB2() {
	link[2][4] = 7;
	link[7][0] = 2;
}

void ConnectB3() {
	link[5][4] = 10;
	link[10][0] = 5;
}

void ConnectB4() {
	link[0][4] = 5;
	link[5][0] = 0;
}
/*
int GetObstaclePos() {//시트지 위치 반환하는 듯?--사용 안함 --
	for (int i = 0; i < 13; i++) {
		if (map[i] == -1)
			return i;
	}

	return -1;
}*/

int GetPathTo(int* path, int* pathLen, int from, int to) {	//checkDFS? 
	*pathLen = 999; 
	int st[13] = { from };

	GetPathToDfs(path, pathLen, st, 1, from, to);

	return *pathLen != 999;
}

void GetPathToDfs(int* path, int* pathLen, int *st, int stLen, int from, int to) {
	if (stLen > 13)
		return;

	if (from == to) {
		if (stLen < *pathLen) {
			for (int i = 0; i < stLen; i++)
				path[i] = st[i];

			*pathLen = stLen;
		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			int next = link[from][i];
			if (next >= 0 && (!map[next] || next == to)){
				int b = map[next];
				map[next] = -2;		//
				st[stLen] = next;	//루트 저장 
				GetPathToDfs(path, pathLen, st, stLen + 1, next, to);
				map[next] = b;
			}
		}
	}
}

int GetCanGoPlacePos(int* canGoPos, int* prevCanGoPos, int* count, int p) {
	*count = 0;

	int q[13], prevQ[13], chk[13] = { 0 };
	int head = 0, tail = 0;

	q[tail++] = p;
	chk[p] = 1;

	while (head < tail) {
		int prevNode = prevQ[head];
		int node = q[head++];

		int flag = 0;
		for (int i = 0; i < 8; i++) {
			int next = link[node][i];
			if (next >= 0 && !chk[next] && !map[next])
			{
				prevQ[tail] = node;
				q[tail++] = next;

				chk[next] = 1;
				flag = 1;
			}
		}

		if ((!flag || map[p] == goal[node]) && node != p) {
			prevCanGoPos[*count] = prevNode;
			canGoPos[(*count)++] = node;
		}
	}

	return *count > 0;
}

int GetCanGoPuckPos(int* canGoPos, int* count, int p) {
	*count = 0;

	int q[13], chk[13] = { 0 };
	int head = 0, tail = 0;

	q[tail++] = p;
	chk[p] = 1;

	while (head < tail) {
		int node = q[head++];

		for (int i = 0; i < 8; i++) {
			int next = link[node][i];
			if (next >= 0 && !chk[next])
			{
				chk[next] = 1;
				if (!map[next])
					q[tail++] = next;
				else if (map[next] > 0)
					canGoPos[(*count)++] = next;
			}
		}
	}


	return *count > 0;
}

int IsMatch(int* rgbCount, int index) {	//Sorted
	if (map[index] == goal[index]) {
		if (map[index] >= 0)
			rgbCount[map[index]]--;

		return 1;
	}	

	if (!map[index] && !rgbCount[goal[index]])
		return 1;

	return 0;
}

int GetConditionResult(int mode) {
	int rgbCount[4] = { 0 };
	for (int p = 0; p < 13; p++) {
		if (!clear[p] && map[p] >= 0)
			rgbCount[map[p]]++;
	}

	switch (mode)
	{
		case 0:
			if (IsMatch(rgbCount, 10) && IsMatch(rgbCount, 11) && IsMatch(rgbCount, 12))
				return 1;
			break;
		case 1:
			if (IsMatch(rgbCount, 0) && IsMatch(rgbCount, 1) && IsMatch(rgbCount, 2))
				return 1;
			break;
		case 2:
			if (IsMatch(rgbCount, 8) && IsMatch(rgbCount, 9))
				return 1;
			break;
		case 3:
			if (IsMatch(rgbCount, 3) && IsMatch(rgbCount, 4))
				return 1;
			break;
		case 4:
			if (IsMatch(rgbCount, 5) && IsMatch(rgbCount, 6) && IsMatch(rgbCount, 7))
				return 1;
			break;
	}

	return 0;
}

void FixPos(int index) {//정렬이 완료된부분 체크 
	if (map[index] == goal[index])
		clear[index] = 1;
}

void FixClear(int mode) {
	switch (mode)
	{
		case 0:
			FixPos(10);
			FixPos(11);
			FixPos(12);
			break;
		case 1:
			FixPos(0);
			FixPos(1);
			FixPos(2);
			break;
		case 2:
			FixPos(8);
			FixPos(9);
			break;
		case 3:
			FixPos(3);
			FixPos(4);
			break;
		case 4:
			FixPos(5);
			FixPos(6);
			FixPos(7);
			break;
	}
}

int Search(int mode, int* fromPath, int* toPath, int* pathLen, int robot) {
	*pathLen = 0;

	for (int i = 1; i < 8; i++) {
		*pathLen = SearchDfs(mode, fromPath, toPath, i, robot, 0);
		if (*pathLen >= 0)
			return 1;
	}

	return 0;
}

//             mode    sort_rouote[0] sort_rouote[1]     min      robot      dp
int SearchDfs(int mode, int* fromPath, int* toPath, int maxDepth, int robot, int depth) {
	if (depth == maxDepth)
		return -1;

	if (GetConditionResult(mode)) {
		return depth;
	}
	else {
		int canGoPuckPos[13] = { 0 }, canGoPuckPosCnt = 0;
		if (GetCanGoPuckPos(canGoPuckPos, &canGoPuckPosCnt, robot)) {
			for (int i = 0; i < canGoPuckPosCnt; i++) {
				int p = canGoPuckPos[i];

				if (clear[p])
					continue;

				int canGoPos[13] = { 0 }, prevCanGoPos[13] = { 0 }, canGoPosCnt = 0;
				if (GetCanGoPlacePos(canGoPos, prevCanGoPos, &canGoPosCnt, p)) {
					for (int j = 0; j < canGoPosCnt; j++) {
						int next = canGoPos[j];
						int prevNext = prevCanGoPos[j];
						fromPath[depth] = p;
						toPath[depth] = next;
						map[next] = map[p];
						map[p] = 0;

						int result = SearchDfs(mode, fromPath, toPath, maxDepth, prevNext, depth + 1);

						map[p] = map[next];
						map[next] = 0;

						if (result >= 0)
							return result;
					}
				}
			}
		}
	}

	return -1;
}

void ExecutePath(int* robot, int* fromPath, int* toPath, int pathLen) {
	int robotPath[13], robotPathLen;
	for (int i = 0; i < pathLen; i++) {
		if (!GetPathTo(robotPath, &robotPathLen, *robot, fromPath[i]))
			return;

		// robot 위치 수정
		*robot = fromPath[i];

		map[toPath[i]] = map[fromPath[i]];
		map[fromPath[i]] = 0;

		printf("%2d -> %2d\n", fromPath[i], toPath[i]);

		if (!GetPathTo(robotPath, &robotPathLen, fromPath[i], toPath[i]))
			return;

		// robot 위치 수정
		*robot = robotPath[robotPathLen - 2];
	}
}

void PrintMap() {
	int c = 0;
	printf("===============================================\n");
	printf("ROBOT: %d\n", robot);
	printf("%2d %2d %2d\n", map[0], map[1], map[2]);
	printf(" %2d  %2d\n", map[3], map[4]);
	printf("%2d %2d %2d\n", map[5], map[6], map[7]);
	printf(" %2d  %2d\n", map[8], map[9]);
	printf("%2d %2d %2d\n", map[10], map[11], map[12]);
	printf("===============================================\n");
}

void PrintGoal() {
	int c = 0;
	printf("===============================================\n");
	printf("ROBOT: %d\n", robot);
	printf("%2d %2d %2d\n", goal[0], goal[1], goal[2]);
	printf(" %2d  %2d\n", goal[3], goal[4]);
	printf("%2d %2d %2d\n", goal[5], goal[6], goal[7]);
	printf(" %2d  %2d\n", goal[8], goal[9]);
	printf("%2d %2d %2d\n", goal[10], goal[11], goal[12]);
	printf("===============================================\n");
}
