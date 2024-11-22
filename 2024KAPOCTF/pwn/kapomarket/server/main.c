#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <fcntl.h>
#include <seccomp.h>

#define True 1
#define False 0
#define MAX_HEAP_SIZE 0x40000
#define MAX_SLOT 0x11
#define MAX_USER 8

#define BUY 1
#define SELL 2
#define INFO 3
#define LOGOUT 4

#define CREATE_USER 1
#define SELECT_USER 2
#define EDIT_USER 3
#define INFO_USER 4

typedef struct _t_slot {
    unsigned char *ptr;
    unsigned long price;
    unsigned char *description;
} t_slot;

typedef struct _t_user {
    unsigned char *name;
    unsigned long long len;
    unsigned long long money;
    t_slot *slots[MAX_SLOT];
} t_user;

t_user *users[MAX_USER];
uint8_t slot_cnts[MAX_USER];

uint8_t is_login = 0;
uint8_t user_cnt = 0;
uint8_t current_user_idx = -1;

const char *things[] = {
    "IPhone 99",
    "Macbook Pro 97inch",
    "KaPochip 65536g",
    "ChatGPT 31",
    "Japanese Wagyu 1024g",
    "flag"
};

const char *thing_description[] = {
    "It's New-Generation IPhone. it supports flip, fly, pocket pistol",
    "Do you want big-size labtop?",
    "The Chip, KaPochip is super-salty and tasty.",
    "ChatGPT 31 is Artificial Superintelligence.",
    "Eat some protein plz.",
    "** secret **"
};

unsigned long long prices[] = {
    1024,
    2048,
    512,
    4096,
    65536,
    (1<<31)
};

void initalize() {
    setvbuf(stdin, 0, 2, 0);    
    setvbuf(stdout, 0, 2, 0);   
    setvbuf(stderr, 0, 2, 0);   
}

int check_idx(unsigned long long min, unsigned long long idx, unsigned long long max) { // check [min, idx)
    if(min <= idx < max) return True;
    else return False;
}

void print_menu(int login) {
    if(login) {
        puts("1. buy");
        puts("2. sell");
        puts("3. info");
        puts("4. logout");
    } else {
        puts("1. create user");
        puts("2. select user");
        puts("3. edit user");
        puts("4. info");
        puts("5. exit");
    } 
    printf("> ");
}

void buy_list() {
    puts("========== KaPo Market ==========");
    uint8_t MAX_THING = 5;
    if(users[current_user_idx]->money >= (1<<31)) MAX_THING=6;
    printf("%llu\n", users[current_user_idx]->money);
    for(int idx = 0; idx < MAX_THING; idx++) {
        printf("%d. %s\n", idx + 1, things[idx]);
        printf("%s\n", thing_description[idx]);
        printf("**********************\n");
    }
}

void buy() {
    while(True) {
        if(slot_cnts[current_user_idx] >= MAX_SLOT) {
            puts("[!] slot is full!!");
            return;
        }
        buy_list();
        printf("> ");
        int sel;
        scanf("%d", &sel);
        uint8_t MAX_SEL = 5;
        if(users[current_user_idx]->money >= (1<<31)) MAX_SEL = 6;
        if(0 <= sel-1 && sel-1 < MAX_SEL) {
            if(users[current_user_idx]->money < 0) {
                puts("[!] Not Enough Money!!");
                continue;
            }
            users[current_user_idx]->slots[slot_cnts[current_user_idx]] = malloc(sizeof(t_slot));
            users[current_user_idx]->slots[slot_cnts[current_user_idx]]->ptr = malloc(strlen(things[sel-1]));
            users[current_user_idx]->slots[slot_cnts[current_user_idx]]->description = malloc(strlen(thing_description[sel-1]));
            strcpy(users[current_user_idx]->slots[slot_cnts[current_user_idx]]->ptr, things[sel-1]); 
            strcpy(users[current_user_idx]->slots[slot_cnts[current_user_idx]]->description, thing_description[sel-1]);
            users[current_user_idx]->slots[slot_cnts[current_user_idx]]->price = prices[sel-1];
            users[current_user_idx]->money -= prices[sel-1];
            slot_cnts[current_user_idx]++;
            puts("[*] Buy Done!");
            break;
        } else {
            puts("[!] Wrong Choice");
            continue;
        }
    }
}

void view_slots() {
    printf("-------- slots --------\n");
    for(int idx = 0; idx < slot_cnts[current_user_idx]; idx++) {
        if(users[current_user_idx]->slots[idx]->price == (1<<31))
            printf("%d. %s(%lld$)\ndescription : %s\n", idx+1, users[current_user_idx]->slots[idx]->ptr, 
                &read, users[current_user_idx]->slots[idx]->description);
        else printf("%d. %s(%lld$)\ndescription : %s\n", idx+1, users[current_user_idx]->slots[idx]->ptr, 
                users[current_user_idx]->slots[idx]->price, users[current_user_idx]->slots[idx]->description);
    }
}

void sell() {
    if(slot_cnts[current_user_idx] == 0) {
        puts("[!] slot is empty");
        return;
    }
    
    view_slots();

    users[current_user_idx]->money += users[current_user_idx]->slots[slot_cnts[current_user_idx] - 1]->price;
    free(users[current_user_idx]->slots[slot_cnts[current_user_idx] - 1]->ptr);
    free(users[current_user_idx]->slots[slot_cnts[current_user_idx] - 1]->description);
    free(users[current_user_idx]->slots[slot_cnts[current_user_idx] - 1]);
    slot_cnts[current_user_idx]--;
    puts("[!] sell is done!");

}

void info() {  
    printf("name : %s\n", users[current_user_idx]->name);
    printf("current money : %llu\n", users[current_user_idx]->money);
    view_slots();
}

void logout() {
    is_login = 0;
    puts("[*] logout done!");
    return;
}

void create_user() {
    if(user_cnt >= MAX_USER) {
        puts("[!] user is full!");
        return;
    }
    users[user_cnt] = malloc(sizeof(t_user));
    users[user_cnt]->money = (1<<19);
    printf("username length > ");
    scanf("%llu", &users[user_cnt]->len);
    if(users[user_cnt]->len > MAX_HEAP_SIZE) {
        puts("[!] So big!!");
        exit(1);
    }
    users[user_cnt]->name = malloc(users[user_cnt]->len); 
    printf("username > ");
    int ret = read(0, users[user_cnt]->name, users[user_cnt]->len);
    if(ret != 0) {
        puts("[*] create done!");
        user_cnt++;
        return;
    } else {
        puts("[!] what?");
        exit(1);
    }
}

void select_user() {
    uint8_t idx; 
    printf("user idx > ");
    scanf("%hhu", &idx);
    if(idx >= user_cnt) {
        puts("[!] out of bound index");
        return;
    } else {
        puts("[*] select done!");
        current_user_idx = idx;
        is_login = 1;
        return;
    }
}

void edit_user() {
    if(user_cnt == 0) {
        puts("not select user or didn't create user?");
        return;
    }
    uint8_t idx;
    printf("select user idx > ");
    scanf("%hhu", &idx);
    if(idx >= MAX_USER || !users[idx]) return;
    unsigned long long cnt = 1;
    unsigned long long off;
    uint8_t val;
    while(cnt <= users[idx]->len) {
        write(1, "offset > ", 9);
        unsigned long long off;
        scanf("%llu", &off);
        if(off == 0xffffffffffffffff) break;
        if(check_idx(0, off, users[idx]->len)) {
            write(1, "byte > ", 7);
            scanf("%hhu", &users[idx]->name[off]);
        }
        cnt++;
    }
    write(1, "[*] Done!\n", 10);  
    info_user();
    exit(0);
}

void info_user() {
    for(int idx = 0; idx < MAX_USER && users[idx]; idx++) {
        printf("%d. username : %s\nmoney : %llu\n", idx+1, users[idx]->name, users[idx]->money);
        printf("slot count : %hhu\n", slot_cnts[idx]);
    }
}

int main() {
    initalize();
    while(True) {
        print_menu(is_login);
        int sel;
        scanf("%d", &sel);
        if(is_login) {
            switch(sel) {
                case BUY:
                    buy();
                    break;
                case SELL:
                    sell();
                    break;
                case INFO:
                    info();
                    break;
                case LOGOUT:
                    logout();
                    break;
                default:
                    break;
            }
        } else {
            switch(sel) {
                case CREATE_USER:
                    create_user();
                    break;
                case SELECT_USER:
                    select_user();
                    break;
                case EDIT_USER:
                    edit_user();
                    break;
                case INFO_USER:
                    info_user();
                    break;
                case 5:
                    exit(0);
                    break;
                default:
                    break;
            }
        }
    }
}