#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define	MAXL			5
#define MAXF			10

extern void init(int N);
extern void add(int id, int F, int ids[MAXF]);
extern void del(int id1, int id2);
extern int  recommend(int id, int list[MAXL]);

#define INIT			1
#define ADD				2
#define DEL				3
#define RECOMMEND		4

static int N, M;

static bool run()
{
    int cmd;
    int id, F, ids[MAXF];
    int id1, id2;
    int len, len_a;
    int list[MAXL], list_a[MAXL];

    bool okay;

    okay = false;

    scanf("%d", &M);

    for (int k = 0; k < M; ++k)
    {
        scanf("%d", &cmd);
        switch (cmd)
        {
        case INIT:
            scanf("%d", &N);
            init(N);
            okay = true;
            break;
        case ADD:
            scanf("%d %d", &id, &F);
            for (int i = 0; i < F; ++i)
                scanf("%d", &ids[i]);
            if (okay)
                add(id, F, ids);
            break;
        case DEL:
            scanf("%d %d", &id1, &id2);
            if (okay)
                del(id1, id2);
            break;
        case RECOMMEND:
            scanf("%d %d", &id, &len_a);
            for (int i = 0; i < len_a; ++i)
                scanf("%d", &list_a[i]);
            if (okay)
            {
                len = recommend(id, list);
                if (len != len_a)
                    okay = false;

                for (int i = 0; okay && i < len_a; ++i)
                    if (list[i] != list_a[i])
                        okay = false;
            }
            break;
        }
    }

    return okay;
}

int main()
{
    int TC, MARK;

    freopen("sample_input.txt", "r", stdin);

    setbuf(stdout, NULL);
    scanf("%d %d", &TC, &MARK);

    for (int testcase = 1; testcase <= TC; ++testcase)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", testcase, score);
    }

    return 0;
}


#if 1

//-------------------------------------------------------------------------------

void generic_swap(void* a, void* b, int size)
{
    char* x = (char*)a;
    char* y = (char*)b;
    char t;

    int i;

    for (i = 0; i < size; i++)
    {
        t = x[i];
        x[i] = y[i];
        y[i] = t;
    }
}

void sort(void* base, size_t num, size_t size,
    int (*cmp_func)(void*, void*))
{
    int i = (num / 2 - 1) * size, n = num * size, c, r;
    void (*swap_func)(void*, void*, int size);
    char* a = (char*)base;

    swap_func = generic_swap;

    /* heapify */
    for (; i >= 0; i -= size) {
        for (r = i; r * 2 + size < n; r = c) {
            c = r * 2 + size;
            if (c < n - size &&
                cmp_func(a + c, a + c + size) < 0)
                c += size;
            if (cmp_func(a + r, a + c) >= 0)
                break;
            swap_func(a + r, a + c, size);
        }
    }

    /* sort */
    for (i = n - size; i > 0; i -= size) {
        swap_func(a, a + i, size);
        for (r = 0; r * 2 + size < i; r = c) {
            c = r * 2 + size;
            if (c < i - size &&
                cmp_func(a + c, a + c + size) < 0)
                c += size;
            if (cmp_func(a + r, a + c) >= 0)
                break;
            swap_func(a + r, a + c, size);
        }
    }
}

//--------------------------------------------------------------------------------------

#define MAX_USER 10000

typedef struct
{
    int uid;
} USER;

typedef struct
{
    int friend_count;
    USER* p;
} SORT_TABLE;

USER user_table[MAX_USER];
int friend_table[MAX_USER][MAX_USER];

int user_count;

void init(int N)
{
    int i, j;
    user_count = N;

    for (i = 0; i < N; i++)
    {
        user_table[i].uid = i + 1;
        for (j = 0; j < N; j++)
            friend_table[i][j] = 0;
    }
}

void add(int id, int F, int ids[])
{
    int i;

    for (i = 0; i < F; i++)
    {
        friend_table[id - 1][ids[i] - 1] = 1;
        friend_table[ids[i] - 1][id - 1] = 1;
    }
}

void del(int id1, int id2)
{
    friend_table[id1-1][id2-1] = 0;
    friend_table[id2-1][id1-1] = 0;
}

int my_compare(void* a, void* b)
{
    int ret = 0;
    SORT_TABLE* x = (SORT_TABLE*)a;
    SORT_TABLE* y = (SORT_TABLE*)b;

    ret = y->friend_count - x->friend_count;

    if (ret == 0)
    {
        ret = x->p->uid - y->p->uid;
    }

    return ret;
}

int recommend(int id, int list[])
{
    int i, j, k;
    int f_count = 0;
    int f_temp[10000];
    SORT_TABLE sort_table[10000] = { 0, };
    int sort_count = 0;

    for (i = 0; i < user_count; i++)
    {
        if (friend_table[id - 1][i] == 1)
            f_temp[f_count++] = i;
    }

    if (f_count == 0)
        return 0;

    for (i = 0; i < user_count; i++)
    {
        if (id - 1 == i)
            continue;

        for (j = 0; j < f_count; j++)
            if (f_temp[j] == i)
                break;

        if (j < f_count)
            continue;

        for (j = 0; j < user_count; j++)
        {
            if (friend_table[i][j] == 0)
                continue;

            for (k = 0; k < f_count; k++)
            {
                if (f_temp[k] == j)
                {
                    sort_table[sort_count].friend_count++;
                }
            }
        }
        if (sort_table[sort_count].friend_count > 0)
            sort_table[sort_count++].p = &user_table[i];
    }

    sort(sort_table, sort_count, sizeof(sort_table[0]), my_compare);
    for (i = 0; i < sort_count; i++)
    {
        if (i >= 5)
            break;
        list[i] = sort_table[i].p->uid;
    }

    return i;
}

#endif

