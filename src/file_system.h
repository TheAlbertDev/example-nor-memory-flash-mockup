#ifndef FILE_SYSTEM_H__
#define FILE_SYSTEM_H__

#include <stddef.h>
#include <stdint.h>

typedef enum {
  FS_Status_Ok,
  FS_Status_Err,
  FS_Status_Folder_Already_Exists,
  FS_Status_Folder_Does_Not_Exist,
  FS_Status_File_Does_Not_Exist,
} FS_Status_t;

FS_Status_t FS_init(void);
FS_Status_t FS_deinit(void);
FS_Status_t FS_create_folder(const char *path);
FS_Status_t FS_save_to_file(const char *directory_path, const char *file_name,
                            const uint8_t *data, const size_t data_size);
FS_Status_t FS_get_file_size(const char *directory_path, const char *file_name,
                             size_t *output_size);
FS_Status_t FS_read_from_file(const char *directory_path, const char *file_name,
                              uint8_t *output_data);

#endif /* FILE_SYSTEM_H__ */