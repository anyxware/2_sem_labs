#ifndef TASK_H
#define TASK_H

char* get_str();
int infix_to_postfix(char* infix, char* postfix);
void show(char* postfix, int len);
void clean(char* infix, char* postfix);

#endif //TASK_H