#include <stdio.h>
#include <stdlib.h>

#define MAX_HEAP_SIZE 1024  
#define MIN_BLOCK_SIZE 8     

typedef struct Block {
    size_t size;
    int is_free;
    struct Block *next;
} Block;

Block *free_list = NULL;

size_t next_power_of_2(size_t size) {
    size_t power = MIN_BLOCK_SIZE;
    while (power < size) {
        power *= 2;
    }
    return power;
}

void init_heap() {
    free_list = (Block *)malloc(sizeof(Block));
    free_list->size = MAX_HEAP_SIZE;
    free_list->is_free = 1;
    free_list->next = NULL;
}

size_t total_free_space() {
    size_t total = 0;
    Block *current = free_list;
    while (current) {
        if (current->is_free) {
            total += current->size;
        }
        current = current->next;
    }
    return total;
}

void *allocate(size_t size) {
    size = next_power_of_2(size + sizeof(Block));  
    Block *current = free_list;

    while (current && (!current->is_free || current->size < size)) {
        current = current->next;
    }

    if (!current) {
        printf("Not enough memory available!\n");
        return NULL;
    }

    while (current->size > size) {
        size_t half_size = current->size / 2;
        Block *buddy = (Block *)((char *)current + half_size);
        buddy->size = half_size;
        buddy->is_free = 1;
        buddy->next = current->next;
        current->size = half_size;
        current->next = buddy;
    }

    current->is_free = 0;
    printf("Memory allocated at: %p (Size: %zu bytes)\n", (void *)(current + 1), current->size);
    printf("Total free space available: %zu bytes\n", total_free_space()); 
    return (void *)(current + 1);
}

void merge_buddies() {
    Block *current = free_list;
    while (current && current->next) {
        if (current->is_free && current->next->is_free && current->size == current->next->size) {
            current->size *= 2;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

void free_memory(void *ptr) {
    if (!ptr) {
        printf("Error: Invalid memory location!\n");
        return;
    }

    Block *block = (Block *)ptr - 1;
    block->is_free = 1;
    printf("Memory freed successfully at: %p\n", ptr);

    merge_buddies();
    printf("Total free space available: %zu bytes\n", total_free_space());  
}

void show_free_list() {
    printf("Free List:\n");
    Block *current = free_list;
    while (current) {
        if (current->is_free) {
            printf("Block at %p | Size: %zu bytes (Free)\n", (void *)current, current->size);
        }
        current = current->next;
    }
}

void show_allocated_list() {
    printf("Allocated List:\n");
    Block *current = free_list;
    while (current) {
        if (!current->is_free) {
            printf("Block at %p | Size: %zu bytes (Allocated)\n", (void *)current, current->size);
        }
        current = current->next;
    }
}

int main() {
    init_heap();
    int choice;
    void *ptrs[100] = {NULL}; 
    int index = 0;

    while (1) {
        printf("\nChoose an option:\n");
        printf("1. Allocate memory\n2. Free memory\n3. Show free list\n4. Show allocated list\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            size_t size;
            printf("Enter size to allocate: ");
            scanf("%zu", &size);
            ptrs[index] = allocate(size);
            if (ptrs[index]) index++;  
        }
        else if (choice == 2) {
            int free_index;
            printf("Enter index of memory to free: ");
            scanf("%d", &free_index);
            if (free_index < 0 || free_index >= index || !ptrs[free_index]) {
                printf("Error: Invalid memory index!\n");
            } else {
                free_memory(ptrs[free_index]);
                ptrs[free_index] = NULL;
            }
        }
        else if (choice == 3) {
            show_free_list();
        }
        else if (choice == 4) {
            show_allocated_list();
        }
        else if (choice == 5) {
            printf("Exiting program...\n");
            
            for (int i = 0; i < index; i++) {
                if (ptrs[i]) free_memory(ptrs[i]);
            }
            printf("Memory freed successfully!\n");
            free(free_list); 
            break;
        }
        else {
            printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
