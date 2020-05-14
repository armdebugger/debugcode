all: PRELOAD_SO TEST

PRELOAD_SO: preload.c
	gcc -shared -fpic -o libpreload.so preload.c -ldl -fno-omit-frame-pointer -g -O0 -rdynamic

TEST: test.c
	gcc -o test test.c -fno-omit-frame-pointer -g -O0 -rdynamic

run: PRELOAD_SO TEST
	LD_PRELOAD=./libpreload.so ./test