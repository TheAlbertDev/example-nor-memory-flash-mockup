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
 * @file file_system.h
 * @brief File system interface for NOR flash memory.
 *
 * This module provides a file system interface using the Littlefs (LFS) library
 * to manage files and directories on a NOR flash memory. It abstracts the
 * underlying memory operations through the memory_io layer.
 */

#ifndef FILE_SYSTEM_H__
#define FILE_SYSTEM_H__

#include <stddef.h>
#include <stdint.h>

/**
 * @brief File system status codes.
 *
 * Enumeration of status codes returned by file system operations.
 */
typedef enum {
  FS_Status_Ok,                    /**< Operation completed successfully */
  FS_Status_Err,                   /**< General error occurred */
  FS_Status_Folder_Already_Exists, /**< Attempted to create a folder that
                                      already exists */
  FS_Status_Folder_Does_Not_Exist, /**< Attempted to access a folder that
                                      doesn't exist */
  FS_Status_File_Does_Not_Exist,   /**< Attempted to access a file that doesn't
                                      exist */
} FS_Status_t;

/**
 * @brief Initialize the file system.
 *
 * Initializes the file system by mounting it if available, or by formatting
 * and then mounting it if it's not available.
 *
 * @return FS_Status_Ok if successful, FS_Status_Err otherwise.
 */
FS_Status_t FS_init(void);

/**
 * @brief Deinitialize the file system.
 *
 * Safely unmounts the file system to ensure all pending operations are
 * completed.
 *
 * @return FS_Status_Ok if successful, FS_Status_Err otherwise.
 */
FS_Status_t FS_deinit(void);

/**
 * @brief Create a new folder in the file system.
 *
 * Creates a folder at the specified path. If intermediate directories
 * in the path don't exist, they will be created automatically.
 *
 * @param path The path where the folder should be created.
 * @return FS_Status_Ok if successful,
 *         FS_Status_Folder_Already_Exists if folder already exists,
 *         FS_Status_Err otherwise.
 */
FS_Status_t FS_create_folder(const char *path);

/**
 * @brief Save data to a file.
 *
 * Writes the provided data to a file in the specified directory.
 * If the file already exists, it will be overwritten.
 *
 * @param directory_path Path to the directory where the file should be saved.
 * @param file_name Name of the file to create or overwrite.
 * @param data Pointer to the data to write.
 * @param data_size Size of the data in bytes.
 * @return FS_Status_Ok if successful,
 *         FS_Status_Folder_Does_Not_Exist if the directory doesn't exist,
 *         FS_Status_Err otherwise.
 */
FS_Status_t FS_save_to_file(const char *directory_path, const char *file_name,
                            const uint8_t *data, const size_t data_size);

/**
 * @brief Get the size of a file.
 *
 * Retrieves the size of a specified file and stores it in the output parameter.
 *
 * @param directory_path Path to the directory containing the file.
 * @param file_name Name of the file.
 * @param output_size Pointer where the file size will be stored.
 * @return FS_Status_Ok if successful,
 *         FS_Status_Folder_Does_Not_Exist if the directory doesn't exist,
 *         FS_Status_File_Does_Not_Exist if the file doesn't exist,
 *         FS_Status_Err otherwise.
 */
FS_Status_t FS_get_file_size(const char *directory_path, const char *file_name,
                             size_t *output_size);

/**
 * @brief Read data from a file.
 *
 * Reads the entire content of a file into the provided output buffer.
 * The caller must ensure that the buffer is large enough to hold the entire
 * file content. Use FS_get_file_size() first to determine the required buffer
 * size.
 *
 * @param directory_path Path to the directory containing the file.
 * @param file_name Name of the file to read.
 * @param output_data Pointer to the buffer where file data will be stored.
 * @return FS_Status_Ok if successful,
 *         FS_Status_Folder_Does_Not_Exist if the directory doesn't exist,
 *         FS_Status_File_Does_Not_Exist if the file doesn't exist,
 *         FS_Status_Err otherwise.
 */
FS_Status_t FS_read_from_file(const char *directory_path, const char *file_name,
                              uint8_t *output_data);

#endif /* FILE_SYSTEM_H__ */