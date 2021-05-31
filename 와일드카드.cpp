#define rint register int
#define NULL 0
const int ALPHABET_SIZE = 26;
enum SEARCH_OPT { COUNT = 0, REMOVE = 1 };
 
int mstrlen(const char *str) {
    rint len = 0;
    while (str[len]) {
        ++len;
    }
    return len;
}
 
int mstrcmp(const char *s1, const char *s2) {
    while (*s1 == *s2 && *s2)
        ++s1, ++s2;
    return (*s1 - *s2);
}
 
int mstrcpy(char *dst, const char*src) {
    rint i = 0;
    for (; src[i]; ++i) dst[i] = src[i];
    dst[i] = src[i];
    return i;
}
void reverseWord(char * dst, const char *src, int len) {
    for (int i = 0, j = len - 1; i < len; i++, j--) {
        dst[j] = src[i];
    }
    dst[len] = NULL;
}
 
struct Trie {
    Trie *parent;
    int word;  // it's end of word
    int cn;    // Number of children pointer    child 포인터의 수   count node
    int cwds;  // number of child words        child 단어의 수     count word  
               // 해당 문자로 시작하는 단어가 몇개가 있는지를 check 할 수 있는 것임. 
    struct Trie *c[ALPHABET_SIZE];
};
Trie pool[50000 * 5];
int pn = 0;
 
// 초기화
struct Trie *getNode(Trie* parent) {
    struct Trie *p = &pool[pn++];
    p->word = p->cwds = p->cn = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        p->c[i] = NULL;
    p->parent = parent;
    return p;
}
//////////////////////////////////////////////////////////////
 
int removeWordInTrie(Trie* p) {
    int cnt_removed = p->word;
    if (cnt_removed > 0) {
        p->word = 0;
        for (p = p->parent; p != NULL; p = p->parent) {
            p->cwds -= cnt_removed;
        }
    }
    return cnt_removed;
}
 
//#include <stdio.h> // for debugging
//int print = 0;     // for debugging
//char prefix[10];   // for debugging
char postfix[10];
int cmp_len = 0;
int word_cnt = 0;
int depth = 0;
             // fl                 g              count        cmp_len는 *뒤의 string len
void dfsTrie(Trie*p, const char* cmp_str, int search_opt = COUNT) {
    if (p->word > 0 && depth >= cmp_len) {  // 단어 count가 있어야 하고,,, cmp_len 보다 depth(확인한 문자)길이가 길어야 한다.
        if (cmp_len == 0 || 0 == mstrcmp(cmp_str, &postfix[depth - cmp_len])) {
            //if (print) printf("%s%s\n", prefix, postfix);  // for debugging
            if (search_opt == COUNT)
                word_cnt += p->word;
            else
                word_cnt += removeWordInTrie(p);
        }
 
    }
    if (p->cwds == 0)
        return;
    for (rint i = 0, rem = p->cn; i < ALPHABET_SIZE && rem > 0; ++i) {  //   아. 이게 자식노드 개수를 count하고, 그거 까지만 돌아가게 한거구나
        if (p->c[i] != NULL) {
            postfix[depth++] = 'a' + i;
            postfix[depth] = NULL;
            dfsTrie(p->c[i], cmp_str, search_opt);
            postfix[--depth] = NULL;
            --rem;
        }
    }
}
//                         fl의 l        *g
int countOrRemoveTrie(Trie* mid, char* cmp_str, int search_opt = COUNT) {
    if (mid == NULL) {
        return 0;
    }
 
    if (*cmp_str == NULL) {      // *cmp_str 이 NULL 이라는 것은 단어 다음의 NULL을 가리킨다는 걸 나타냄 
        if (search_opt == COUNT) {
            return mid->word;          // 단어의 끝이므로, mid->word 카운트를 넘겨준다.
        }
        else {
            return removeWordInTrie(mid);
        }
    }
    else if (*cmp_str == '*' && *(cmp_str + 1) == NULL && search_opt == COUNT) { //  fla* 와 같이 맨 마지막이 * 라는 뜻.
        return mid->word + mid->cwds;  // mid가 word이면.. 해당 word 카운트와  해당 문자로 시작하는 word가 뒤에 더 있으니 cwds 를 더해야 된다. 
    }
    else {
        ++cmp_str; // skip asterisk.
        cmp_len = mstrlen(cmp_str);  // cmp_str 은 g 임,  mid는 l 임
        postfix[0] = NULL;
        word_cnt = depth = 0;
        dfsTrie(mid, cmp_str, search_opt);
        return word_cnt;
    }
}
 
void insertTrie(Trie *root, const char* str) {
    register Trie *p = root;
 
    for (rint i = 0; str[i]; i++) {
        int idx = str[i] - 'a';
        if (p->c[idx] == NULL) {
            p->c[idx] = getNode(p);
            p->cn++;    // p가 몇개의 node point를 갖는지 저장하는 것임.
        }
        p->cwds++;      // 아래 for문대신 넣으면 되는 것임. 단, p->child 로 이동하기 전에 넣어야 한다. 
                        // 해당 문자로 시작하는 단어가 몇개가 있는지를 check 할 수 있는 것임. 
        p = p->c[idx];
    }
    p->word++;
    //for (p = p->parent; p != NULL; p = p->parent) {
    //  p->cwds++;
    //}
    int a = 1;
}
 
Trie* searchTrie(Trie *root, const char* str) {
    register Trie *p = root;
    rint i;
    for (i = 0; str[i] != NULL && str[i] != '*'; i++) {
        int idx = str[i] - 'a';
        if (p->c[idx] == NULL) {
            return NULL;
        }
        //prefix[i] = str[i]; // for debugging
        p = p->c[idx];
    }
    //prefix[i] = NULL; // for debugging
    return p;
}
 
Trie *head_trie, *tail_trie;
 
//////////////////////////////////////////////////////////////
 
void init() {
    pn = 0;
    head_trie = getNode(NULL);
    tail_trie = getNode(NULL);
}
 
void addWord(char str[]) {
    //printf("addWord = %s \n", str);
    insertTrie(head_trie, str);
    int len = mstrlen(str);
 
    char tail_str[10];
    reverseWord(tail_str, str, len);
    insertTrie(tail_trie, tail_str);
    int a = 1;
}
 
int searchWord(char str[]) {
    //printf("searchWord = %s \n", str);
    int len = mstrlen(str);
    int head_len = 0;
 
    // head_len 와 tail_len을 구한다.
    while (str[head_len] != '*' && str[head_len] != NULL) { // * 앞까지의 head_len를 구한다.
        head_len++;                                         // fl*g  이면 head_len는 2
    }
    int tail_len = len == head_len ? len : len - head_len - 1; // tail_len 는 1
 
    if (head_len >= tail_len) {
        Trie *p = searchTrie(head_trie, str);
        return countOrRemoveTrie(p, &str[head_len], COUNT);
    }
    else { // tail_len이 더 크면
        char tail_str[10];
        reverseWord(tail_str, str, len);
 
        Trie *p = searchTrie(tail_trie, tail_str);
        return countOrRemoveTrie(p, &tail_str[tail_len], COUNT);
    }
}
 
int removeWord(char str[]) {
    int len = mstrlen(str);
    int head_len = 0;
    while (str[head_len] != '*' && str[head_len] != NULL) head_len++;
    int tail_len = len == head_len ? len : len - head_len - 1;
 
    Trie *p;
    p = searchTrie(head_trie, str);
    int cnt1 = countOrRemoveTrie(p, &str[head_len], REMOVE);
 
    char tail_str[10];
    reverseWord(tail_str, str, len);
    p = searchTrie(tail_trie, tail_str);
    int cnt2 = countOrRemoveTrie(p, &tail_str[tail_len], REMOVE);
    return cnt2;
}
