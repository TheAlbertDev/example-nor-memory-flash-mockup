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

/**
 * @file memory_io.h
 * @brief NOR flash memory I/O interface.
 *
 * This module provides a hardware abstraction layer for NOR flash memory
 * operations. It offers basic functionality for reading, programming (writing),
 * and erasing NOR flash memory. These operations respect the specific
 * characteristics of NOR flash memory, such as the need to erase before writing
 * and page/block alignment.
 */

#ifndef MEMORY_IO_H__
#define MEMORY_IO_H__

#include <stdint.h>

/**
 * @brief Status codes for memory I/O operations.
 *
 * These codes represent the result of memory operations on NOR flash.
 */
typedef enum {
  MEMIO_Status_Ok,  /**< Operation completed successfully */
  MEMIO_Status_Err, /**< Operation failed */
} MEMIO_Status_t;

/**
 * @brief Read data from NOR flash memory.
 *
 * This function reads a specified number of bytes from the NOR flash memory
 * starting at the given address.
 *
 * @param address The starting memory address to read from (byte-aligned).
 * @param buffer Pointer to a buffer where the read data will be stored.
 * @param size Number of bytes to read.
 * @return MEMIO_Status_Ok if successful, MEMIO_Status_Err otherwise.
 */
MEMIO_Status_t MEMIO_read(uint32_t address, void *buffer, uint32_t size);

/**
 * @brief Program (write) data to NOR flash memory.
 *
 * This function writes data to the NOR flash memory. Note that NOR flash
 * requires the target memory to be in an erased state before programming.
 * The function does not automatically erase memory before writing.
 *
 * @param address The starting memory address to write to (must be properly
 * aligned).
 * @param buffer Pointer to the data to be written.
 * @param size Number of bytes to write.
 * @return MEMIO_Status_Ok if successful, MEMIO_Status_Err otherwise.
 */
MEMIO_Status_t MEMIO_prog(uint32_t address, const void *buffer, uint32_t size);

/**
 * @brief Erase a block of NOR flash memory.
 *
 * This function erases a block of memory at the specified address. NOR flash
 * memory must be erased before new data can be programmed. Erasing sets all
 * bits in the memory block to 1 (0xFF).
 *
 * @param address The starting address of the block to erase (must be
 * block-aligned).
 * @return MEMIO_Status_Ok if successful, MEMIO_Status_Err otherwise.
 */
MEMIO_Status_t MEMIO_erase(uint32_t address);

#endif /* MEMORY_IO_H__ */