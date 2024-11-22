# Makefile

# 컴파일러
CC = gcc

# 컴파일 옵션
CFLAGS = -Wall -Wextra -O0 -no-pie -fno-stack-protector

# 대상 파일
TARGET = main

# 소스 파일
SRC = main.c

# 기본 규칙
all: $(TARGET)

# 실행 파일 생성 규칙
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# 정리 규칙
clean:
	rm -f $(TARGET)
