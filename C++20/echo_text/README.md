# Echo Text

Echo Text is a C++ application designed to read text from a file and output it to the console with a specified delay between each character. This is particularly useful for demonstrations or educational purposes where a typing effect is desired.

## Features

- Read text from a file.
- Output text to the console with a controlled time delay between characters.
- Command-line arguments to specify the filename and delay time.

## System Requirements

- Compiler supporting C++20 
- Tested on UNIX-based systems and Windows


## Building

To compile the Echo Text application, you can use the following g++ command:

```bash
make
sudo make install
```

## Usage

To run the application, you need to provide command-line arguments for the input file and the time delay in milliseconds. Here is how you can run the application:

```bash
./echo-text -i filename.txt -t 100
```

### Arguments

- `-t` or `--time delay` - Sets the delay time between each character output to the console.
- `-i` or `--input filename` - Specifies the file from which to read the text.
- `-h` or `--help` - Displays help information.

## Error Handling

The application includes error handling for file access issues and argument parsing errors.
