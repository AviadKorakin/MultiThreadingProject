#ifndef SEARCH_H
#define SEARCH_H
#define MAX_FILE_PATH 255
#define MAX_KEYWORD_LEN 100
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include "Queue.h"
#include "ThreadPool.h"
// Shared data structure to store search results
char * tolowercase(char * str);
char * strcasestr(const char *s,const char *find);
void* search_keyword_in_file(void* arg);
void search_keyword_in_directory(char * directory);
#endif /* SEARCH_H */
