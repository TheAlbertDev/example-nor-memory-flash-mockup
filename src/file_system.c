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

#include "file_system.h"
#include "lfs/lfs.h"
#include "memory_io.h"
#include <string.h>

static lfs_t lfs;
lfs_file_t file;

static int read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
                void *buffer, lfs_size_t size);
static int prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
                const void *buffer, lfs_size_t size);
static int erase(const struct lfs_config *c, lfs_block_t block);
static int sync(const struct lfs_config *c);

static const struct lfs_config cfg = {
    .read = read,
    .prog = prog,
    .erase = erase,
    .sync = sync,
    .read_size = 256,
    .prog_size = 256,
    .block_size = 4096,
    .block_count = 512,
    .cache_size = 256,
    .lookahead_size = 16,
    .block_cycles = 100000,
};

FS_Status_t FS_init(void) {
  int err = lfs_mount(&lfs, &cfg);
  if (err != LFS_ERR_OK) {
    err = lfs_format(&lfs, &cfg);
    if (err != LFS_ERR_OK) {
      return FS_Status_Err;
    }
    err = lfs_mount(&lfs, &cfg);
    if (err != LFS_ERR_OK) {
      return FS_Status_Err;
    }
  }
  return FS_Status_Ok;
}

FS_Status_t FS_deinit(void) {
  int err = lfs_unmount(&lfs);
  if (err != LFS_ERR_OK) {
    return FS_Status_Err;
  }
  return FS_Status_Ok;
}

FS_Status_t FS_create_folder(const char *path) {
  if (path == NULL || path[0] == '\0') {
    return FS_Status_Err;
  }

  if (path[0] == '/' && path[1] == '\0') {
    return FS_Status_Folder_Already_Exists;
  }

  struct lfs_info file_info;
  int ret = lfs_stat(&lfs, path, &file_info);
  if (ret == LFS_ERR_OK) {
    if (file_info.type == LFS_TYPE_DIR) {
      return FS_Status_Folder_Already_Exists;
    } else {
      return FS_Status_Err;
    }
  } else if (ret != LFS_ERR_NOENT) {
    return FS_Status_Err;
  }

  if (strlen(path) > LFS_NAME_MAX) {
    return FS_Status_Err;
  }

  char current_path[LFS_NAME_MAX + 1];
  char segment_buffer[LFS_NAME_MAX + 1];

  current_path[0] = '\0';

  size_t current_path_len = 0;
  size_t start = 0;

  if (path[0] == '/') {
    current_path[0] = '/';
    current_path[1] = '\0';
    current_path_len = 1;
    start = 1;
  }

  size_t pos = start;

  while (path[pos] != '\0') {
    size_t segment_start = pos;
    size_t segment_len = 0;

    while (path[pos] != '/' && path[pos] != '\0') {
      pos++;
      segment_len++;
    }

    if (segment_len > 0) {
      size_t j;
      for (j = 0; j < segment_len && j < LFS_NAME_MAX; j++) {
        segment_buffer[j] = path[segment_start + j];
      }
      segment_buffer[j] = '\0';

      if (current_path_len > 0 && current_path[current_path_len - 1] != '/') {
        if (current_path_len + 1 < LFS_NAME_MAX) {
          current_path[current_path_len] = '/';
          current_path[current_path_len + 1] = '\0';
          current_path_len++;
        } else {
          return FS_Status_Err;
        }
      }

      if (current_path_len + segment_len < LFS_NAME_MAX) {
        for (j = 0; j < segment_len; j++) {
          current_path[current_path_len + j] = segment_buffer[j];
        }
        current_path[current_path_len + segment_len] = '\0';
        current_path_len += segment_len;
      } else {
        return FS_Status_Err;
      }

      struct lfs_info info;
      ret = lfs_stat(&lfs, current_path, &info);

      if (ret == LFS_ERR_NOENT) {
        ret = lfs_mkdir(&lfs, current_path);
        if (ret != LFS_ERR_OK) {
          return FS_Status_Err;
        }
      } else if (ret == LFS_ERR_OK) {
        if (info.type != LFS_TYPE_DIR) {
          return FS_Status_Err;
        }
      } else {
        return FS_Status_Err;
      }
    }

    if (path[pos] == '/') {
      pos++;
    }
  }

  return FS_Status_Ok;
}

FS_Status_t FS_save_to_file(const char *directory_path, const char *file_name,
                            const uint8_t *data, const size_t data_size) {
  if (directory_path == NULL || file_name == NULL || data == NULL) {
    return FS_Status_Err;
  }

  struct lfs_info dir_info;
  int ret = lfs_stat(&lfs, directory_path, &dir_info);
  if (ret != LFS_ERR_OK || dir_info.type != LFS_TYPE_DIR) {
    return FS_Status_Folder_Does_Not_Exist;
  }

  char full_path[LFS_NAME_MAX + 1];
  size_t dir_len = strlen(directory_path);
  size_t file_len = strlen(file_name);
  size_t i, j;

  if (dir_len + file_len + 2 > LFS_NAME_MAX) {
    return FS_Status_Err;
  }

  for (i = 0; i < dir_len && i < LFS_NAME_MAX; i++) {
    full_path[i] = directory_path[i];
  }

  if (dir_len > 0 && directory_path[dir_len - 1] != '/') {
    full_path[i++] = '/';
  }

  for (j = 0; j < file_len && i + j < LFS_NAME_MAX; j++) {
    full_path[i + j] = file_name[j];
  }

  full_path[i + j] = '\0';

  int flags = LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC;

  ret = lfs_file_open(&lfs, &file, full_path, flags);
  if (ret != LFS_ERR_OK) {
    return FS_Status_Err;
  }

  lfs_ssize_t bytes_written = lfs_file_write(&lfs, &file, data, data_size);

  int close_ret = lfs_file_close(&lfs, &file);

  if (bytes_written != (lfs_ssize_t)data_size || close_ret != LFS_ERR_OK) {
    return FS_Status_Err;
  }

  return FS_Status_Ok;
}

FS_Status_t FS_get_file_size(const char *directory_path, const char *file_name,
                             size_t *output_size) {
  if (directory_path == NULL || file_name == NULL || output_size == NULL) {
    return FS_Status_Err;
  }

  struct lfs_info dir_info;
  int ret = lfs_stat(&lfs, directory_path, &dir_info);
  if (ret != LFS_ERR_OK || dir_info.type != LFS_TYPE_DIR) {
    return FS_Status_Folder_Does_Not_Exist;
  }

  char full_path[LFS_NAME_MAX + 1];
  size_t dir_len = strlen(directory_path);
  size_t file_len = strlen(file_name);
  size_t i, j;

  if (dir_len + file_len + 2 > LFS_NAME_MAX) {
    return FS_Status_Err;
  }

  for (i = 0; i < dir_len && i < LFS_NAME_MAX; i++) {
    full_path[i] = directory_path[i];
  }

  if (dir_len > 0 && directory_path[dir_len - 1] != '/') {
    full_path[i++] = '/';
  }

  for (j = 0; j < file_len && i + j < LFS_NAME_MAX; j++) {
    full_path[i + j] = file_name[j];
  }

  full_path[i + j] = '\0';

  struct lfs_info file_info;
  ret = lfs_stat(&lfs, full_path, &file_info);
  if (ret != LFS_ERR_OK) {
    return FS_Status_File_Does_Not_Exist;
  }

  if (file_info.type != LFS_TYPE_REG) {
    return FS_Status_Err;
  }

  *output_size = file_info.size;

  return FS_Status_Ok;
}

FS_Status_t FS_read_from_file(const char *directory_path, const char *file_name,
                              uint8_t *output_data) {
  if (directory_path == NULL || file_name == NULL || output_data == NULL) {
    return FS_Status_Err;
  }

  struct lfs_info dir_info;
  int ret = lfs_stat(&lfs, directory_path, &dir_info);
  if (ret != LFS_ERR_OK || dir_info.type != LFS_TYPE_DIR) {
    return FS_Status_Folder_Does_Not_Exist;
  }

  char full_path[LFS_NAME_MAX + 1];
  size_t dir_len = strlen(directory_path);
  size_t file_len = strlen(file_name);
  size_t i, j;

  if (dir_len + file_len + 2 > LFS_NAME_MAX) {
    return FS_Status_Err;
  }

  for (i = 0; i < dir_len && i < LFS_NAME_MAX; i++) {
    full_path[i] = directory_path[i];
  }

  if (dir_len > 0 && directory_path[dir_len - 1] != '/') {
    full_path[i++] = '/';
  }

  for (j = 0; j < file_len && i + j < LFS_NAME_MAX; j++) {
    full_path[i + j] = file_name[j];
  }

  full_path[i + j] = '\0';

  /* Check if file exists and get its size */
  struct lfs_info file_info;
  ret = lfs_stat(&lfs, full_path, &file_info);
  if (ret != LFS_ERR_OK) {
    return FS_Status_File_Does_Not_Exist;
  }

  if (file_info.type != LFS_TYPE_REG) {
    return FS_Status_Err;
  }

  /* Open the file for reading */
  ret = lfs_file_open(&lfs, &file, full_path, LFS_O_RDONLY);
  if (ret != LFS_ERR_OK) {
    return FS_Status_Err;
  }

  /* Read the file content */
  lfs_ssize_t bytes_read =
      lfs_file_read(&lfs, &file, output_data, file_info.size);

  /* Close the file */
  int close_ret = lfs_file_close(&lfs, &file);

  if (bytes_read != (lfs_ssize_t)file_info.size || close_ret != LFS_ERR_OK) {
    return FS_Status_Err;
  }

  return FS_Status_Ok;
}

static int read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
                void *buffer, lfs_size_t size) {
  MEMIO_Status_t status = MEMIO_read(block * c->block_size + off, buffer, size);
  if (status != MEMIO_Status_Ok) {
    return LFS_ERR_IO;
  }
  return LFS_ERR_OK;
}

static int prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
                const void *buffer, lfs_size_t size) {
  MEMIO_Status_t status = MEMIO_prog(block * c->block_size + off, buffer, size);
  if (status != MEMIO_Status_Ok) {
    return LFS_ERR_IO;
  }
  return LFS_ERR_OK;
}

static int erase(const struct lfs_config *c, lfs_block_t block) {
  MEMIO_Status_t status = MEMIO_erase(block * c->block_size);
  if (status != MEMIO_Status_Ok) {
    return LFS_ERR_IO;
  }
  return LFS_ERR_OK;
}

static int sync(const struct lfs_config *c) { return LFS_ERR_OK; }