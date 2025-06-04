# NOR Memory Flash Mockup Example

This repository demonstrates how to create test fixtures for embedded systems, specifically for interacting with a GD25Q16C NOR flash memory chip. It provides a comprehensive example of implementing and testing a file system interface using the LittleFS filesystem on NOR flash memory.

The project implements a simple file system interface that operates on top of NOR flash memory. For testing purposes, it includes a fake implementation that simulates the behavior of the physical memory, allowing for thorough testing without actual hardware.

## Getting Started

### Running Tests

To verify the implementation, run the included tests:

```shell
cd test
make
```


## Project Structure

The project is organized as follows:

- **`src/`**: Contains the production code
  - `file_system.c/h`: File system interface implementation
  - `memory_io.h`: Hardware abstraction layer for memory operations
  - `lfs/`: LittleFS library integration (v2.11.0)

- **`test/`**: Contains test code and fixtures
  - `fake_memory_io.c/h`: Fake implementation of the memory I/O interface
  - `file_system.test.cpp`: CppUTest test cases for the file system
  - `makefile`: Build instructions for the test suite

## Devcontainer

This project includes a Visual Studio Code Devcontainer configuration that provides a ready-to-use development environment with all necessary tools preinstalled:

- Ubuntu 24.04 LTS base image
- GCC, G++, Make, and CMake
- CppUTest framework preinstalled at `/opt/cpputest`
- Code formatting and static analysis tools (clang-format)

To use it:
1. Open the project in Visual Studio Code
2. Install the "Remote - Containers" extension
3. Use the command palette (F1) and select "Remote-Containers: Reopen in Container"

## Software Bill of Materials (SBOM)

| Library   | Tag/Version           | Remote Repository                                                                 |
|-----------|-----------------------|----------------------------------------------------------------------------------|
| LittleFS  | v2.11.0               | [LittleFS v2.11.0](https://github.com/littlefs-project/littlefs/tree/v2.11.0)    |
| CppUTest  | latest-passing-build  | [CppUTest latest-passing-build](https://github.com/cpputest/cpputest/tree/004af40710d24c5dd075b0b84c82598e6d4f852b) |

## Dummy image generation

The project uses the `littlefs-python` package to generate test images stored in the `test/generated_images` folder. Each binary image (`.bin` file) corresponds to a folder with the same name that contains the files and directories to be included in the image.

To create a new LittleFS image:

```shell
littlefs-python create img01 img01.bin --fs-size=2mb --block-size=4096
```

This command creates a 2MB image file `img01.bin` with 4KB blocks from the contents of the `img01` directory.

The project already includes sample images in `test/generated_images/` that are used by the test fixtures. The folder structure (e.g., `img01/tmp/test_folder/test_file.bin`) is mirrored in the corresponding binary image.

## License

This project is licensed under the MIT License - see the LICENSE file for details.