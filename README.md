# SEC
SEC is a encryption cipher that uses XOR encryption und byte shifting, aswell as keyexpansion method to encrypt and decrypt data.
SEC stands for [S]ecure [E]ncryption [C]ipher

SEC isnt actually secure from a cryptographic perspective, using other ciphers like AES would be favorable, but SEC can still provide basic educational value to learners.

# Versions 
SEC is currently officially avaiable on **Linux** and **Windows**, to download sec, you can download:
- 'sec' Linux native executable
- 'sec.exe' Windows native executable

# How to use SEC?
To use SEC, you need to open your native CLI and move to the dir where sec is located, then you can use the following as a guide as for what to do with SEC:

## Usage:
  program [OPTION] [KEY] [INPUT_FILE] [OUTPUT_FILE]

## Options:
  --e              Encrypt the input file using the provided key.
  --d              Decrypt the input file using the provided key.

## Arguments:
  [KEY]            A string used as the encryption/decryption key. The length of the key will determine the strength of the encryption.
  [INPUT_FILE]     Path to the input file to be encrypted or decrypted.
  [OUTPUT_FILE]    Path where the processed file (encrypted or decrypted) will be saved.

## Description:
  This program performs encryption and decryption on files using a custom algorithm. It reads the input file in blocks, processes each block based on the specified option (`--e` for encryption or `--d` for decryption), and writes the output to a new file.

## Examples:
###  Encrypt a file:
    program --e mykey input.txt encrypted.txt

###  Decrypt a file:
    program --d mykey encrypted.txt decrypted.txt

## Notes:
  - The key must be a non-empty string.
  - Both the input and output file paths must be valid.
  - The program processes files in blocks of 512 bytes but supports files of any size.
  - If the file size is smaller than 512 bytes, the program dynamically adjusts the block size.

**You can also use --help argument**

# License

SEC uses the MIT License.
