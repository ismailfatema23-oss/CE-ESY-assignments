#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// تحديد حجم المخزن (اخترنا حجماً صغيراً للتأكد من عمل الدائرة كما طلب الدكتور)
#define BUFFER_SIZE 10

// تعريف هيكل المخزن الدائري
typedef struct {
    char data[BUFFER_SIZE];
    int head; // مؤشر الكتابة
    int tail; // مؤشر القراءة
    int count; // عدد العناصر الحالية
} CircularBuffer;

// 1. وظيفة تهيئة المخزن
void initBuffer(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

// 2. وظيفة التحقق هل المخزن ممتلئ
bool isFull(CircularBuffer *cb) {
    return cb->count == BUFFER_SIZE;
}

// 3. وظيفة التحقق هل المخزن فارغ
bool isEmpty(CircularBuffer *cb) {
    return cb->count == 0;
}

// 4. وظيفة الكتابة (الإدخال) - التعامل مع Overflow
bool writeBuffer(CircularBuffer *cb, char value) {
    if (isFull(cb)) {
        printf("[Overflow] المخزن ممتلئ! لا يمكن إضافة: %c\n", value);
        return false;
    }
    cb->data[cb->head] = value;
    cb->head = (cb->head + 1) % BUFFER_SIZE; // الحركة بشكل دائري
    cb->count++;
    return true;
}

// 5. وظيفة القراءة (الإزالة) - التعامل مع Underflow
bool readBuffer(CircularBuffer *cb, char *value) {
    if (isEmpty(cb)) {
        printf("[Underflow] المخزن فارغ!\n");
        return false;
    }
    *value = cb->data[cb->tail];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE; // الحركة بشكل دائري
    cb->count--;
    return true;
}

// --- الدالة الرئيسية main ---
int main() {
    CircularBuffer myBuffer;
    initBuffer(&myBuffer);

    char name[50];
    char fullName[100];
    const char *suffix = "CE-ESY";

    // أ. إدخال الاسم من المستخدم
    printf("الرجاء إدخال اسمك: ");
    scanf("%s", name);

    // ب. إضافة السلسلة النصية "CE-ESY"
    strcpy(fullName, name);
    strcat(fullName, " ");
    strcat(fullName, suffix);

    printf("\nالنص الكامل المراد تخزينه: %s\n", fullName);
    printf("--- بدء عملية التخزين ---\n");

    // ج. تخزين الناتج داخل المخزن الدائري
    for (int i = 0; i < strlen(fullName); i++) {
        writeBuffer(&myBuffer, fullName[i]);
    }

    printf("\n--- بدء عملية القراءة ---\n");
    printf("النتيجة المستخرجة من المخزن: ");

    // د. قراءة البيانات وعرضها والتأكد من الفراغ بعدها
    char tempChar;
    while (!isEmpty(&myBuffer)) {
        if (readBuffer(&myBuffer, &tempChar)) {
            printf("%c", tempChar);
        }
    }

    // هـ. التحقق من أن المخزن أصبح فارغاً
    if (isEmpty(&myBuffer)) {
        printf("\n\nتمت القراءة بنجاح، والمخزن الآن فارغ تماماً.\n");
    }

    return 0;
}