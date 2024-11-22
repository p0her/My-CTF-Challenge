#include "main.h"

#define MAX_SLOT_ID 0x10
#define MAX_DESCRIPTION_SIZE 0x10

uint8_t slot_id;

typedef struct _user_t {
    uint8_t *name;
    uint8_t *description;
    uint8_t age;
} user_t;

int is_admin;
uint32_t* mt;
int mti;

user_t *notes[MAX_SLOT_ID];
unsigned int name_sizes[MAX_SLOT_ID];

void initalize() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    mt = malloc(sizeof(uint32_t) * N + 1);
    memset(mt, 0, sizeof(uint32_t) * N + 1);
    init_genrand(get_random());
}

void init_genrand(uint32_t seed) {
    int _mti;
    mt[0] = seed & 0xffffffffUL;
    for(_mti = 1;  _mti < N; _mti++) {
        mt[_mti] = (F * (mt[_mti-1] ^ (mt[_mti-1] >> 30)) + _mti);
        mt[_mti] &= 0xffffffffUL;
    }
    mti = _mti;
}

uint32_t genrand_uint32() {
    uint32_t y;
    static const uint32_t mag01[2] = { 0x0U, MATRIX_A };
    if(mti >= N) {
        int kk;
        for(kk = 0; kk < N - M; kk++) {
            y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }
        for(;kk<N - 1; kk++) {
            y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1U];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1U];

        mti = 0;
    }
    y = mt[mti++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680U;
    y ^= (y << 15) & 0xefc60000U;
    y ^= (y >> 18);
    return y;
}

uint32_t get_random() {
    uint32_t buf;
    syscall(__NR_getrandom, &buf, 4, GRND_RANDOM, 0, 0, 0);
    return buf;
}

void create_user() {
    if(slot_id > MAX_SLOT_ID) {
        slot_id = 0;   
        return;
    }
    uint32_t size;
    notes[slot_id] = malloc(sizeof(user_t));
    memset(notes[slot_id], 0, sizeof(user_t));
    write(1, "name size > ", 12);
    scanf("%u", &size); 
    if(size >= 0x1000 || size == 0) {
        write(1, "Invalid size", 12);
        return;
    }
    name_sizes[slot_id] = size; 
    notes[slot_id]->name = malloc(name_sizes[slot_id]);
    memset(notes[slot_id]->name, 0, name_sizes[slot_id]);
    notes[slot_id]->description = malloc(MAX_DESCRIPTION_SIZE);
    memset(notes[slot_id]->description, 0, MAX_DESCRIPTION_SIZE);
    write(1, "name > ", 7);
    read(0, notes[slot_id]->name, (unsigned int)(name_sizes[slot_id] - 1));
    write(1, "description > ", 14);
    read(0, notes[slot_id]->description, MAX_DESCRIPTION_SIZE - 1);
    slot_id++;
    write(1, "Done!", 6);
    return;
}

void edit_user() {
    uint8_t idx;
    write(1, "edit user slot idx > ", 21);
    scanf("%hhu", &idx);
    if(idx >= MAX_SLOT_ID) return;
    if(!notes[idx]) {
        write(1, "user not exist\n", 15);
        return;
    }
    write(1, "name > ", 7);
    read(0, notes[idx]->name, (unsigned int)(name_sizes[idx] - 1));
    write(1, "description > ", 14);
    read(0, notes[idx]->description, MAX_DESCRIPTION_SIZE - 1);
    write(1, "Done!", 5);
    return;
}

void win_list() {
    for(int i = 0; i < 10; i++) write(1, "*", 1);
    write(1, " Winner ", 8);
    for(int i = 0; i < 10; i++) write(1, "*", 1);
    for(int i = 0; i < MAX_SLOT_ID; i++) {
        if(!notes[i]) return;
        printf("\n[%d] name : ", i);
        write(1, notes[i]->name, (unsigned int)(name_sizes[i] - 1));
        printf("\n[%d] description : ", i);
        write(1, notes[i]->description, MAX_DESCRIPTION_SIZE - 1);
        write(1, "\n----", 6);
    }
}

void play_game() {
    int win_cnt = 0;
    uint32_t lotto[7];
    uint32_t t[7];
    uint8_t arr[7];
    memset(arr, 0, sizeof(arr) / sizeof(uint8_t));
    memset(t, 0, sizeof(t) / sizeof(uint32_t));
    memset(lotto, 0, sizeof(lotto) / sizeof(uint32_t));
    write(1, "win probability is 1/8140000....\n", 33);
    for(int i = 0; i < 6; i++) {
        write(1, "> ", 2);
        scanf("%hhu", &arr[i]);
    }
    for(int i = 0; i < 6; i++)
        t[i] = genrand_uint32();

    for(int i = 0; i < 6; i++) 
        lotto[i] = (t[i] % 45) + 1;

    for(int i = 0; i < 6; i++)
        if(lotto[i] == arr[i]) win_cnt++;

    if(win_cnt == 6) {
        write(1, "** Win!!! **\n", 13);
        write(1, "Save your information\n", 22);
        create_user();
        return;
    }

    write(1, "!! Failed !!\n", 13);
    write(1, "Do you want to see what value of lotto?\n> ", 42);
    char sel[2];
    scanf(" %c", sel);
    if(sel[0] == 'y') {
        write(1, "how much do you want?\n> ", 24);
        int cnt;
        scanf("%d", &cnt);
        if(cnt < 0 || cnt > 6) {
            write(1, "No!!\n", 5);
            return;
        }
        write(1, "Lotto Numbers : [", 17);
        for(int idx = 0; idx <= (uint8_t)(cnt - 1); idx++) {
            if(idx == cnt - 1) printf("%u", lotto[idx]);
            else printf("%u, ", lotto[idx]);
        }
        write(1, "]\n", 2);
    } else return;
}

void generate_md5(char *id, unsigned char* digest) {
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, id, strlen(id));
    MD5_Final(digest, &ctx);
}

int check_id(char *id) {
    char admin_id[0x10];
    sprintf(admin_id, "admin%08X", genrand_uint32());
    unsigned char user_input_digest[MD5_DIGEST_LENGTH];
    unsigned char admin_digest[MD5_DIGEST_LENGTH];
    char user_input_hexdigest[2*MD5_DIGEST_LENGTH + 1];
    char admin_hexdigest[2*MD5_DIGEST_LENGTH + 1];

    generate_md5(id, user_input_digest);
    generate_md5(admin_id, admin_digest);
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(user_input_hexdigest+2*i, "%02X", user_input_digest[i]);
        sprintf(admin_hexdigest+2*i, "%02X", admin_digest[i]);
    }
    if(!strcmp(user_input_hexdigest, admin_hexdigest)) return 1;
    return 0;
} 

int check_pw(char *pw) {
    char admin_pw[0x9];
    sprintf(admin_pw, "%08X", genrand_uint32());
    unsigned char user_input_digest[MD5_DIGEST_LENGTH];
    unsigned char admin_pw_digest[MD5_DIGEST_LENGTH];
    char user_input_hexdigest[2*MD5_DIGEST_LENGTH + 1];
    char admin_pw_hexdigest[2*MD5_DIGEST_LENGTH + 1];
    generate_md5(pw, user_input_digest);
    generate_md5(admin_pw, admin_pw_digest);
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(user_input_hexdigest+2*i, "%02X", user_input_digest[i]);
        sprintf(admin_pw_hexdigest+2*i, "%02X", admin_pw_digest[i]);
    }
    if (!strcmp(user_input_hexdigest, admin_pw_hexdigest)) return 1;
    return 0;
}

void admin() {
    char id[0x10]; 
    char pw[0x10];
    memset(id, 0, 0x10);
    memset(pw, 0, 0x10); 
    write(1, "ID > ", 5);
    read(0, id, 0x10);
    write(1, "PW > ", 5);
    read(0, pw, 0x10);
    printf("Your ID : %s\n", id);
    printf("Your PW : %s\n", pw);
    if(!check_id(id) || !check_pw(pw)) {
        write(1, "!! Failed Login !!\n", 19);
        return;
    }
    write(1, "** Login Success! **\n", 21);
    edit_user();
}

void print_menu() {
    write(1, "1. **kapo lotto**\n", 18);
    write(1, "2. win user list\n", 17);
    write(1, "3. exit\n", 8);
    write(1, "> ", 2);
}

int main(void) {
    initalize();
    int idx;
    while(1) {
        print_menu();
        scanf("%d", &idx);
        switch(idx) {
            case 1:
                play_game();
                break;
            case 2:
                win_list();
                break;
            case 3:
                write(1, "Bye", 3);
                exit(0);
                break;
            case 0x1007:
                admin();
                break;
        }
    }
    return 0;
}
