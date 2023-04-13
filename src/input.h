#ifndef INPUT_H
#define INPUT_H

char* editorPrompt(char* prompt, int state, void (*callback)(char*, int));
void editorMoveCursor(int key);
void editorProcessKeypress();

void editorScrollToCursor();
void editorScrollToCursorCenter();
void editorScroll(int dist);

void mousePosToEditorPos(int* x, int* y);
int getMousePosField(int x, int y);

#endif
