#include "Search.h"
pthread_mutex_t mutex;
char * modKeyword;
ThreadPool tFilesScan;
int counter = 0;
int numOfResults = 0;
// Function to search for the keyword in files in a given file
void* search_keyword_in_file(void* arg) {
	char* path = (char *)arg;
	char line[MAX_KEYWORD_LEN];
	int line_number = 0;
	char * res;
	int loc;
	int index = 0;
	// Open file
	FILE* file = fopen(path, "r");

	// Search for the keyword in the file
	if (file)
	{
		while (fgets(line, sizeof(line), file)) {
			line_number++;
			res = strcasestr(line, modKeyword);
			if (res) {
				// Keyword found, store the result in the shared data structure
				loc = res - line;
				printf("\033[1;33m Found At line %d in File %s \033[0m \n \n ", line_number, path);
				for (index; index < loc; index++)
					putchar(line[index]);

				printf("\033[1;32m");

				for (index; index < loc + strlen(modKeyword); index++)
					putchar(line[index]);

				printf("\033[0m");

				for (index; index < strlen(line); index++)
					putchar(line[index]);
				puts("\n\n");
				pthread_mutex_lock(&mutex);
				numOfResults++;
				pthread_mutex_unlock(&mutex);
				break;
			}
		}
		fclose(file);
	}
	free(path);
}
// Function to search for the keyword in files in a given directory
void  search_keyword_in_directory(char * directory) {
	char *file_path;
	int index;
	int size1, size2;
	struct dirent* entry;
	DIR* dir = opendir(directory);
	if (dir)
	{
		size1 = strlen(directory);
		while ((entry = readdir(dir)) != NULL)
		{
			index = 0;
			size2 = strlen(entry->d_name);
			file_path = (char *)malloc(sizeof(char)*(size1 + size2 + 2));
			for (index; index < size1; index++)file_path[index] = directory[index];
			file_path[index++] = '/';
			for (index; index < (size1 + size2 + 1); index++)file_path[index] = entry->d_name[index - (size1 + 1)];
			file_path[index] = '\0';
			if (entry->d_type == DT_REG)
			{
				counter++;
				thread_pool_add_task(&tFilesScan, search_keyword_in_file, (void *)file_path);
			}
			else if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && entry->d_type == DT_DIR)
			{
				search_keyword_in_directory(file_path);
				free(file_path);
			}

		}
		closedir(dir);
	}
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Usage: %s <directory> <keyword>\n", argv[0]);
		return 1;
	}
	thread_pool_init(&tFilesScan, 8);
	char* directory = argv[1];
	char* keyword = argv[2];
	// Initialize the shared data structure and mutex
	modKeyword = tolowercase(keyword);
	search_keyword_in_directory((void*)directory);
	thread_pool_terminate(&tFilesScan);
	printf("\n \n \n Found the keyword %s in %d Files out of %d. \n", keyword, numOfResults, counter);
	return 0;
}
char * strcasestr(const char *s, const char *find)
{
	char c, sc;
	size_t len;

	if ((c = *find++) != 0) {
		c = tolower((unsigned char)c);
		len = strlen(find);
		do {
			do {
				if ((sc = *s++) == 0)
					return (NULL);
			} while ((unsigned char)sc != c);
		} while (strncasecmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}
char * tolowercase(char * str)
{
	char *modStr = (char *)malloc(sizeof(char)*(strlen(str) + 1));
	int y = 0;
	for (y; y < strlen(str); y++)
	{
		modStr[y] = tolower(str[y]);
	}
	return modStr;
}
