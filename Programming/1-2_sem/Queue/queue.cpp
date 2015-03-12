#include <cstdio>
#include <stdlib.h>
#include <conio.h>

#define MAX_SIZE 100

struct element
{
	int value;
    int priority;
    element (int v = 0, int p = 0) {
        value = v;
        priority = p;
    }
} a[MAX_SIZE];

int size;

void swap(element &a, element &b)
{
	element t = a;
	a = b;
	b = t;
}

void heapify_up(int i) {
	while (i != 0 && a[i].priority > a[(i - 1) / 2].priority) {
		swap(a[i], a[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

void heapify_down (int k)
{
 int new_el = a[k].priority;
 int child;
 while (k <= size/2)
 {	
	 child = 2*k;
	if ( (child < size) && (a[child].priority < a[child + 1].priority) )
		++child;
	if (new_el >= a[child].priority)
		break;
	else {
		a[k].priority = a[child].priority;
		k = child;
	}
 }
 a[k].priority = new_el;
}

bool enqueue(int value, int priority) {
    if (size + 1 == MAX_SIZE) {
        printf("Out of size");
		return 0;
	}
    a[size++] = element(value, priority);
    heapify_up(size + 1);
	return 1;
}

bool dequeue() {
    if (size == 0) {
        printf("Out of size");
		return 0;
	}
    swap(a[0], a[--size]);
    heapify_down (0);
    return 1;
}

int top() {
	return a[0].value;
}

int sizeofqueue() {
	return size;
}

int main() {
	return 0;
}