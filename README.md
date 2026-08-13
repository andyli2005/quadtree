# PPM Quadtree Compression and Steganography

A C program for loading grayscale PPM images, compressing them with a quadtree, and
hiding text or smaller images inside their pixel data.

The quadtree recursively divides an image into smaller regions until each region falls
below a configurable root mean square error (RMSE). Each leaf stores one average
intensity, allowing the tree to recreate an approximate version of the original image.

## Features

- Loads plain-text `P3` PPM images into a dynamically allocated pixel array
- Builds a quadtree using average intensity and an adjustable RMSE threshold
- Handles odd image dimensions and one-pixel-wide or one-pixel-tall regions
- Saves and reloads quadtrees through a preorder text representation
- Reconstructs a grayscale PPM image from quadtree leaf nodes
- Hides and reveals null-terminated text through least-significant-bit encoding
- Hides and reveals a smaller grayscale image inside another image
- Recursively frees every node in a dynamically allocated quadtree

## How the Quadtree Works

Each `QTNode` stores its image region, average grayscale intensity, node type, and up to
four children. The program calculates the RMSE between the region's pixels and its
average intensity:

- If the RMSE is at or below the selected threshold, the region becomes a leaf.
- If the RMSE is above the threshold, the region is divided and processed recursively.

A lower threshold preserves more image detail by creating more nodes. A higher threshold
creates fewer, larger regions and produces a more compressed approximation.

## Steganography

The message and image hiding functions modify only the least significant bit of each
carrier-image pixel:

- `hide_message` stores each character as eight bits and appends a null terminator.
- `reveal_message` reads groups of eight bits until it reconstructs that terminator.
- `hide_image` stores the secret image's width and height followed by its pixel values.
- `reveal_image` reads that metadata and recreates the hidden image as a PPM file.

The carrier image must have enough pixels for the encoded data. The implementation reads
the blue channel of each input pixel and writes grayscale output with equal red, green,
and blue values.

## Repository Structure

```text
include/
  image.h          PPM and steganography interfaces
  qtree.h          Quadtree structure and operations
src/
  image.c          PPM loading, cleanup, and least-significant-bit encoding
  qtree.c          Quadtree creation, serialization, reconstruction, and cleanup
  hw3_main.c       Demonstration driver for each operation
images/originals/  Provided PPM input images
tests/input/       Provided serialized quadtree input
tests/expected/    Provided expected PPM output
tests/src/         Local test and utility programs
```

## Requirements

- A POSIX-compatible environment such as Linux, macOS, or WSL
- A C11 compiler such as GCC or Clang
- CMake 3.10 or newer

## Building and Running

The main demonstration program can be built with CMake from the repository root:

```bash
cmake -S . -B build -DBUILD_CODEGRADE_TESTS=ON
cmake --build build --target hw3_main
./build/hw3_main
```

The `BUILD_CODEGRADE_TESTS=ON` setting skips the optional local test targets, which link
against GoogleTest libraries that are not bundled with the repository. It does not skip
the `hw3_main` demonstration program.

The program creates `tests/output/` when needed and demonstrates:

- Creating and deleting a quadtree
- Loading and saving preorder quadtree data
- Reconstructing a compressed PPM image
- Hiding and revealing a text message
- Hiding and revealing a smaller image

Generated files are written under `tests/output/`. Run the executable from the repository
root so its relative image and test paths resolve correctly.

## Using the Functions

The public interfaces are declared in `include/image.h` and `include/qtree.h`. The main
operations are:

| Function | Purpose |
|---|---|
| `load_image` | Loads a `P3` PPM file into memory |
| `create_quadtree` | Compresses an image using an RMSE threshold |
| `save_preorder_qt` / `load_preorder_qt` | Serializes or restores a quadtree |
| `save_qtree_as_ppm` | Reconstructs a PPM image from a quadtree |
| `hide_message` / `reveal_message` | Stores or retrieves text in pixel bits |
| `hide_image` / `reveal_image` | Stores or retrieves a smaller grayscale image |

## Testing

`src/hw3_main.c`, `tests/src/write_unit_test.c`, and `tests/src/part2_tests.c` are manual
harnesses rather than a self-contained automated test suite. The repository includes a
provided preorder quadtree fixture and expected reconstructed PPM file for comparison.
Build and runtime verification have not been rerun in the current Windows environment
because a compatible POSIX C compiler is unavailable.

## Context

I completed this project individually for Stony Brook University's CSE 220 course. The
initial commit contains the starter headers and source files, demonstration driver, image
fixtures, expected output, and development configuration provided for the assignment.

All later implementation commits are my individual work, including PPM loading,
least-significant-bit message and image hiding, RMSE-based quadtree construction,
preorder serialization, image reconstruction, memory cleanup, and the local test
harnesses.
