#define MAXL    5
#define MAXF    10
 
#define MAX_POOL 500000
#define MAX_N 10001
 
int pcnt;
int _N;
 
struct Node {
    Node* next;
    int id;
}pool[MAX_POOL];
 
struct List {
    Node add;
    Node del;
    void adding(int id) {
        Node* nn = &pool[pcnt++];
        nn->id = id;
        nn->next = add.next;
        add.next = nn;
    }
    void deleting(int id) {
        Node* nn = &pool[pcnt++];
        nn->id = id;
        nn->next = del.next;
        del.next = nn;
    }
}user[MAX_N];
 
void init(int N)
{
    pcnt = 0;
    _N = N;
    for (register int i = 0; i < MAX_N; i++) {
        user[i].add.next = 0;
        user[i].del.next = 0;
    }
}
 
 
void add(int id, int F, int ids[MAXF])
{
    for (register int i = 0; i < F; i++) {
        user[id].adding(ids[i]);
        user[ids[i]].adding(id);
    }
}
 
void del(int id1, int id2)
{
    user[id1].deleting(id2);
    user[id2].deleting(id1);
}
 
int recommend(int id, int list[MAXL])
{
    int friends[MAX_N] = { 0 };
    int fcnt[MAX_N] = { 0 };
 
    for (register Node* ptr = &user[id].add; ptr->next; ptr = ptr->next) {
        friends[ptr->next->id]++;
    }
    for (register Node* ptr = &user[id].del; ptr->next; ptr = ptr->next) {
        friends[ptr->next->id]--;
    }
 
    for (register int i = 1; i <= _N; i++) {
        if (friends[i] == 1) {
            for (register Node* ptr = &user[i].add; ptr->next; ptr = ptr->next) {
                if(friends[ptr->next->id] == 0)
                    fcnt[ptr->next->id]++;
            }   
            for (register Node* ptr = &user[i].del; ptr->next; ptr = ptr->next) {
                if (friends[ptr->next->id] == 0)
                fcnt[ptr->next->id]--;
            }
        }
    }
    fcnt[id] = -1;
    register int cnt = 0;
    register int idx = 0;
    for (register int i = 0; i < 5; i++) {
        register int max = 0;
        for (register int j = 1; j <= _N; j++) {
            if (max < fcnt[j]) {
                max = fcnt[j];
                idx = j;
            }
        }
        if (max > 0) {
            fcnt[idx] = 0;
            list[cnt++] = idx;
        }
        else
            break;
    }
 
    return cnt;
}
