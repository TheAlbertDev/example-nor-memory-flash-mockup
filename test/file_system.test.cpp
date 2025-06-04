#include "CppUTest/TestHarness.h"
#include <stdexcept>
#include <stdio.h>

extern "C" {
#include "fake_memory_io.h"
#include "file_system.h"
}

static uint8_t memory_buffer[4096 * 512] = {0};

static void load_binary_image(const char *filepath);

// clang-format off
TEST_GROUP(File__system__initialization)
{
    void setup() {
        memset(memory_buffer, 0xFF, sizeof(memory_buffer));
        FAKE_MEMORY_IO_set_buffer(memory_buffer);
    }
};
// clang-format on

TEST(File__system__initialization, Initialize__non__existing__file__system) {
  FS_Status_t status;

  status = FS_init();
  CHECK_EQUAL(FS_Status_Ok, status);

  status = FS_deinit();
  CHECK_EQUAL(FS_Status_Ok, status);
}

TEST(File__system__initialization,
     Initialize__already__existing__file__system) {
  FS_Status_t status;
  const char *directory_path = "/tmp/test_folder";
  const char *file_name = "test_file.bin";
  size_t file_size;

  load_binary_image("./generated_images/img01.bin");

  status = FS_init();
  CHECK_EQUAL(FS_Status_Ok, status);

  status = FS_get_file_size(directory_path, file_name, &file_size);
  CHECK_EQUAL(FS_Status_Ok, status);

  uint8_t read_buffer[file_size];
  memset(read_buffer, 0, sizeof(read_buffer));

  status = FS_read_from_file(directory_path, file_name, read_buffer);
  CHECK_EQUAL(FS_Status_Ok, status);

  STRCMP_EQUAL("Hello, World!", (const char *)read_buffer);

  status = FS_deinit();
  CHECK_EQUAL(FS_Status_Ok, status);
}

TEST(File__system__initialization,
     Initialize__already__existing__corrupted__file__system) {
  FS_Status_t status;
  const char *directory_path = "/tmp/test_folder";
  const char *file_name = "test_file.bin";
  size_t file_size;

  load_binary_image("./generated_images/img01.bin");

  memory_buffer[0] = 0xAA; // Corrupt the first byte

  status = FS_init();
  CHECK_EQUAL(FS_Status_Ok, status);

  status = FS_get_file_size(directory_path, file_name, &file_size);
  CHECK_EQUAL(FS_Status_Folder_Does_Not_Exist, status);

  status = FS_deinit();
  CHECK_EQUAL(FS_Status_Ok, status);
}

// clang-format off
TEST_GROUP(File__system__management)
{
    void setup() {
        memset(memory_buffer, 0xFF, sizeof(memory_buffer));
        FAKE_MEMORY_IO_set_buffer(memory_buffer);
        FS_init();
    }

    void teardown() {
        FS_deinit();
    }
};
// clang-format on

TEST(File__system__management, Folder__creation__with__recursion) {
  FS_Status_t status;

  status = FS_create_folder("/tmp/test_folder");
  CHECK_EQUAL(FS_Status_Ok, status);

  status = FS_create_folder("/tmp/test_folder");
  CHECK_EQUAL(FS_Status_Folder_Already_Exists, status);

  status = FS_create_folder("/tmp/test_folder/");
  CHECK_EQUAL(FS_Status_Folder_Already_Exists, status);

  status = FS_create_folder("/tmp/test_folder/test/");
  CHECK_EQUAL(FS_Status_Ok, status);

  status = FS_create_folder("/tmp/test_folder/test/");
  CHECK_EQUAL(FS_Status_Folder_Already_Exists, status);

  status = FS_create_folder("tmp/test_folder/test/");
  CHECK_EQUAL(FS_Status_Folder_Already_Exists, status);
}

TEST(File__system__management,
     Folder__creation__returns__error__when__null__path) {
  FS_Status_t status;

  status = FS_create_folder(nullptr);
  CHECK_EQUAL(FS_Status_Err, status);
}

TEST(File__system__management,
     Folder__creation__returns__error__when__emtpy__path) {
  FS_Status_t status;

  status = FS_create_folder(nullptr);
  CHECK_EQUAL(FS_Status_Err, status);
}

TEST(File__system__management,
     Folder__creation__returns__already__exists_when__path__is__root) {
  FS_Status_t status;

  status = FS_create_folder("/");
  CHECK_EQUAL(FS_Status_Folder_Already_Exists, status);
}

TEST(File__system__management, Save__struct__into__non__existing__file) {
  FS_Status_t status;
  const char *directory_path = "/tmp/test_folder";
  const char *file_name = "test_file.bin";

  struct {
    int id;
    char name[20];
  } test_data = {42, "Test Data"};

  FS_create_folder(directory_path);
  status = FS_save_to_file(directory_path, file_name,
                           (const uint8_t *)&test_data, sizeof(test_data));
  CHECK_EQUAL(FS_Status_Ok, status);
}

TEST(File__system__management, Save__struct__into__existing__file) {
  FS_Status_t status;
  const char *directory_path = "/tmp/test_folder";
  const char *file_name = "test_file.bin";
  struct {
    int id;
    char name[20];
  } test_data = {42, "Test Data"};

  FS_create_folder(directory_path);
  FS_save_to_file(directory_path, file_name, (const uint8_t *)&test_data,
                  sizeof(test_data));

  test_data = {84, "Updated Data"};
  status = FS_save_to_file(directory_path, file_name,
                           (const uint8_t *)&test_data, sizeof(test_data));
  CHECK_EQUAL(FS_Status_Ok, status);
}

TEST(File__system__management,
     Save__to__file__returns__error__when__null__data) {
  FS_Status_t status;
  const char *directory_path = "/tmp/test_folder";
  const char *file_name = "test_file.bin";

  FS_create_folder(directory_path);
  status =
      FS_save_to_file(directory_path, file_name, (const uint8_t *)nullptr, 0U);
  CHECK_EQUAL(FS_Status_Err, status);
}

TEST(File__system__management,
     Save__to__file__returns__error__when__null__directory) {
  FS_Status_t status;
  const char *directory_path = "/tmp/test_folder";
  const char *file_name = "test_file.bin";
  struct {
    int id;
    char name[20];
  } test_data = {42, "Test Data"};

  FS_create_folder(directory_path);
  status = FS_save_to_file(nullptr, file_name, (const uint8_t *)&test_data,
                           sizeof(test_data));
  CHECK_EQUAL(FS_Status_Err, status);
}

TEST(File__system__management,
     Save__to__file__returns__error__when__null__file__name) {
  FS_Status_t status;
  const char *directory_path = "/tmp/test_folder";
  struct {
    int id;
    char name[20];
  } test_data = {42, "Test Data"};

  FS_create_folder(directory_path);
  status = FS_save_to_file(directory_path, nullptr, (const uint8_t *)&test_data,
                           sizeof(test_data));
  CHECK_EQUAL(FS_Status_Err, status);
}

TEST(File__system__management,
     Save__to__file__returns__error__when_directory__does__not__exist) {
  FS_Status_t status;
  const char *directory_path = "/tmp/test_folder";
  const char *file_name = "test_file.bin";
  struct {
    int id;
    char name[20];
  } test_data = {42, "Test Data"};

  status = FS_save_to_file(directory_path, file_name,
                           (const uint8_t *)&test_data, sizeof(test_data));
  CHECK_EQUAL(FS_Status_Folder_Does_Not_Exist, status);
}

TEST(File__system__management, Get__data__from__existing__file) {
  FS_Status_t status;
  const char *directory_path = "/tmp/test_folder";
  const char *file_name = "test_file.bin";

  struct test_struct {
    int id;
    char name[20];
  } test_data = {42, "Test Data"};

  FS_create_folder(directory_path);
  FS_save_to_file(directory_path, file_name, (const uint8_t *)&test_data,
                  sizeof(test_data));

  size_t file_size = 0;
  status = FS_get_file_size(directory_path, file_name, &file_size);
  CHECK_EQUAL(FS_Status_Ok, status);

  uint8_t read_buffer[file_size];
  memset(read_buffer, 0, sizeof(read_buffer));

  status = FS_read_from_file(directory_path, file_name, read_buffer);
  CHECK_EQUAL(FS_Status_Ok, status);

  CHECK_EQUAL(test_data.id, ((struct test_struct *)read_buffer)->id);
  for (size_t i = 0U; i < sizeof(test_data.name); i++) {
    CHECK_EQUAL(test_data.name[i],
                ((struct test_struct *)read_buffer)->name[i]);
  }
}

static void load_binary_image(const char *filepath) {
  FILE *file = fopen(filepath, "rb");
  if (file == nullptr) {
    throw std::runtime_error(std::string("Failed to open binary image file: ") +
                             filepath);
  }
  fread(memory_buffer, 1, sizeof(memory_buffer), file);
  fclose(file);
}