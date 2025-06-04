/**
 * MIT License
 *
 * Copyright (c) 2025 Albert Álvarez Carulla (TheAlbertDev)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
    fake_buffer[address + i] &= ((const uint8_t *)buffer)[i];
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