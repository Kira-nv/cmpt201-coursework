#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void print_out(char *format, void *data, size_t data_size) {
  char buf[64];
  ssize_t len = snprintf(buf, 64, format, *(uint64_t *)data);
  if (len < 0) {
    perror("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

struct header {
  uint64_t size;
  struct header *next;
};

int main() {

  // Use sbrk to increase heap size by 256, sbrk returns (void*)-1 when failed
  // not NULL

  void *heap_start = sbrk(256);
  if (heap_start == (void *)-1) {
    perror("sbrk failed");
    return -1;
  }

  // Calculate the size of headersize and size of data after header

  uint64_t blocksize = 128;
  size_t headersize = sizeof(struct header);
  size_t datasize = blocksize - headersize;

  // Create pointers to headers at the beginning of each block
  // must set second heap_start pointer from void pointer to char pointer
  // to be able to do pointer arithmatics on it
  // (char* has a definite byte size of 1 whereas void* has no definite size)

  struct header *block1 = (struct header *)heap_start;
  struct header *block2 = (struct header *)(heap_start + blocksize);

  // Fill header values

  block1->size = datasize;
  block1->next = NULL;

  block2->size = datasize;
  block2->next = block1;

  // Fill data values

  char *block1_data = (char *)block1 + headersize;
  memset(block1_data, 0, datasize);

  char *block2_data = (char *)block2 + headersize;
  memset(block2_data, 1, datasize);

  // Print data

  print_out("first block:          %p\n", &block1, sizeof(&block1));
  print_out("second block:         %p\n", &block2, sizeof(&block2));

  print_out("first block size:     %lu\n", &(block1->size), sizeof(block1->size));
  print_out("first block next:     %p\n", &(block1->next), sizeof(block1->next));

  print_out("second block size:    %lu\n", &(block2->size), sizeof(block2->size));
  print_out("second block next:    %p\n", &(block2->next), sizeof(block2->next));

  for (size_t i = 0; i < datasize; i++) {
    uint64_t val = (uint64_t)block1_data[i];
    print_out("%lu\n", &val, sizeof(val));
  }

  for (size_t i = 0; i < datasize; i++) {
    uint64_t val = (uint64_t)block2_data[i];
    print_out("%lu\n", &val, sizeof(val));
  }

  return 0;
}
