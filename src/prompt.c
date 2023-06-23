#include "prompt.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "editor.h"
#include "input.h"
#include "output.h"
#include "status.h"
#include "terminal.h"

#define PROMPT_BUF_INIT_SIZE 64
#define PROMPT_BUF_GROWTH_RATE 2.0f
char* editorPrompt(char* prompt, int state, void (*callback)(char*, int)) {
    gEditor.state = state;

    // TODO: Make prompt buffer a row
    size_t bufsize = PROMPT_BUF_INIT_SIZE;
    char* buf = malloc_s(bufsize);

    size_t buflen = 0;
    buf[0] = '\0';

    int start = 0;
    while (prompt[start] != '\0' && prompt[start] != '%') {
        start++;
    }
    gEditor.px = start;
    while (true) {
        editorSetStatusMsg(prompt, buf);
        editorRefreshScreen();
        int x, y;
        int c = editorReadKey(&x, &y);
        size_t idx = gEditor.px - start;
        switch (c) {
            case DEL_KEY:
                if (idx != buflen)
                    idx++;
                else
                    break;
                // fall through
            case CTRL_KEY('h'):
            case BACKSPACE:
                if (idx != 0) {
                    memmove(&buf[idx - 1], &buf[idx], buflen - idx + 1);
                    buflen--;
                    idx--;
                    if (callback)
                        callback(buf, c);
                }
                break;

            case CTRL_KEY('v'): {
                if (!gEditor.clipboard.size)
                    break;
                // Only paste the first line
                const char* paste_buf = gEditor.clipboard.data[0];
                size_t paste_len = strlen(paste_buf);
                if (paste_len == 0)
                    break;

                if (buflen + paste_len >= bufsize) {
                    bufsize = buflen + paste_len + 1;
                    bufsize *= PROMPT_BUF_GROWTH_RATE;
                    buf = realloc_s(buf, bufsize);
                }
                memmove(&buf[idx + paste_len], &buf[idx], buflen - idx + 1);
                memcpy(&buf[idx], paste_buf, paste_len);
                buflen += paste_len;
                idx += paste_len;

                if (callback)
                    callback(buf, c);

                break;
            }

            case HOME_KEY:
                idx = 0;
                break;

            case END_KEY:
                idx = buflen;
                break;

            case ARROW_LEFT:
                if (idx != 0)
                    idx--;
                break;

            case ARROW_RIGHT:
                if (idx < buflen)
                    idx++;
                break;

            case WHEEL_UP:
                editorScroll(-3);
                break;

            case WHEEL_DOWN:
                editorScroll(3);
                break;

            case MOUSE_PRESSED: {
                int field = getMousePosField(x, y);
                if (field == FIELD_PROMPT) {
                    if (x >= start) {
                        size_t cx = x - start;
                        if (cx < buflen)
                            idx = cx;
                        else
                            idx = buflen;
                    }
                    break;
                } else if (field == FIELD_TEXT) {
                    mousePosToEditorPos(&x, &y);
                    gCurFile->cursor.y = y;
                    gCurFile->cursor.x = editorRowRxToCx(&gCurFile->row[y], x);
                    gCurFile->sx = x;
                }
                // fall through
            }
            case CTRL_KEY('q'):
            case ESC:
                editorSetStatusMsg("");
                gEditor.state = EDIT_MODE;
                if (callback)
                    callback(buf, c);
                free(buf);
                return NULL;

            case '\r':
                if (buflen != 0) {
                    editorSetStatusMsg("");
                    gEditor.state = EDIT_MODE;
                    if (callback)
                        callback(buf, c);
                    return buf;
                }
                break;

            default:
                if (isprint(c)) {
                    if (buflen == bufsize - 1) {
                        bufsize *= PROMPT_BUF_GROWTH_RATE;
                        buf = realloc_s(buf, bufsize);
                    }
                    memmove(&buf[idx + 1], &buf[idx], buflen - idx + 1);
                    buf[idx] = c;
                    buflen++;
                    idx++;
                }

                if (callback)
                    callback(buf, c);
                break;
        }
        gEditor.px = start + idx;
    }
}

static void editorGotoCallback(char* query, int key) {
    if (query == NULL || key == ESC)
        return;

    int line = atoi(query);
    if (line < 0) {
        line = gCurFile->num_rows + 1 + line;
    }

    if (line > 0 && line <= gCurFile->num_rows) {
        gCurFile->cursor.x = 0;
        gCurFile->sx = 0;
        gCurFile->cursor.y = line - 1;
        editorScrollToCursorCenter();
    } else {
        editorSetStatusMsg("Type a line number between 1 to %d.",
                           gCurFile->num_rows);
    }
}

void editorGotoLine(void) {
    char* query =
        editorPrompt("Goto line: %s", GOTO_LINE_MODE, editorGotoCallback);
    if (query) {
        free(query);
    }
}
