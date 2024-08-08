#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>

struct arguments {
    char *input_file;
    char *output_file;
    int no_palette;
    char *var_name;
};

static char* basename(char const *path) {
    char *s = strrchr(path, '/');
    if(!s) {
        return strdup(path);
    } else {
        return strdup(s + 1);
    }
}

static char* tr(char orig, char rep, char *str) {
    if (!str) {
        return NULL;
    }

    char *ix = str;
    while((ix = strchr(ix, orig)) != NULL) {
        *ix++ = rep;
    }

    return str;
}

static size_t file_size(FILE *f) {
    struct stat fs_buf;

    int fd = fileno(f);
    if (fstat(fd, &fs_buf) != 0) {
        return -1;
    } else {
        return fs_buf.st_size;
    }
}

static char *slurp(FILE *pcx, size_t size) {
    // whatever, just go for it...
    char *buf = malloc(size);

    if (!buf) {
        return NULL;
    }

    if (fread(buf, 1, size, pcx) != size) {
        free(buf);
        return NULL;
    }

    return buf;
}

static bool dump_palette(char *buf, FILE *out) {
    char sp_buf[9];

    if (*buf++ != 0x0C) {
        printf("ERROR: Palette indicator not present\n");
        return false;
    } else {
        for (int i = 0; i < 256; i++) {
            if (i % 16 == 0) {
                fputs("\n    ", out);
            }

            uint16_t entry = 0;

            entry |= ((*buf++ & 0xF0) << 4);
            entry |= (*buf++ & 0xF0);
            entry |= ((*buf++ & 0xF0) >> 4);

            int sn = snprintf(sp_buf, 9, "0x%04x, ", entry);                        
            if (sn && sn < 9) {
                fputs(sp_buf, out);
            } else {
                printf("ERROR: snprintf failed\n");
                return false;
            }
        }

        return true;
    }
}

static size_t draw(char pix, FILE *out) {
    static bool high = false;
    static uint16_t save_pix = 0;
    static size_t count = 0;
    char sp_buf[9];

    if (high) {
        if (count++ % 16 == 0) {
            fputs("\n    ", out);
        }

        // output
        uint16_t entry = save_pix << 8 | pix;

        int sn = snprintf(sp_buf, 9, "0x%04x, ", entry);                        
        if (sn && sn < 9) {
            fputs(sp_buf, out);
        } else {
            printf("ERROR: snprintf failed (%d: %s)\n", sn, sp_buf);
            return -1;
        }

        high = false;
    } else {
        save_pix = pix;
        high = true;
    }

    return count;
}

static size_t dump_image(uint32_t buf_size, char *buf, FILE *out) {
    size_t last_count = 0;

    for (uint32_t i = 0; i < buf_size; i++) {
        uint8_t pix = buf[i];

        if ((pix & 0xc0) == 0xc0) {
            // Do a run
            uint8_t len = pix & 0x3f;
            pix = buf[++i];
            
            for (uint8_t j = 0; j < len; j++) {
                if ((last_count = draw(pix, out)) == -1) {
                    return -1;
                }
            }
        } else {
            // Single pixel
            if ((last_count = draw(pix, out)) == -1) {
                return -1;
            }
        }
    }

    return last_count;
}

static void print_usage(const char *program_name) {
    printf("Usage: %s [options] <input file> <output file>\n", program_name);
    printf("Options:\n");
    printf("  -n, --no-palette           Exclude palette\n");
    printf("  -v, --var-name <name>      Specify alternative output variable name\n");
    printf("  --help                     Show this help message and exit\n");
}

static int parse_opts(int argc, char **argv, struct arguments *args) {
    /* Default values */
    args->no_palette = 0;
    args->var_name = NULL;

    /* Option definitions */
    const char *short_opts = "nv:";
    const struct option long_opts[] = {
        {"no-palette", no_argument, NULL, 'n'},
        {"var-name", required_argument, NULL, 'v'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1) {
        switch (opt) {
            case 'n':
                args->no_palette = 1;
                break;
            case 'v':
                args->var_name = optarg;
                break;
            case 'h':
                print_usage(argv[0]);
                return 1;
            case '?':
                print_usage(argv[0]);
                return 1;
            default:
                break;
        }
    }

    /* Remaining arguments should be the input and output files */
    if (optind + 2 != argc) {
        fprintf(stderr, "Error: Expected input and output file arguments\n");
        print_usage(argv[0]);
        return 1;
    }

    args->input_file = argv[optind];
    args->output_file = argv[optind + 1];

    return 0;
}

int main(int argc, char **argv) {
    struct arguments arguments;

    int opt_result = parse_opts(argc, argv, &arguments);
    if (opt_result) {
        exit(opt_result);
    }

    printf("Processing %s => %s\n", arguments.input_file, arguments.output_file);

    char *in_base;
    
    if (arguments.var_name) {
        in_base = arguments.var_name;
    } else {
        in_base = tr('.', '_', basename(arguments.input_file));
        in_base = tr('-', '_', in_base);
    }

    if (!in_base) {
        printf("Failed to dup string\n");
        exit(2);
    }

    FILE *pcx = fopen(arguments.input_file, "rb");
    if (!pcx) {
        printf("'%s': no such file\n", arguments.input_file);
        exit(3);
    }

    size_t buffer_size = file_size(pcx);
    if (buffer_size == -1) {
        printf("'%s': cannot stat\n", arguments.input_file);
        exit(4);
    }

    if (buffer_size >= UINT_MAX) {
        printf("File is too big for 32-bit system\n");
        exit(5);
    }

    char size_str[20];
    int sn = snprintf(size_str, 20, "%ld", buffer_size);
    if (!sn || sn >= 20) {
        printf("ERROR: snprintf failed\n");
        exit(6);
    }

    char *buffer = slurp(pcx, buffer_size);
    if (!buffer) {
        printf("Failed to slurp file\n");
        exit(7);
    }

    FILE *out = fopen(arguments.output_file, "w");
    if (!out) {
        printf("'%s': open failed\n", arguments.output_file);
        exit(8);
    }

    fputs("#ifndef __pcx_", out);
    fputs(in_base, out);
    fputs("_H\n#define __pcx_", out);
    fputs(in_base, out);
    fputs("_H\n\n#include <stdint.h>\n\n", out);

    fputs("uint32_t pcx_", out);
    fputs(in_base, out);
    fputs("_compressed_len_bytes = ", out);
    fputs(size_str, out);
    fputs(";\n\n", out);

    if (!arguments.no_palette) {
        fputs("uint16_t pcx_", out);
        fputs(in_base, out);
        fputs("_palette[] = {", out);

        if (!dump_palette(buffer + (buffer_size - 769), out)) {
            printf("ERROR: Palette dump failed\n");
            return false;
        }    

        fputs("\n};\n", out);

        fputs("uint32_t pcx_", out);
        fputs(in_base, out);
        fputs("_palette_len_words = 256;\n\n", out);
    }

    uint32_t data_size = buffer_size - 769 - 128;

    fputs("uint16_t pcx_", out);
    fputs(in_base, out);
    fputs("_data[] = {", out);

    size_t words = dump_image(data_size, buffer + 128, out);
    if (words == -1) {
        printf("ERROR: Image load failed\n");
        exit(9);
    }

    fputs("\n};\n", out);

    sn = snprintf(size_str, 20, "%ld", words);
    if (!sn || sn >= 20) {
        printf("ERROR: snprintf failed\n");
        exit(10);
    }

    fputs("uint32_t pcx_", out);
    fputs(in_base, out);
    fputs("_data_len_words = ", out);
    fputs(size_str, out);
    fputs(";\n\n", out);

    fputs("#endif\n", out);
    
    fclose(pcx);
}