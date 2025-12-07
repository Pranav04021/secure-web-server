#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

int buffer[SIZE];
int in = 0;
int out = 0;
int count = 0;

void delay() {
    for (volatile int i = 0; i < 1000000; ++i);
}

void produce(int item) {
    while (count == SIZE) {
        printf("Producer waiting...\n");
        delay();
    }
    buffer[in] = item;
    in = (in + 1) % SIZE;
    count++;
    printf("Produced: %d\n", item);
}

int consume() {
    while (count == 0) {
        printf("Consumer waiting...\n");
        delay();
    }
    int item = buffer[out];
    out = (out + 1) % SIZE;
    count--;
    printf("Consumed: %d\n", item);
    return item;
}

int main() {
    int item;
    for (int i = 0; i < 10; ++i) {
        item = rand() % 100;
        produce(item);
        delay();
        consume();
    }
    return 0;
}
