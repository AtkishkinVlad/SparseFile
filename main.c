#include<stdio.h>

#include<stdlib.h>

#include<sys/stat.h>

#include<fcntl.h>

#include<unistd.h>

#include<string.h>

#include<getopt.h>

void usage(int code) {
    printf("usage with optional parameter block_size:\n");
    printf("./sparse <output_filename> [ ( -b | --block ) <block_size> ]\n");
    printf("./sparse <input_filename> <output_filename> [ ( -b | --block ) <block_size> ]\n");
    printf("print help message:\n");
    printf("./sparse --help | - h\n");
    exit(code);
}

char * myStrDuplicate(char * s) {
    size_t len = strlen(s);
    char * result = (char * ) malloc(len + 1);

    if (result == NULL) {
        fprintf(stderr, "Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    strcpy(result, s);
    return result;
}

int main(int argc, char * argv[]) {
    const struct option optarg_options[] = {
        {
            "block",
            1,
            NULL,
            'b'
        },
        {
            "help",
            0,
            NULL,
            'h'
        },
        {
            NULL,
            0,
            NULL,
            0
        }
    };

    const char * const short_options = "-b:h";
    int next_option;
    int block_size = -1;
    char * input_filename = NULL;
    char * output_filename = NULL;

    do {
        next_option = getopt_long(argc, argv, short_options,
            optarg_options, NULL);

        switch (next_option) {
          case 'b':
              if (sscanf(optarg, "%d", &block_size) != 1) {
                  fprintf(stderr, "Bad block size\n");
                  exit(EXIT_FAILURE);
              }
              if (block_size <= 0) {
                  fprintf(stderr, "Block size must be natural\n");
                  exit(EXIT_FAILURE);
              }
              break;

          case 'h':
              usage(EXIT_SUCCESS);
              break;

          case 1:
              if (input_filename == NULL) {
                  input_filename = myStrDuplicate(optarg);
                  break;
              }
              if (output_filename == NULL) {
                  output_filename = myStrDuplicate(optarg);
                  break;
              }
              
              fprintf(stderr, "Bad command line\n");
              usage(EXIT_FAILURE);
              break;

          case '?':
              fprintf(stderr, "Bad command line\n");
              usage(EXIT_FAILURE);
              break;
        }

    } while (next_option != -1);

    if (block_size == -1) {
        block_size = 4096;
    }

    if (input_filename == NULL) {
        fprintf(stderr, "Bad command line\n");
        usage(EXIT_FAILURE);
    }

    if (output_filename == NULL) {
        output_filename = input_filename;
        input_filename = NULL;
    }

    if ((input_filename != NULL) && (output_filename != NULL) &&
        (strcmp(input_filename, output_filename) == 0)) {
        fprintf(stderr, "Input and output filenames cannot be equal\n");
        exit(EXIT_FAILURE);
    }

    int fd_in;
    
    if (input_filename != NULL) {
        fd_in = open(input_filename, O_RDONLY);
        if (fd_in == -1) {
            fprintf(stderr, "Cannot open '%s' for reading\n", input_filename);
            exit(EXIT_FAILURE);
        }
    } else {        // Если читаем из stdin
        fd_in = 0;
    }
    
    int fd_out = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    if (fd_out == -1) {
        fprintf(stderr, "Cannot open '%s' for writing\n", output_filename);
        exit(EXIT_FAILURE);
    }

    char * buffer = (char *)malloc(block_size);
    
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    // Счетчик числа прочитанных блоков
    int n = 0;
    // Число байт, прочтенное или записанное в операции чтения/записи
    ssize_t count;

    while (1) {
        count = read(fd_in, buffer, block_size);

        if (count == -1) {
            fprintf(stderr, "I/O error while reading input file\n");
            exit(EXIT_FAILURE);
        }
        
        if (count == 0) {
            break;
        }
        
        n++;
        int nonZeroCharacter = 0;
        char * p = buffer;

        for (int i = count; i > 0; i--) {
            if ( * (p++)) {
                nonZeroCharacter = 1;
                break;
            }
        }

        if (nonZeroCharacter) {
            // Передвигаем указатель выходного файла на начало блока
            if (lseek(fd_out, block_size * (n - 1), SEEK_SET) == -1) {
                fprintf(stderr, "Cannot lseek in output file\n");
                exit(EXIT_FAILURE);
            }
            // Записываем блок в выходной файл
            if (write(fd_out, buffer, count) != count) {
                fprintf(stderr, "I/O error while writing to output file\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (input_filename != NULL) {
        if (close(fd_in) == -1) {
            fprintf(stderr, "Cannot close input file\n");
            exit(EXIT_FAILURE);
        }
    }

    if (close(fd_out) == -1) {
        fprintf(stderr, "Cannot close output file\n");
        exit(EXIT_FAILURE);
    }

    if (input_filename != NULL) {
        free(input_filename);
    }

    if (output_filename != NULL) {
        free(output_filename);
    }

    return 0;
}