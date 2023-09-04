#include "bmp.h"
#include "io.h"

#include <stdio.h>
#include <unistd.h>
#define OPTIONS "i:o:h"

int main(int argc, char **argv) {
    int opt = 0;
    char *infile = NULL;
    char *outfile = NULL;
    int i = 0;
    int o = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            infile = optarg;
            // if (infile == NULL) {
            //     fprintf(stderr, "colorb: error reading input file %s\n", optarg);
            //     //remember to test for valgrind invalid functions!!!!
            //     return 0;
            // }
            i = 1;
            break;
        case 'o':
            outfile = optarg;
            // if (outfile == NULL) {
            //     fprintf(stderr, "colorb: error reading output file %s\n", optarg);
            //     return 0;
            // }
            o = 1;
            break;
        case 'h': printf("Usage: colorb -i infile -o outfile \n       colorb -h\n"); return 0;
        }
    }
    if (i == 1 && o == 1) {
        //Goal: modify the file
        //create a buffer that looks at the infile
        Buffer *buf = read_open(infile);

        //turn file into bmp file
        BMP *inbmp = bmp_create(buf);

        read_close(&buf);

        //convert to colorblind
        bmp_reduce_palette(inbmp);

        //write a new file
        Buffer *outbuf = write_open(outfile);
        //write_close(&outbuf);

        // //make this the bmp file
        bmp_write(inbmp, outbuf);

        bmp_free(&inbmp);

        write_close(&outbuf);

    } else {
        if (i == 0) {
            printf("colorb:  -i option is required\n");
        } else {
            printf("colorb:  -o option is required\n");
        }
    }
}
