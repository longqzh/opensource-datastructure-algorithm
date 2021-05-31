#define NULL 0
#define rint register int
#define MAX_TABLE 20011//서로 다른 단어의 종류는 최대 10,000 
#define MAIL_MAX  30005// sendMail() 함수의 호출 횟수는 최대 10,000 
#define MAX_NODE 500000 //20000 (testcase 10 : 410870)
#define MAX_WORD 11
#define MAX_USERS 1005
 
typedef unsigned long long u64;
 
u64 getHash(char* s, int* pos) {
    u64 h = 0;
    int cnt = 0;
    while (*s && *s != ' ') {
        h = (h << 5) + *s++ - 'a' + 1;
        cnt++;
    }
    *pos = cnt;
    return h;
}
 
int N; // ~ 1.000
int K; // ~ 300
 
//User 별 double linked List. (head, tail)
struct Node {
    int midx;
    Node *prev;
    Node *next;
};
Node nodePull[MAX_NODE];
int nodeIdx = 0;
#define newNode() &(nodePull[nodeIdx++])
 
struct LIST {
    Node* head[MAX_USERS];
    Node* tail[MAX_USERS];
    int listSize[MAX_NODE];
    void init(int N) {
        nodeIdx = 0;
        for (rint i = 0; i < N; i++) {
            head[i] = newNode();
            tail[i] = newNode();
            head[i]->next = tail[i];
            tail[i]->prev = head[i];
            listSize[i] = 0;
        }
    }
    void insert_Head(int uid, Node* n) {
        n->next = head[uid]->next;
        n->prev = head[uid];
        head[uid]->next->prev = n;
        head[uid]->next = n;
        listSize[uid]++;
    }
    void delete_node(int uid, Node* n) {
        if (n->next) n->next->prev = n->prev;
        if (n->prev) n->prev->next = n->next;
        listSize[uid]--;
    }
}DLL;
 
struct Mail {
    int midx;// mail idx
    int uid;
    u64 words[MAX_WORD];
    int wordNum;
}MPULL[MAIL_MAX];
 
int nMailCnt = 0;
#define newMail() &(MPULL[nMailCnt++])
 
//수신인들이 메일함 (user 별로 최대 300 개 저장)
 
//////////////////////////////////////////////////
void init(int N, int K)
{
    ::N = N;
    ::K = K;
    DLL.init(N);
    for (rint i = 0; i < nMailCnt; ++i) {
        MPULL[i] = { 0, };
    }
    nMailCnt = 0;
}
 
void sendMail(char subject[], int uID, int cnt, int rIDs[])
{
    Mail* mail = newMail();
    mail->midx = nMailCnt - 1;
    mail->uid = uID;
    int wordNum = 0;
    while (*subject) {
        int pos = 0;
        mail->words[wordNum++] = getHash(subject, &pos);
        subject = subject + pos;
        if (*subject == ' ') subject++;
    }
    mail->wordNum = wordNum;
    for (rint i = 0; i < cnt; ++i) {
        int uid = rIDs[i];
        if (DLL.listSize[uid] == K) {
            //tail->prev 제거
            DLL.delete_node(uid, DLL.tail[uid]->prev);
        }
        Node* n = newNode();
        n->midx = mail->midx;
        DLL.insert_Head(uid, n);
    }
}
 
int getCount(int uID)
{
    return  DLL.listSize[uID];
}
 
int deleteMail(int uID, char subject[])
{
    int cnt = DLL.listSize[uID];
    u64 words[MAX_WORD] = { 0, };
    int wordNum = 0;
    int match = 0;
 
    while (*subject) {
        int pos = 0;
        words[wordNum++] = getHash(subject, &pos);
        subject = subject + pos;
        if (*subject == ' ') subject++;
    }
    Node* t = DLL.head[uID]->next;
    rint i, j;
    for (i = 0; t && i < cnt; ++i) {
        Mail* mail = &(MPULL[t->midx]);
        if (mail->wordNum != wordNum) {
            t = t->next; //이거 빼먹음.
            continue;
        }
        for (j = 0; j < wordNum; ++j) {
            if (mail->words[j] != words[j]) break;
        }
        if (j == wordNum) {
            match++;
            DLL.delete_node(uID, t);
        }
        t = t->next;
    }
    return match;
}
 
int searchMail(int uID, char text[])
{
    int cnt = DLL.listSize[uID];
    Node* t = DLL.head[uID]->next;
    int pos;
    u64 word = getHash(text, &pos);
    int match = 0;
    for (rint i = 0; t && i < cnt; ++i) {
        Mail* mail = &(MPULL[t->midx]);
        bool find = false;
        for (rint j = 0; j < mail->wordNum; ++j) {
            if (mail->words[j] == word) {
                find = true;
                break;
            }
        }
        if (find) {
            match++;
        }
        t = t->next;
    }
 
    return match;
}
