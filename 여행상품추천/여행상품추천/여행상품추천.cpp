#define MAX_PROD  40000 
#define MAX_SIZE MAX_PROD
#define MAX_HASH MAX_PROD * 2 + 7
 
#define MAX_AREA 11
#define MAX_USER 1000
 
 
typedef long long ll;
struct P {
    int pid;
    int aid;
    int price;
    ll score;
    int heapidx;
 
    P *n;
};
P prod[MAX_PROD];
int pCnt;
P table[MAX_HASH];
 
struct AREA {
    P *heap[MAX_SIZE];
    int heapSize = 0;
 
    void up(register int current) {
        register P *temp;
        while (current > 0 && heap[current]->score < heap[(current - 1) >> 1]->score) {
            temp = heap[(current - 1) >> 1];
            heap[(current - 1) >> 1] = heap[current];
            heap[current] = temp;
 
            heap[(current - 1) >> 1]->heapidx = (current - 1) >> 1;
            heap[current]->heapidx = current;
 
            current = (current - 1) >> 1;
        }
    }
 
    int heapPush(register P *value){
        if (heapSize + 1 > MAX_SIZE){
            return 0;
        }
 
        heap[heapSize] = value;
 
        heap[heapSize]->heapidx = heapSize;
        up(heapSize);
 
        heapSize = heapSize + 1;
 
        return 1;
    }
 
    void down(register int current) {
        register int child;
        register P *temp;
        while (current * 2 + 1 < heapSize) {
            if (current * 2 + 2 == heapSize) {
                child = current * 2 + 1;
            } else {
                child = heap[current * 2 + 1]->score < heap[current * 2 + 2]->score ? current * 2 + 1 : current * 2 + 2;
            }
 
            if (heap[current]->score < heap[child]->score) {
                break;
            }
 
            temp = heap[current];
            heap[current] = heap[child];
            heap[child] = temp;
 
            heap[current]->heapidx = current;
            heap[child]->heapidx = child;
 
            current = child;
        }
    }
 
    int heapPop(){
        if (heapSize <= 0) {
            return -1;
        }
 
        heapSize = heapSize - 1;
 
        heap[0] = heap[heapSize];
 
        heap[0]->heapidx = 0;
        down(0);
         
        return 1;
    }
 
    void del(int idx) {
        heap[idx] = heap[--heapSize];
        heap[idx]->heapidx = idx;
        up(idx);
        down(idx);
    }
 
};
AREA ar[MAX_AREA + 1];
 
int N, M;
int friends[MAX_USER + 1][20 + 1];
int fcnt[MAX_USER + 1];
 
int areaCnt[MAX_USER + 1][MAX_AREA + 1];
 
void init(int N, int M) {
    ::N = N;    // user ¼ö
    ::M = M;    // area ¼ö
 
    pCnt = 0;
    for (register int i = 0; i < MAX_HASH; ++i) {
        table[i].n = 0;
    }
     
    for (register int i = 0; i < MAX_AREA + 1; ++i) {
        ar[i].heapSize = 0;
    }
 
    for (register int i = 0; i < MAX_USER + 1; ++i) {
        friends[i][0] = i;
        fcnt[i] = 1;
        for (register int k = 0; k < MAX_AREA + 1; ++k) {
            areaCnt[i][k] = 0;
        }
    }
}
 
void befriend(register int uid1, register int uid2) {
    friends[uid1][fcnt[uid1]++] = uid2;
    friends[uid2][fcnt[uid2]++] = uid1;
}
 
void add(register int pid, register int area, register int price) {
    register P *p = &prod[pCnt++];
    p->pid = pid;    // 1 ~ 10¾ï
    p->aid = area;
    p->price = price;
    p->score = ((ll)price << 32) + pid;
 
    register int pkey = pid % MAX_HASH;
    p->n = table[pkey].n;
    table[pkey].n = p;
 
    ar[p->aid].heapPush(p);
}
 
void reserve(register int uid, register int pid) {
    register int pkey = pid % MAX_HASH;
    register P *p = table[pkey].n;
    while (p) {
        if (p->pid == pid)
            break;
        p = p->n;
    }
     
    ++areaCnt[uid][p->aid];  // user * area
    ar[p->aid].del(p->heapidx);
}
 
int recommend(register int uid) {
    register int list[MAX_AREA] = { 0, };
    for (register int i = 0; i < MAX_AREA; ++i) {
        for (register int k = 0; k < fcnt[uid]; ++k) {
            list[i] += areaCnt[friends[uid][k]][i];
        }
    }
 
    register int max = -1;
    register P *p = 0;
    for (register int i = 1; i <= M; ++i) {
        if (max < list[i] && ar[i].heapSize != 0) {
            p = ar[i].heap[0];
            max = list[i];
        }
        else if (max == list[i] && p->score > ar[i].heap[0]->score) {
            p = ar[i].heap[0];
        }
    }
 
    return p->pid;
}
