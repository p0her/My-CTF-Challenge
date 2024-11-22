#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 0x10
#define CONTENT_SIZE 0x100
#define DIARY_SIZE 0x10

#define ADD 1
#define EDIT 2
#define DELETE 3
#define PRINT 4
#define EXIT 5

__asm__(
    "pop %rdi;"
    "ret;"
    "pop %rsi;"
    "ret;");

struct diary
{
    unsigned int page;
    char name[NAME_SIZE];
    unsigned char content[CONTENT_SIZE];
    struct tm tm;
    int is_used;
};

void init()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

void init_diary(struct diary *diary)
{
    for (int i = 0; i < DIARY_SIZE; i++)
    {
        diary[i].page = i + 1;
        memset(diary[i].name, 0, NAME_SIZE);
        memset(diary[i].content, 0, CONTENT_SIZE);
        time_t t = time(NULL);
        diary[i].tm = *localtime(&t);
        diary[i].is_used = 0;
    }
}

void print_menu()
{
    puts("1. add diary");
    puts("2. edit diary");
    puts("3. delete diary");
    puts("4. print diary");
    puts("5. exit");
}

unsigned int read_page()
{
    unsigned int page;
    printf("diary page > ");
    scanf("%u", &page);
    if (page >= DIARY_SIZE)
    {
        puts("[!] out of bound");
        exit(1);
    }
    return page;
}

void add_diary(struct diary *diary)
{
    unsigned int page = read_page();
    if (diary[page].is_used)
    {
        puts("[!] already added diary.");
        exit(1);
    }
    printf("diary name > ");
    scanf("%15s", diary[page].name);
    printf("diary content > ");
    scanf("%255s", diary[page].content);
    time_t t = time(NULL);
    diary[page].tm = *localtime(&t);
    diary[page].is_used = 1;
}

void edit_diary(struct diary *diary)
{
    unsigned int page = read_page();
    if (!diary[page].is_used)
    {
        puts("[!] Not added yet diary.");
        exit(1);
    }
    printf("diary name > ");
    scanf("%15s", diary[page].name);
    printf("diary content > ");
    scanf("%s", diary[page].content); // stack buffer overflow
}

void delete_diary(struct diary *diary)
{
    unsigned int page = read_page();
    if (!diary[page].is_used)
    {
        puts("[!] Not added yet diary.");
        exit(1);
    }
    diary[page].is_used = 0;
}

void print_diary(struct diary *diary)
{
    for (int page = 0; page < DIARY_SIZE; page++)
    {
        if (diary[page].is_used)
        {
            printf("**** diary[%d] ****\n", page);
            printf("name : %s\n", diary[page].name);
            printf("content : %s\n", diary[page].content);
            printf("diary time : %s\n", asctime(&diary[page].tm));
            printf("------------------\n");
        }
    }
}

int main()
{
    struct diary diary[DIARY_SIZE];
    init();
    init_diary(diary);
    while (1)
    {
        int choose;
        print_menu();
        printf("> ");
        scanf("%d", &choose);
        switch (choose)
        {
        case ADD:
            add_diary(diary);
            break;
        case EDIT:
            edit_diary(diary);
            break;
        case DELETE:
            delete_diary(diary);
            break;
        case PRINT:
            print_diary(diary);
            break;
        case EXIT:
            return 0;
        }
    }
}