# mcp

Produce sparse files using mmap(2)

## Installation

### Prerequisites

- macOS 10.12 (Sierra) or later
- Xcode and the Command Line Tools installed

### From source

Clone the repository and navigate to the project directory:

```sh
git clone https://github.com/inopinatus/mcp.git
cd mcp
```

Compile and install:

```sh
make install
```

Now, you should be able to use `mcp` from anywhere in your terminal. By default, files will be installed under `/usr/local`. You can adjust locations using the variables in the Makefile, e.g. `make install PREFIX=/opt`.

## Usage

```
mcp src dst
```

The destination will be produced using mmap(2) and memcpy(3), skipping zero pages, using a page size equal to the blocksize of the destination filesystem.

## Exit Status

The mcp utility exits with 0 on success, or >0 if an error occurs.

## Contributing

Bug reports and pull requests are welcome on Github at https://github.com/inopinatus/mcp
