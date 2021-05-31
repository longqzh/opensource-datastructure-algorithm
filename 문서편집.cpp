#define R register
typedef unsigned int UD;
 
#define MAX_ROW (1000)
#define MAX_ID (9999)
 
enum USER_STAT { EDIT, CONFLICT };
 
UD width;
UD char_cnt[MAX_ROW];
UD row_cnt[MAX_ROW];
 
struct USER {
    enum USER_STAT stat;
    UD row;
    UD col;
    UD end;
    int offset;
    struct USER* prev;
    struct USER* next;
} user[MAX_ID + 1];
 
USER wait_for_edit[MAX_ROW];
 
inline void pending(int row, int id) {
    R USER* w = &user[id];
    w->next = wait_for_edit[row].next;
    w->next->prev = w;
    w->prev = &wait_for_edit[row];
    w->prev->next = w;
}
 
void Init(int width, int length, char str[])
{
    ::width = width;
    R USER* temp = wait_for_edit;
    for (R int i = 0; i < MAX_ROW; i++) {
        temp->prev = temp->next = temp;
        temp++;
    }
 
    R UD row = 0;
    R UD cnt = 0;
    while (length--) {
        if (*str == '\n') {
            char_cnt[row] = cnt;
            row_cnt[row++] = ((cnt - 1) / width) + 1;
            cnt = 0;
        }
        else {
            cnt++;
        }
        str++;
    }
    char_cnt[row] = cnt;
    row_cnt[row] = ((cnt - 1) / width) + 1;
}
 
inline void find_rc(int row, int col, UD* ret_row, UD* ret_col) {
    R UD temp_row = 0;
    for (R int i = 0; i < MAX_ROW; i++) {
        R UD cur_row = row_cnt[i];
        if (row >= temp_row && row < (temp_row + cur_row)) {
            *ret_row = i;
            *ret_col = (row - temp_row) * width + col;
            return;
        }
        temp_row += cur_row;
    }
}
 
void Edit(int id, int row, int col)
{
    R USER* u = &user[id];
    u->stat = EDIT;
 
    R UD ret_row = 0, ret_col = 0;
    find_rc(row, col, &ret_row, &ret_col);
 
    u->row = ret_row;
    u->col = ret_col;
    u->end = ret_col + 1;
    pending(ret_row, id);
}
 
void Add(int id, int length, char str[])
{
    user[id].offset = length;
}
 
void Delete(int id, int length)
{
    user[id].end += length - 1;
    user[id].offset = -length;
}
 
void Substitute(int id, int length, char str[])
{
    user[id].end += length - 1;
    user[id].offset = 0;
}
 
inline void find_conflict(int row, int start, int end, int offset) {
    R USER* ptr = wait_for_edit[row].next;
    while (ptr != &wait_for_edit[row]) {
        if (ptr->col >= end) {
            ptr->col += offset;
            ptr->end += offset;
        }
        else if (ptr->end > start) {
            ptr->stat = CONFLICT;
            ptr->next->prev = ptr->prev;
            ptr->prev->next = ptr->next;
        }
        ptr = ptr->next;
    }
    char_cnt[row] += offset;
    row_cnt[row] = ((char_cnt[row] - 1) / width) + 1;
}
 
int EditDone(int id)
{
    R USER* u = &user[id];
 
    if (u->stat == CONFLICT)
        return 0;
 
    R UD row = u->row;
    R UD col = u->col;
    R UD end = u->end;
 
    find_conflict(row, col, end, u->offset);
 
    return 1;
}
