/*
 
 * @file: [H2008] 삼삼퍼즐
 
 * @brief: 샘플 답안
 
 * @copyright: All rights reserved (c) 2020 Samsung Electronics, Inc.
 
 */
 
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
 
extern bool swap(int dir);
 
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };
 
/**
* board: 현재 상태
* pattern: 정답 패턴
* Y0: 현재 빈칸 위치 (Y좌표)
* X0: 현재 빈칸 위치 (X좌표)
*/
int board[5][5], pattern[3][3], Y0, X0;
 
 
void mySwap(int dir) {
    board[Y0][X0] = board[Y0 + dy[dir]][X0 + dx[dir]];
    board[Y0 + dy[dir]][X0 + dx[dir]] = 0;
 
    Y0 += dy[dir]; X0 += dx[dir];
    swap(dir);
}
/**
* fixed: 정답 맞춘 위치는 1 나머진 -1
* Visited: 
* marked: 
* 
*/
int fixed[5][5], Visited[5][5];
bool marked[5][5];
 
struct Point 
{ 
    int y, x; 
};
Point queue[555]; int front, end;
int Movement[555], moveCnt;
 
void trace(int ys, int xs, int ye, int xe, int mark) {
    int ws = moveCnt;
    while (!(ys == ye && xs == xe)) {
        marked[ye][xe] = mark;
        int dir = Visited[ye][xe];
        Movement[moveCnt++] = dir;
        ye -= dy[dir]; xe -= dx[dir];
    }
     
    // 이 아래는 그냥 reverse 해주는 과정. 다르게 처리해도 될듯..
    int we = moveCnt;
    for (int i = 0; i < (we - ws) / 2; i++) {
        int t = Movement[ws + i];
        Movement[ws + i] = Movement[we - i - 1];
        Movement[we - i - 1] = t;
    }
}
 
void move(int ys, int xs, int ye, int xe) {
    front = end = 0;
    queue[0].y = ys;
    queue[0].x = xs;
     
    //[ys][xs]로 부터 모든 위치까지 이동 방향을 Visited[][] 배열에 저장.
    while (front <= end) {
        Point p = queue[front++];
        for (int i = 0; i < 4; i++) {
            int ny = p.y + dy[i];
            int nx = p.x + dx[i];
             
            // 맵 밖으로 나가는지 체크
            if (!(0 <= ny && ny < 5 && 0 <= nx && nx < 5)) continue;
             
            // 이미 정답처리된 위치(fixed)거나 방문한 위치인지 체크
            if (fixed[ny][nx] || Visited[ny][nx] != -1) continue;
 
            ++end;
            queue[end].y = ny;
            queue[end].x = nx;
 
            Visited[ny][nx] = i;
            if (ny == ye && nx == xe)break;
        }
    }
    trace(ys, xs, ye, xe, 0);
}
 
void calc(int y, int x, int c) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++)
            Visited[i][j] = -1;
    }
 
    //////////////////////////////////////////////////////////////////////////////////////////
    // 내가 원하는 위치에 빈칸을 이동 시킴
    Visited[Y0][X0] = 0;
    moveCnt = 0;
    move(Y0, X0, y, x); // 이동 경로 찾기
    for (int i = 0; i < moveCnt; i++) 
        mySwap(Movement[i]);
    //////////////////////////////////////////////////////////////////////////////////////////
 
 
    //////////////////////////////////////////////////////////////////////////////////////////
    // 가장 가까운 target 색칸 찾기
    int Yt = -1, Xt = -1;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Visited[i][j] = -1;
            marked[i][j] = 0;
        }
    }
    Visited[Y0][X0] = 0;
 
    front = end = 0;
    queue[0].y = Y0;
    queue[0].x = X0;
     
    while (front <= end && Yt == -1 && Xt == -1) {
        Point p = queue[front++];
        int y = p.y, x = p.x;
        for (int i = 0; i < 4; i++) {
            int ny = y + dy[i];
            int nx = x + dx[i];
            if (!(0 <= ny && ny < 5 && 0 <= nx && nx < 5)) continue;
            if (fixed[ny][nx] || Visited[ny][nx] >= 0) continue;
             
            ++end;
            queue[end].y = ny;
            queue[end].x = nx;
            Visited[ny][nx] = i;
             
            if (board[ny][nx] == c) {
                Yt = ny, Xt = nx;
                break;
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////
    moveCnt = 0;
    trace(Y0, X0, Yt, Xt, 1);
 
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) Visited[i][j] = marked[i][j] ? 9 : -1;
    }
    Visited[Yt][Xt] = 0;
    move(Yt, Xt, Y0, X0);
 
    for (int i = 0; ; i = (i + 1) % moveCnt) {
        mySwap(Movement[i]);
        if (board[y][x] == c) return;
    }
}
 
void solve(int orgBoard[5][5], int orgPattern[3][3], int callCntLimit) {
    // board 초기화, 
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            marked[i][j] = fixed[i][j] = 0;
            board[i][j] = orgBoard[i][j];
 
            // 빈 칸 위치를 기억함.
            if (board[i][j] == 0) Y0 = i, X0 = j;
        }
    }
    //pattern 초기화
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            pattern[i][j] = orgPattern[i][j];
        }
    }
 
    // pattern의 우상단(1,1) 부터 순서대로 완성해나감
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // (i+1, i+1) 위치를 정답색으로 만든다.
            calc(i + 1, j + 1, pattern[i][j]);
 
            // fix 된 위치, 5x5 맵에 정답 완성된 위치를 fixed 표시함
            // 이후 bfs 탐색에서 요 위치는 방문하지 않도록 처리한다.
            fixed[i + 1][j + 1] = 1;
        }
    }
}
