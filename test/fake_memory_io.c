#include "fake_memory_io.h"

static uint8_t *fake_buffer = NULL;
void FAKE_MEMORY_IO_set_buffer(uint8_t *buffer) { fake_buffer = buffer; }

MEMIO_Status_t MEMIO_read(uint32_t address, void *buffer, uint32_t size) {
  for (uint32_t i = 0; i < size; i++) {
    ((uint8_t *)buffer)[i] = fake_buffer[address + i];
  }
  return MEMIO_Status_Ok;
}

MEMIO_Status_t MEMIO_prog(uint32_t address, const void *buffer, uint32_t size) {
  for (uint32_t i = 0; i < size; i++) {
    fake_buffer[address + i] = ((const uint8_t *)buffer)[i];
  }
  return MEMIO_Status_Ok;
}
MEMIO_Status_t MEMIO_erase(uint32_t address) {
  address &= 0xFFFF000;
  for (uint32_t i = 0; i < 4096; i++) {
    fake_buffer[address + i] = 0xFF;
  }
  return MEMIO_Status_Ok;
}