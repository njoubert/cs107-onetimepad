// one_time_pad.c
// Note: Buggy code!

#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <assert.h>

// bytearray struct necessary because input might not be
// ASCII text and NULLs allowed in input
struct bytearray {
    char *bytes;
    size_t length;
};

// cipher
// perform encryption/decryption on input using the key at the offset
void cipher(struct bytearray *input, const struct bytearray *keychars,size_t offset) {
    for (size_t i = 0; i < input->length; i++) {
        input->bytes[i] ^= keychars->bytes[i+offset];
    }
}

// read_bytes_from_file 
// reads in all bytes from key file
// caller responsible for freeing pointer in struct 
struct bytearray read_bytes_from_file(char *filename)
{
    FILE *fp = fopen(filename,"r");
    struct stat st;

    // get file information (for size)
    stat(filename, &st);

    struct bytearray keybytes;
    keybytes.length = st.st_size; 

    keybytes.bytes = malloc(keybytes.length);

    fread(keybytes.bytes, keybytes.length, 1, fp);
    fclose(fp);

    return keybytes;
}

// save_to_file
// saves a bytearray to a file
void save_to_file(struct bytearray output,char *filename)
{
    FILE *fp = fopen(filename,"w");

    fwrite(output.bytes, output.length, 1, fp);
    fclose(fp);
}

int main(int argc, char **argv)
{
    if (argc < 4) {
        printf("Usage:\n\t%s input_filename key_filename key_offset output_filename\n",argv[0]);
        return -1;
    }

    struct bytearray input = read_bytes_from_file(argv[1]);
    struct bytearray key = read_bytes_from_file(argv[2]);
    size_t offset = atoi(argv[3]);

    // encrypt or decrypt input
    cipher(&input,&key,offset);

    save_to_file(input,argv[4]);

    printf("Performed cipher on text. New offset:%lu\n",offset+input.length);
    return 0;
}
