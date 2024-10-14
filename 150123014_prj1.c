/* ALi Doðan Pekdaþ 150123014 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct node {
    int digit;
    struct node *ptr;
};

struct node *newPtr(int digit) {
    struct node *head = (struct node *)malloc(sizeof(struct node));
    head->digit = digit;
    head->ptr = NULL;
    return head;
}

void append(struct node **head, int digit) {
    struct node *newNode = newPtr(digit);
    if (*head == NULL) {
        *head = newNode;
    } else {
        struct node *current = *head;
        while (current->ptr != NULL) {
            current = current->ptr;
        }
        current->ptr = newNode;
    }
}

struct node *reverse(struct node *head) {
    struct node *prev = NULL;
    struct node *current = head;
    struct node *next = NULL;
    while (current != NULL) {
        next = current->ptr;
        current->ptr = prev;
        prev = current;
        current = next;
    }
    return prev;
}

struct node *addLists(struct node *multiplicand, struct node *multiplier) {
    struct node *result = NULL;
    struct node **lastPtr = &result;
    int carry = 0;

    while (multiplicand != NULL || multiplier != NULL || carry) {
        int sum = carry;

        if (multiplicand != NULL) {
            sum += multiplicand->digit;
            multiplicand = multiplicand->ptr;
        }

        if (multiplier != NULL) {
            sum += multiplier->digit;
            multiplier = multiplier->ptr;
        }

        carry = sum / 10;
        append(lastPtr, sum % 10);
        lastPtr = &((*lastPtr)->ptr);
    }
    return result;
}

struct node *multiplyLists(struct node *multiplicand, struct node *multiplier) {
    struct node *result = NULL;
    struct node *ptr1 = multiplicand;
    int shift = 0;

    while (ptr1 != NULL) {
        struct node *currentResult = NULL;
        struct node *ptr2 = multiplier;
        int carry = 0;

        while (ptr2 != NULL) {
            int product = ((ptr1->digit) * (ptr2->digit) + carry);
            append(&currentResult, product % 10);
            carry = product / 10;
            ptr2 = ptr2->ptr;
        }

        if (carry > 0) {
            append(&currentResult, carry);
        }

		int i;
        for (i = 0; i < shift; i++) {
            struct node *zeroNode = newPtr(0);
            zeroNode->ptr = currentResult;
            currentResult = zeroNode;
        }

        result = addLists(result, currentResult);
        ptr1 = ptr1->ptr;
        shift++;
    }

    return result;
}

void readingFile(const char *filename, struct node **multiplicand, struct node **multiplier, int *decimal_places_1, int *decimal_places_2) {
    FILE *fptr = fopen(filename, "r");
    char ch;
    int decimal_seen = 0;

    while ((ch = fgetc(fptr)) != EOF && ch != '\n') {
        if (ch == '.') {
            decimal_seen = 1;
        } 
		else if (ch >= '0' && ch <= '9') {
            append(multiplicand, ch - '0');
            if (decimal_seen) (*decimal_places_1)++;
        }
    }

    decimal_seen = 0;
    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == '.') {
            decimal_seen = 1;
        } 
		else if (ch >= '0' && ch <= '9') {
            append(multiplier, ch - '0');
            if (decimal_seen) (*decimal_places_2)++;
        }
    }
    fclose(fptr);
}

void printList(FILE *fptr, struct node *head, int decimal_places) {
    if (head == NULL) {
        fprintf(fptr, "0");
        printf("0");
        return;
    }
    
    struct node *current = head;
    int length = 0;
    while (current != NULL) {
        length++;
        current = current->ptr;
    }

    current = head;
    int position = length - decimal_places;
    int i;
    for (i = 0; i < length; i++) {
        if (i == position && decimal_places > 0) {
            fprintf(fptr, ".");
            printf(".");
        }
        fprintf(fptr, "%d", current->digit);
        printf("%d", current->digit);
        current = current->ptr;
    }
    fprintf(fptr, "\n");
    printf("\n");
}

int main() {
    clock_t start = clock();

    struct node *multiplicand = NULL;
    struct node *multiplier = NULL;
    int decimal_places_1 = 0, decimal_places_2 = 0;

    readingFile("150123014_prj1_input.txt", &multiplicand, &multiplier, &decimal_places_1, &decimal_places_2);

    multiplicand = reverse(multiplicand);
    multiplier = reverse(multiplier);
    
    struct node *result = multiplyLists(multiplicand, multiplier);

    int total_decimal_places = decimal_places_1 + decimal_places_2;
    result = reverse(result);

    clock_t finish = clock();
    double ex_time = ((double)(finish - start)) / CLOCKS_PER_SEC;

    FILE *fptr;
	fptr = fopen("150123014_prj1_output.txt", "w");

    fprintf(fptr, "The multiplicand: ");
    printf("The multiplicand: ");
    printList(fptr, reverse(multiplicand), decimal_places_1);

    fprintf(fptr, "The multiplier: ");
    printf("The multiplier: ");
    printList(fptr, reverse(multiplier), decimal_places_2);

    fprintf(fptr, "The result: ");
    printf("The result: ");
    printList(fptr, result, total_decimal_places);

    fprintf(fptr, "Execution time of the program: %.10f seconds\n", ex_time);
    printf("Execution time of the program: %.10f seconds\n", ex_time);

    fclose(fptr);
    return 0;
}

