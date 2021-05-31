#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

extern void init(int N, int M);
extern void befriend(int uid1, int uid2);
extern void add(int pid, int area, int price);
extern void reserve(int uid, int pid);
extern int  recommend(int uid);

#define INIT			100
#define BEFRIEND		200
#define ADD				300
#define RESERVE			400
#define RECOMMEND		500

static bool run()
{
    int L, N, M;
    bool okay = false;

    int pid, uid;
    int uid1, uid2;
    int area, price;

    int ans;
    int cmd, ret;

    scanf("%d", &L);

    for (int l = 0; l < L; ++l)
    {
        scanf("%d", &cmd);
        switch (cmd)
        {
        case INIT:
            scanf("%d %d", &N, &M);
            init(N, M);
            okay = true;
            break;
        case BEFRIEND:
            scanf("%d %d", &uid1, &uid2);
            befriend(uid1, uid2);
            break;
        case ADD:
            scanf("%d %d %d", &pid, &area, &price);
            add(pid, area, price);
            break;
        case RESERVE:
            scanf("%d %d", &uid, &pid);
            reserve(uid, pid);
            break;
        case RECOMMEND:
            scanf("%d %d", &uid, &ans);
            ret = recommend(uid);
            if (ret != ans)
                okay = false;
            break;
        }
    }

    return okay;
}

int main() {
    int TC, MARK;

    freopen("sample_input.txt", "r", stdin);

    setbuf(stdout, NULL);
    scanf("%d %d", &TC, &MARK);

    for (int testcase = 1; testcase <= TC; ++testcase) {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", testcase, score);
    }

    return 0;
}

#if 1

#undef  offsetof
#define offsetof(TYPE, MEMBER)	((unsigned int)&((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) 			\
		((type *)( (char *)ptr - offsetof(type,member) ))

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_entry(pos, head, member)				\
	for (pos = list_first_entry(head, typeof(*pos), member);	\
			&pos->member != (head);					\
			pos = list_next_entry(pos, member))

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

typedef struct node
{
	struct node* next, * prev;
} NODE;

void __list_add(NODE* temp,
	NODE* prev,
	NODE* next)
{
	next->prev = temp;
	temp->next = next;
	temp->prev = prev;
	prev->next = temp;
}

void list_add(NODE* temp, NODE* head)
{
	__list_add(temp, head, head->next);
}

void list_add_tail(NODE* temp, NODE* head)
{
	__list_add(temp, head->prev, head);
}

void __list_del(NODE* prev, NODE* next)
{
	next->prev = prev;
	prev->next = next;
}

void list_del(NODE* entry)
{
	__list_del(entry->prev, entry->next);
}

int list_empty(const NODE* head)
{
	return head->next == head;
}

void INIT_LIST_HEAD(NODE* list)
{
	list->next = list;
	list->prev = list;
}

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
	char* a = (char*)base;
	int i = (num / 2 - 1) * size, n = num * size, c, r;
	void (*swap_func)(void*, void*, int size);

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

#define MAX_USER 1001
#define MAX_PRODUCT 40000
#define MAX_AREA 11
#define MAX_FRIENDS 1000
#define MAX_HASH 20007

typedef struct
{
	int pid;
	int area;
	int price;
	NODE list;
	NODE hash_list;
} PRODUCT;

typedef struct
{
	int uid;
	NODE rlist_head;
} USER;

typedef struct
{
	PRODUCT* p;
} SORT_TABLE;

typedef struct
{
	int area;
	int count;
} AREA_TABLE;


USER user_pool[MAX_USER];
PRODUCT product_pool[MAX_PRODUCT];
AREA_TABLE area_pool[MAX_AREA];
int friend_table[MAX_FRIENDS][MAX_FRIENDS];

NODE product_list_head;
NODE product_hash_heads[MAX_HASH];


int product_pool_count;
int user_pool_count;
int area_pool_count;
int friend_pool_count;

void init(int N, int M)
{
	int i, j;

	product_pool_count = 0;
	user_pool_count = N;
	area_pool_count = M;
	friend_pool_count = N;

	INIT_LIST_HEAD(&product_list_head);

	for (i = 0; i < user_pool_count; i++)
	{
		user_pool[i].uid = i + 1;
		INIT_LIST_HEAD(&user_pool[i].rlist_head);
	}

	for (i = 0; i < area_pool_count; i++)
	{
		area_pool[i].area = 0;
		area_pool[i].count = 0;
	}

	for (i = 0; i < MAX_HASH; i++)
	{
		INIT_LIST_HEAD(&product_hash_heads[i]);
	}

	for (i = 0; i < friend_pool_count; i++)
		for (j = 0; j < friend_pool_count; j++)
			friend_table[i][j] = 0;
}


void befriend(int uid1, int uid2)
{
	friend_table[uid1 - 1][ uid2 - 1] = 1;
	friend_table[uid2 - 1][ uid1 - 1] = 1;
}

void add(int pid, int area, int price)
{
	PRODUCT* p;
	p = &product_pool[product_pool_count++];
	p->pid = pid;
	p->area = area;
	p->price = price;
	list_add_tail(&p->list, &product_list_head);
	list_add_tail(&p->hash_list, &product_hash_heads[pid%MAX_HASH]);
}

void reserve(int uid, int pid)
{
	int i;
	PRODUCT* p = 0;
	NODE* temp;
	for (i = 0; i < user_pool_count; i++)
	{
		if (user_pool[i].uid == uid)
			break;
	}
	list_for_each(temp, &product_hash_heads[pid%MAX_HASH])
	{
		p = list_entry(temp, PRODUCT, hash_list);
		if (p->pid == pid)
			break;
	}
	list_del(&p->list);
	list_add_tail(&p->list, &user_pool[i].rlist_head);
}

int my_product_compare(void* a, void* b)
{
	int ret = 0;
	SORT_TABLE* x = (SORT_TABLE*)a;
	SORT_TABLE* y = (SORT_TABLE*)b;

	if (x->p->price - y->p->price != 0)
	{
		ret = x->p->price - y->p->price;
	}
	else
		ret = x->p->pid - y->p->pid;

	return ret;
}

int my_area_compare(void* a, void* b)
{
	int ret = 0;
	AREA_TABLE* x = (AREA_TABLE*)a;
	AREA_TABLE* y = (AREA_TABLE*)b;

	ret = y->count - x->count;

	return ret;
}

int recommend(int uid)
{
	int pid;
	int i, j;
	PRODUCT* p;
	NODE* temp;
	int count = 0;
	SORT_TABLE sort_table[10000] = { 0, };
	int sort_count = 0;

	for (i = 0; i < area_pool_count; i++)
	{
		area_pool[i].area = i + 1;
		area_pool[i].count = 0;
	}

	for (i = 0; i < user_pool_count; i++)
	{
		if (user_pool[i].uid == uid)
		{
			list_for_each(temp, &user_pool[i].rlist_head)
			{
				p = list_entry(temp, PRODUCT, list);
				area_pool[p->area - 1].count++;
				count++;
			}
			break;
		}
	}


	for (j = 0; j < user_pool_count; j++)
	{
		if (friend_table[i][j])
		{
			list_for_each(temp, &user_pool[j].rlist_head)
			{
				p = list_entry(temp, PRODUCT, list);
				area_pool[p->area - 1].count++;
				count++;
			}
		}
	}

	if (count == 0)
	{
		list_for_each(temp, &product_list_head)
		{
			p = list_entry(temp, PRODUCT, list);
			sort_table[sort_count++].p = p;
		}
		sort(sort_table, sort_count, sizeof(sort_table[0]), my_product_compare);
		pid = sort_table[0].p->pid;
	}
	else
	{
		sort(area_pool, area_pool_count, sizeof(area_pool[0]), my_area_compare);

		for (i = 0; i < area_pool_count; i++)
		{
			if (area_pool[i].count == 0)
				break;

			list_for_each(temp, &product_list_head)
			{
				p = list_entry(temp, PRODUCT, list);

				for (j = i; j < area_pool_count - 1; j++)
				{
					if (area_pool[j].area == p->area)
					{
						sort_table[sort_count++].p = p;
					}
					if (area_pool[j].count != area_pool[j + 1].count)
						break;
				}
			}

			if (sort_count == 0)
				continue;

			sort(sort_table, sort_count, sizeof(sort_table[0]), my_product_compare);
			pid = sort_table[0].p->pid;
			break;
		}

		if (sort_count == 0)
		{
			list_for_each(temp, &product_list_head)
			{
				p = list_entry(temp, PRODUCT, list);
				sort_table[sort_count++].p = p;
			}
			sort(sort_table, sort_count, sizeof(sort_table[0]), my_product_compare);
			pid = sort_table[0].p->pid;
		}
	}
	return pid;
}
#endif





#if 0
#define MAX_USER 1001
#define MAX_TRAVEL 40000
#define MAX_AREA 11
#define MAX_FRIENDS 20
#define MAX_HASH 20007

typedef unsigned long long LL;

struct NODE {
    int pid;
    int area;
    LL score;
    int reserved;
    NODE* next;
}node[MAX_TRAVEL];

struct USER {
    int friendsCnt;
    int friends[MAX_FRIENDS];
    int total[MAX_AREA];
    int count[MAX_AREA];
}user[MAX_USER];

struct HEAP {
    int heapSize;
    int heapId[MAX_TRAVEL];

    void push(int id) {
        register int current;
        current = ++heapSize;

        while ((current != 1) && node[id].score < node[heapId[current / 2]].score) {
            heapId[current] = heapId[current / 2];
            current = current / 2;
        }

        heapId[current] = id;
    }

    void pop() {
        register int parent, child;
        register int id = heapId[heapSize--];

        parent = 1;
        child = 2;

        while (child <= heapSize) {
            if ((child < heapSize) && (node[heapId[child]].score > node[heapId[child + 1]].score)) {
                child++;
            }

            if (node[id].score <= node[heapId[child]].score) {
                break;
            }

            heapId[parent] = heapId[child];

            parent = child;
            child = child * 2;
        }

        heapId[parent] = id;
    }

    int getTop() {
        return heapId[1];
    }
}heap[MAX_AREA];

int nodeCnt;
NODE* table[MAX_HASH];
int totalAreaCnt;
NODE* tempRetNode[1];

NODE* findNode(LL key, int pid) {
    for (register NODE* temp = table[key]; temp; temp = temp->next) {
        if (temp->pid == pid) {
            return temp;
        }
    }

    return 0;
}

void init(int N, int M)
{
    nodeCnt = 0;
    totalAreaCnt = M;

    for (register int i = 0; i < MAX_TRAVEL; i++) {
        node[i] = { 0, };
    }

    for (register int i = 0; i < MAX_USER; i++) {
        user[i].friendsCnt = 0;
        for (register int j = 1; j <= totalAreaCnt; j++) {
            user[i].total[j] = 0;
            user[i].count[j] = 0;
        }
    }

    for (register int i = 0; i < MAX_HASH; i++) {
        table[i] = { 0, };
    }

    for (register int i = 0; i < MAX_AREA; i++) {
        heap[i].heapSize = 0;
    }
}

void befriend(int uid1, int uid2)
{
    user[uid1].friends[user[uid1].friendsCnt++] = uid2;
    user[uid2].friends[user[uid2].friendsCnt++] = uid1;

    for (register int i = 1; i <= totalAreaCnt; i++) {
        user[uid1].total[i] += user[uid2].count[i];
        user[uid2].total[i] += user[uid1].count[i];
    }
}

void add(int pid, int area, int price)
{
    register LL key = pid % MAX_HASH;

    register NODE* tempNode = &node[nodeCnt];
    tempNode->pid = pid;
    tempNode->area = area;
    tempNode->reserved = 0;
    tempNode->score = (LL)price << 30 | pid;
    tempNode->next = table[key];
    table[key] = tempNode;

    heap[area].push(nodeCnt++);
}

void reserve(int uid, int pid)
{
    register LL key = pid % MAX_HASH;
    register NODE* tempNode = findNode(key, pid);
    tempNode->reserved = 1;

    user[uid].count[tempNode->area]++;
    user[uid].total[tempNode->area]++;

    for (register int i = 0; i < user[uid].friendsCnt; i++) {
        user[user[uid].friends[i]].total[tempNode->area]++;
    }
}

int recommend(int uid)
{
    register NODE tempNode = { 0,0,(LL)1 << 40 };
    register int tempVal = 0;

    for (register int i = 1; i <= totalAreaCnt; i++) {
        while (heap[i].heapSize > 0 && node[heap[i].getTop()].reserved) {
            heap[i].pop();
        }

        if (heap[i].heapSize == 0)
            continue;

        //가장 많이 예약한 지역을 찾는다
        if (tempVal < user[uid].total[i]) {
            tempNode = node[heap[i].getTop()];
            tempVal = user[uid].total[i];
        }
        else if (tempVal == user[uid].total[i] && node[heap[i].getTop()].score < tempNode.score) {
            tempNode = node[heap[i].getTop()];
        }
    }

    return tempNode.pid;
}
#endif