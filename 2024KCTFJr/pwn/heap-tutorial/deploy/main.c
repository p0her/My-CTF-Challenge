#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define NOTES_CNT 16

typedef struct note_t {
	char content[256];
	void (*func)();
	char* name;
	uint64_t size;
} note;

note* notes[NOTES_CNT];

void initalize() {
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);
	setvbuf(stderr, 0, 2, 0);
}

void print_content(char* content) {
	printf("content : %s\n", content);
}

void win() {
	execve("/bin/sh", 0, 0);
}

void make_note() {
	uint64_t idx;
	printf("idx > ");
	scanf("%lu", &idx);
	if(idx >= NOTES_CNT) {
		puts("out of index!");
		return;
	}
	notes[idx] = malloc(sizeof(note));
	uint64_t size;
	printf("content size > ");
	scanf("%lu", &size);
	notes[idx]->name = malloc(0x100);
	notes[idx]->size = size;
	printf("name > ");
	read(0, notes[idx]->name, 0x100 - 1);
	printf("content > ");
	read(0, notes[idx]->content, notes[idx]->size - 1);
	notes[idx]->func = print_content;
}

void delete_note() {
	uint64_t idx;
	printf("idx > ");
	scanf("%lu", &idx);
	if(idx >= NOTES_CNT) {
		puts("out of index!");
		return;
	}
	if(notes[idx]) {
		free(notes[idx]->name);
		free(notes[idx]);
		notes[idx] = 0;
		printf("delete done!\n");
	}
}

void edit_note() {
	uint64_t idx;
	printf("idx > ");
	scanf("%lu", &idx);
	if(idx >= NOTES_CNT) {
		puts("out of index!");
		return;
	}
	if(notes[idx]) {
		printf("name > "); 
		read(0, notes[idx]->name, 0x100 - 1);
		printf("content > ");
		read(0, notes[idx]->content, (uint32_t)(notes[idx]->size-1));
	}
}

void print() {
	uint64_t idx;
	printf("idx > ");
	scanf("%lu", &idx);
	if(idx >= NOTES_CNT) {
		puts("out of index!");
		return;
	}
	if(notes[idx]) {
		printf("name : %s\n", notes[idx]->name);
		notes[idx]->func(notes[idx]->content);
	}
}

void print_menu() {
	puts("1. make note");
	puts("2. delete note");
	puts("3. edit note");
	puts("4. print note");
	puts("5. exit");
	printf("> ");
}

int main() {
	initalize();
	while(1) {
		print_menu();
		int sel;
		scanf("%d", &sel);
		switch(sel) {
			case 1:
				make_note();
				break;
			case 2:
				delete_note();
				break; 
			case 3:
				edit_note();
				break;
			case 4:
				print(); 
				break;
			case 5:
				exit(0);
				break;
			default:
				continue;
		}
	}
}