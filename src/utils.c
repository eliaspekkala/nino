#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "defines.h"

void abufAppend(abuf* ab, const char* s, int len) {
    char* new = realloc(ab->buf, ab->len + len);

    if (new == NULL)
        return;
    memcpy(&new[ab->len], s, len);
    ab->buf = new;
    ab->len += len;
}

void abufFree(abuf* ab) {
    free(ab->buf);
}

int editorRowCxToRx(EditorRow* row, int cx) {
    int rx = 0;
    for (int i = 0; i < cx; i++) {
        if (row->data[i] == '\t') {
            rx += (TAB_SIZE - 1) - (rx % TAB_SIZE);
        }
        rx++;
    }
    return rx;
}

int editorRowRxToCx(EditorRow* row, int rx) {
    int cur_rx = 0;
    int cx;
    for (cx = 0; cx < row->size; cx++) {
        if (row->data[cx] == '\t')
            cur_rx += (TAB_SIZE - 1) - (cur_rx % TAB_SIZE);
        cur_rx++;
        if (cur_rx > rx)
            return cx;
    }
    return cx;
}

int editorRowSxToCx(EditorRow* row, int sx) {
    if (sx <= 0)
        return 0;
    int cx = 0;
    int rx = 0;
    int rx2 = 0;
    while (cx < row->size && rx < sx) {
        rx2 = rx;
        if (row->data[cx] == '\t') {
            rx += (TAB_SIZE - 1) - (rx % TAB_SIZE);
        }
        rx++;
        cx++;
    }
    if (rx - sx >= sx - rx2) {
        cx--;
    }
    return cx;
}
