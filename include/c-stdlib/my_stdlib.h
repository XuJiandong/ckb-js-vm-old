#ifndef LUA_C_STDLIB_STDLIB_H_
#define LUA_C_STDLIB_STDLIB_H_

float strtof(const char *__restrict, char **__restrict);
double strtod(const char *__restrict, char **__restrict);
long double strtold(const char *__restrict, char **__restrict);
int atoi(const char *);

int abs(int);
void exit(int);
void abort(void);
#define PRId64 "ld"

#endif /* LUA_C_STDLIB_STDLIB_H_ */
