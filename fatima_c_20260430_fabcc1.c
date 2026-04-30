#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Two different sizes for testing
#define BUFFER_SIZE_SMALL  5
#define BUFFER_SIZE_LARGE  15

// Circular buffer structure
typedef struct {
    unsigned char *buffer;  // array to store data
    int head;               // write index
    int tail;               // read index
    int size;               // buffer size
    int count;              // number of elements currently in buffer
} CircularBuffer;

// Initialize the buffer - allocate memory and set starting values
CircularBuffer* circular_buffer_init(int size) {
    CircularBuffer *cb = (CircularBuffer*)malloc(sizeof(CircularBuffer));
    if (cb == NULL) return NULL;
    
    cb->buffer = (unsigned char*)malloc(size * sizeof(unsigned char));
    if (cb->buffer == NULL) {
        free(cb);
        return NULL;
    }
    
    // start with everything empty
    cb->head = 0;
    cb->tail = 0;
    cb->size = size;
    cb->count = 0;
    
    return cb;
}

// Free the memory so we don't have memory leaks
void circular_buffer_free(CircularBuffer *cb) {
    if (cb != NULL) {
        if (cb->buffer != NULL) free(cb->buffer);
        free(cb);
    }
}

// Check if buffer is full
int is_full(CircularBuffer *cb) {
    return (cb->count == cb->size);
}

// Check if buffer is empty
int is_empty(CircularBuffer *cb) {
    return (cb->count == 0);
}

// Write data into buffer
// Returns 1 if success, 0 if overflow (buffer full)
int circular_buffer_put(CircularBuffer *cb, unsigned char data) {
    if (is_full(cb)) {
        return 0;   // no space left
    }
    
    cb->buffer[cb->head] = data;           // store the data
    cb->head = (cb->head + 1) % cb->size;  // move head circularly
    cb->count++;                            // increase count
    
    return 1;
}

// Read data from buffer
// Returns 1 if success, 0 if underflow (buffer empty)
int circular_buffer_get(CircularBuffer *cb, unsigned char *data) {
    if (is_empty(cb)) {
        return 0;   // nothing to read
    }
    
    *data = cb->buffer[cb->tail];          // get the data
    cb->tail = (cb->tail + 1) % cb->size;  // move tail circularly
    cb->count--;                            // decrease count
    
    return 1;
}

// Print buffer status so I can see what's happening
void print_buffer_status(CircularBuffer *cb) {
    printf("[Status] count=%d, head=%d, tail=%d, size=%d\n", 
           cb->count, cb->head, cb->tail, cb->size);
}

int main() {
    printf("===== Circular Buffer Test (CE-ESY) =====\n\n");
    
    // The string we need to store (required by assignment)
    char test_string[] = "CE-ESY";
    int string_length = strlen(test_string);  // should be 6 characters
    
    // ----------------------------------------------
    // TEST 1: Small buffer (size = 5) - smaller than our data
    // We expect overflow to happen
    // ----------------------------------------------
    printf("--- Test 1: Small buffer (size=5) ---\n");
    CircularBuffer *cb_small = circular_buffer_init(BUFFER_SIZE_SMALL);
    if (cb_small == NULL) {
        printf("Memory allocation failed!\n");
        return -1;
    }
    
    printf("Storing \"%s\" into buffer (size=%d):\n", test_string, BUFFER_SIZE_SMALL);
    for (int i = 0; i < string_length; i++) {
        int result = circular_buffer_put(cb_small, test_string[i]);
        if (result == 1) {
            printf("  Put '%c' [OK]\n", test_string[i]);
        } else {
            printf("  Put '%c' [OVERFLOW - buffer full!]\n", test_string[i]);
        }
        print_buffer_status(cb_small);
    }
    
    printf("\nReading from buffer:\n");
    unsigned char read_char;
    int read_count = 0;
    while (!is_empty(cb_small)) {
        int result = circular_buffer_get(cb_small, &read_char);
        if (result == 1) {
            printf("  Got '%c'\n", read_char);
            read_count++;
        }
    }
    printf("Total read: %d characters\n", read_count);
    
    // Try to read one more time to test underflow
    int underflow_check = circular_buffer_get(cb_small, &read_char);
    if (underflow_check == 0) {
        printf("[UNDERFLOW] Buffer empty, cannot read.\n");
    }
    
    print_buffer_status(cb_small);
    circular_buffer_free(cb_small);
    
    printf("\n");
    
    // ----------------------------------------------
    // TEST 2: Large buffer (size = 15) - larger than our data
    // Everything should work fine, no overflow
    // ----------------------------------------------
    printf("--- Test 2: Large buffer (size=15) ---\n");
    CircularBuffer *cb_large = circular_buffer_init(BUFFER_SIZE_LARGE);
    if (cb_large == NULL) {
        printf("Memory allocation failed!\n");
        return -1;
    }
    
    printf("Storing \"%s\" into buffer (size=%d):\n", test_string, BUFFER_SIZE_LARGE);
    for (int i = 0; i < string_length; i++) {
        int result = circular_buffer_put(cb_large, test_string[i]);
        if (result == 1) {
            printf("  Put '%c' [OK]\n", test_string[i]);
        } else {
            printf("  Put '%c' [OVERFLOW - should not happen here]\n", test_string[i]);
        }
        print_buffer_status(cb_large);
    }
    
    printf("\nReading from buffer:\n");
    while (!is_empty(cb_large)) {
        circular_buffer_get(cb_large, &read_char);
        printf("  Got '%c'\n", read_char);
    }
    print_buffer_status(cb_large);
    
    // Make sure buffer is empty after reading
    if (is_empty(cb_large)) {
        printf("\n[SUCCESS] Buffer is empty after reading all data.\n");
    }
    
    circular_buffer_free(cb_large);
    
    // ----------------------------------------------
    // Extra test: Check underflow on empty buffer
    // ----------------------------------------------
    printf("\n--- Extra underflow test ---\n");
    CircularBuffer *cb_empty = circular_buffer_init(3);
    unsigned char dummy;
    if (circular_buffer_get(cb_empty, &dummy) == 0) {
        printf("[UNDERFLOW] Correctly detected read from empty buffer.\n");
    }
    circular_buffer_free(cb_empty);
    
    printf("\n===== End of Circular Buffer Test (CE-ESY) =====\n");
    return 0;
}