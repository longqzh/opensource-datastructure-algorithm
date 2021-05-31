#define MAX_STR 20000
#define NULL 0
#define MAX_LEN 50001
#define MAX_NODE 140000
 
 
struct NODE
{
    int mArrID;
    int mStr;
 
    NODE *prev;
    NODE *next;
}mNode[MAX_NODE];
 
int arridx;
int MAXN;
char Ans[MAX_LEN];
 
 
NODE *pHead[MAX_STR];
NODE *pTail[MAX_STR];
 
NODE *myAlloc(int v)
{
    mNode[v].mArrID = -1;
    mNode[v].next = NULL;
    mNode[v].prev = NULL;
 
    return &mNode[v];
}
 
int GetNumber(char *str)
{
    int ret = 0;
 
    for (int i = 0; i < 3; i++)
    {
        ret = (ret * 27) + str[i] - 'a';
    }
 
    return ret;
}
 
void init(int N, char init_string[])
{
    MAXN = N;
    arridx = 100000;
 
    for (register int i = 0; i < MAX_STR; i++)
    {
        pHead[i] = NULL;
        pTail[i] = NULL;
        pHead[i] = myAlloc(arridx++);
        pTail[i] = myAlloc(arridx++);
 
        pHead[i]->next = pTail[i];
        pTail[i]->prev = pHead[i];
    }
    arridx = 0;
 
    for (register int i = 0; i < N; i++)
    {
        Ans[i] = init_string[i];
 
        if (i >= 2)
        {
            int hkey = GetNumber(&Ans[i - 2]);
         
            NODE *p = myAlloc(arridx++);
            p->mArrID = i - 2;
            p->mStr = hkey;
         
            p->prev = pTail[hkey]->prev;
            pTail[hkey]->prev = p;
 
            p->next = p->prev->next;
            p->prev->next = p;
        }
    }
}
void insert_Node(int dst, int idx, int option)
{
    if (option == -1)
    {
        mNode[idx].prev = pTail[dst]->prev;
        pTail[dst]->prev = &mNode[idx];
        mNode[idx].next = mNode[idx].prev->next;
        mNode[idx].prev->next = &mNode[idx];
    }
    else
    {
        mNode[idx].prev = mNode[option].prev;
        mNode[option].prev = &mNode[idx];
        mNode[idx].next = mNode[idx].prev->next;
        mNode[idx].prev->next = &mNode[idx];
 
    }
 
}
 
void delete_node(int idx)
{
 
    mNode[idx].prev->next = mNode[idx].next;
    mNode[idx].next->prev = mNode[idx].prev;
}
 
int Find_Node(int hkey, int idx)
{
 
    for (NODE *pp = pHead[hkey]->next; pp != pTail[hkey]; pp = pp->next)
    {
        if (idx <= pp->mArrID)
            return pp->mArrID;
    }
 
    return -1;
}
 
void Change_Node(int hkey, int dst, int idx)
{
    int pos = 0;
    delete_node(idx);
 
    NODE *p = myAlloc(idx);
    p->mArrID = idx;
    p->mStr = dst;
 
    pos = Find_Node(dst, idx);
    insert_Node(dst, idx, pos);
 
}
 
 
 
 
 
int change(char string_A[], char string_B[])
{
    int ret = 0;
    int src = GetNumber(string_A);
    int dst = GetNumber(string_B);
    int dummy=0;
    int cnt = 0;
    int pos = 0;
 
    while (cnt < MAXN)
    {
 
        pos = Find_Node(src, cnt);
 
        if (pos >= 0)
        {
            for (int i = pos; i < pos + 3; i++)
                Ans[i] = string_B[i - pos];
            cnt = mNode[pos].mArrID;
            Change_Node(src, dst, pos);
 
            if (pos - 2 >= 0)
            {
                dummy = GetNumber(&Ans[pos - 2]);
                Change_Node(mNode[pos-2].mStr, dummy, pos-2);
            }
 
            if (pos - 1 >= 0)
            {
                dummy = GetNumber(&Ans[pos - 1]);
                Change_Node(mNode[pos - 1].mStr, dummy, pos-1);
            }
 
            if (pos + 1 < MAXN-2)
            {
                dummy = GetNumber(&Ans[pos + 1]);
                Change_Node(mNode[pos + 1].mStr, dummy, pos+1);
            }
 
            if (pos + 2 < MAXN-2)
            {
                dummy = GetNumber(&Ans[pos +2]);
                Change_Node(mNode[pos +2].mStr, dummy, pos+2);
            }
 
 
            ret++;
 
 
        }
        else
        {
 
            break;
        }
 
 
        cnt += 3;
         
    }
     
 
 
 
    return ret;
}
void result(char ret[])
{
    int i = 0;
    for (i = 0; i < MAXN; i++)
        ret[i] = Ans[i];
    ret[i] = '\0';
}
