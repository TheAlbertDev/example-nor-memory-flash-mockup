#ifndef MEMORY_IO_H__
#define MEMORY_IO_H__

#include <stdint.h>

typedef enum {
  MEMIO_Status_Ok,
  MEMIO_Status_Err,
} MEMIO_Status_t;

MEMIO_Status_t MEMIO_read(uint32_t address, void *buffer, uint32_t size);
MEMIO_Status_t MEMIO_prog(uint32_t address, const void *buffer, uint32_t size);
MEMIO_Status_t MEMIO_erase(uint32_t address);

#endif /* MEMORY_IO_H__ */