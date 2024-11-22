#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NAME_SIZE 0x10
#define CONTENT_SIZE 0x100
#define DIARY_SIZE 0x10

#define ADD 1
#define EDIT 2
#define DELETE 3
#define PRINT 4
#define EXIT 5

struct diary
{
    unsigned int page;
    struct tm tm;
    char *name;
    unsigned char *content;
    void (*fp)(struct diary *);
};

struct diary *diary[DIARY_SIZE];

void win()
{
    system("/bin/sh");
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

void print_diary_fp(struct diary *diary)
{
    printf("**** diary[%d] ****\n", diary->page);
    printf("name : %s\n", diary->name);
    printf("content : %s\n", diary->content);
    printf("diary time : %s\n", asctime(&diary->tm));
    printf("------------------\n");
}

void add_diary()
{
    unsigned int page = read_page();
    if (diary[page])
    {
        puts("[!] already added diary.");
        exit(1);
    }
    diary[page] = malloc(sizeof(struct diary));
    diary[page]->page = page + 1;
    diary[page]->name = malloc(NAME_SIZE);
    diary[page]->content = malloc(CONTENT_SIZE);
    printf("diary name > ");
    scanf("%15s", diary[page]->name);
    printf("diary content > ");
    scanf("%255s", diary[page]->content);
    time_t t = time(NULL);
    diary[page]->tm = *localtime(&t);
    diary[page]->fp = &print_diary_fp;
}

void edit_diary()
{
    unsigned int page = read_page();
    if (!diary[page])
    {
        puts("[!] Not added yet diary.");
        exit(1);
    }
    printf("diary name > ");
    scanf("%15s", diary[page]->name);
    printf("diary content > ");
    scanf("%s", diary[page]->content);
    time_t t = time(NULL);
    diary[page]->tm = *localtime(&t);
}

void delete_diary()
{
    unsigned int page = read_page();
    if (!diary[page])
    {
        puts("[!] Not added yet diary.");
        exit(1);
    }
    free(diary[page]->name);
    free(diary[page]->content);
    free(diary[page]); // double free here!
}

void print_diary()
{
    unsigned int page = read_page();
    if (diary[page])
        diary[page]->fp(diary[page]);
}

void print_menu()
{
    puts("1. add diary");
    puts("2. edit diary");
    puts("3. delete diary");
    puts("4. print diary");
    puts("5. exit");
}

void init()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

int main()
{
    init();
    while (1)
    {
        int choose;
        print_menu();
        printf("> ");
        scanf("%d", &choose);
        switch (choose)
        {
        case ADD:
            add_diary();
            break;
        case EDIT:
            edit_diary();
            break;
        case DELETE:
            delete_diary();
            break;
        case PRINT:
            print_diary();
            break;
        case EXIT:
            return 0;
        }
    }
}