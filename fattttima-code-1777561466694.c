#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 10

typedef struct {
    char data[BUFFER_SIZE];
    int head;
    int tail;
    int count;
} CircularBuffer;

void initBuffer(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

bool isFull(CircularBuffer *cb) {
    return cb->count == BUFFER_SIZE;
}

bool isEmpty(CircularBuffer *cb) {
    return cb->count == 0;
}

bool writeBuffer(CircularBuffer *cb, char value) {
    if (isFull(cb)) {
        printf("[Overflow] المخزن ممتلئ! لا يمكن إضافة: %c\n", value);
        return false;
    }
    cb->data[cb->head] = value;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    cb->count++;
    return true;
}

bool readBuffer(CircularBuffer *cb, char *value) {
    if (isEmpty(cb)) {
        printf("[Underflow] المخزن فارغ!\n");
        return false;
    }
    *value = cb->data[cb->tail];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    cb->count--;
    return true;
}

int main() {
    // تعريف المتغيرات خارج الحلقة
    CircularBuffer myBuffer;
    char name[50];
    char fullName[100];
    const char *suffix = "CE-ESY";

    // حلقة تكرار لا نهائية لجعل البرنامج يطلب الاسم باستمرار
    while (1) {
        initBuffer(&myBuffer); // إعادة تهيئة المخزن في كل مرة

        printf("\nالرجاء إدخال اسمك (أو اضغط Ctrl+C للخروج): ");
        scanf("%s", name);

        strcpy(fullName, name);
        strcat(fullName, " ");
        strcat(fullName, suffix);

        printf("\nالنص الكامل المراد تخزينه: %s\n", fullName);
        printf("--- بدء عملية التخزين ---\n");

        for (int i = 0; i < strlen(fullName); i++) {
            writeBuffer(&myBuffer, fullName[i]);
        }

        printf("\n--- بدء عملية القراءة ---\n");
        printf("النتيجة المستخرجة من المخزن: ");

        char tempChar;
        while (!isEmpty(&myBuffer)) {
            if (readBuffer(&myBuffer, &tempChar)) {
                printf("%c", tempChar);
            }
        }
        
        printf("\n-------------------------------------------");
        printf("\nتمت الدورة بنجاح. سأطلب منك اسماً جديداً الآن...\n");
    }

    return 0;
}
