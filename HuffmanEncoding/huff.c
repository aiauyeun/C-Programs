#include "bitwriter.h"
#include "pq.h"

#include <stdio.h>
#include <unistd.h>
#define OPTIONS "i:o:h"

typedef struct Code {
    uint64_t code;
    uint8_t code_length;
} Code;

void tree_free(Node *node) {
    if (node == NULL) {
        return;
    }
    tree_free(node->right);
    tree_free(node->left);
    node_free(&node);
}
//1)clear the old histogrram
//2)Read the file in inbuf
//3)For each byte read add it to the histogram.
//4)Incremeaant filesize too. Return it.
//ensure that at least two leaves are here.
uint64_t fill_histogram(Buffer *inbuf, double *histogram) {
    for (int i = 0; i < 256; i++) {
        histogram[i] = 0.0;
    }
    histogram[0x00]++;
    histogram[0xff]++;
    int filesize = 0;
    uint8_t byte;
    bool guber = read_uint8(inbuf, &byte);
    while (guber) {
        //printf("Byte: %d\n", byte);
        histogram[byte]++;
        filesize++;
        guber = read_uint8(inbuf, &byte);
    }
    return filesize;
}

//creates and returns pointer to Huff Tree.
//iterates thru vals in histogram and then creates tree. geegegegee

Node *create_tree(double *histogram, uint16_t *num_leaves) {
    PriorityQueue *pq = pq_create();
    //go through histogram and grab values and add them to the pq
    for (int i = 0; i < 256; i++) {
        if (histogram[i] > 0) {
            //printf("%d\n",i);
            Node *node = node_create(i, histogram[i]);
            enqueue(pq, node);
            (*num_leaves)++;
        }
    }
    //fdeselksedglkjsefklj
    Node *left = NULL;
    Node *right = NULL;
    while (!pq_is_empty(pq) && !pq_size_is_1(pq)) {
        dequeue(pq, &left);
        dequeue(pq, &right);
        Node *branch = node_create(69, left->weight + right->weight);
        branch->left = left;
        branch->right = right;
        enqueue(pq, branch);
        //node_print_tree(branch,'<', 0);
    }
    //printf("Wow look the size of our queue is: %d", pq_size_is_1(pq));

    dequeue(pq, &left); //check if this works. if no just make a new node
    //node_print_tree(left,'<', 0);
    pq_free(&pq);
    return left;
}

//recursively fills in the code table. basically reads the huffman tree and stores the codes.
void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) {
    if (node->left != NULL && node->right != NULL) {
        fill_code_table(code_table, node->left, code, code_length + 1);
        code |= 1 << code_length;
        fill_code_table(code_table, node->right, code, code_length + 1);
    } else {
        /* Leaf node: store the Huffman Code. */
        code_table[node->symbol].code = code;
        code_table[node->symbol].code_length = code_length;
    }
}

//outbuf writes to the output file
//inbuf: reads the input file. we need to close the buffer and then reopen it after calling fill_histogram
//the filesize
//num leaves
//

void huff_write_tree(BitWriter *outbuf, Node *node) {
    if (node->left != NULL && node->right != NULL) {
        huff_write_tree(outbuf, node->left);
        huff_write_tree(outbuf, node->right);
        bit_write_bit(outbuf, 0);
    } else {
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, node->symbol);
    }
}

void huff_compress_file(BitWriter *outbuf, Buffer *inbuf, uint64_t filesize, uint16_t num_leaves,
    Node *code_tree, Code *code_table) {

    bit_write_uint8(outbuf, 'H');
    bit_write_uint8(outbuf, 'C');
    bit_write_uint32(outbuf, filesize);
    bit_write_uint16(outbuf, num_leaves);
    huff_write_tree(outbuf, code_tree);
    //node_print_tree(code_tree,'<', 0);
    uint8_t byte;
    bool guber = read_uint8(inbuf, &byte);
    while (guber) {

        uint64_t code = code_table[byte].code;
        uint8_t code_length = code_table[byte].code_length;
        for (int i = 0; i < code_length; i++) {
            bit_write_bit(outbuf, code & 1);
            code >>= 1;
        }
        guber = read_uint8(inbuf, &byte);
    }
}

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
        case 'h':
            printf("huff:  -i option is required\nUsage: huff -i infile -o outfile\n       huff -v "
                   "-i infile -o outfile\n       huff -h");
            return 0;
        }
    }
    // 1. Create a histogram of the input file’s bytes/symbols (see fill_histogram()).
    // 2. Create a code tree from the histogram (see create_tree()).
    // 3. Fill a 256-entry code table, one entry for each byte value (see fill_code_table()).
    // 4. Close and Reopen the input file in preparation for Step 5
    // 5. Create a Huffman Coded output file from the input file (see huff_compress_file()).
    if (i == 1 && o == 1) {
        //printf("\nThis is our infile: %s. This is our outfile: %s\n", infile, outfile);
        double histogram[256];
        Buffer *inbuf = read_open(infile);
        //read infile and store char freqs in histogram.
        uint64_t file_size = fill_histogram(inbuf, histogram); //this works!
        read_close(&inbuf);
        // for (int i = 0; i < 256; i++) {
        //     if(histogram[i] != 0) {
        //         printf("%d: %lf \n", i, histogram[i]);
        //     }
        // }
        //create a huff tree from that histogram
        uint16_t num_leaves = 0;

        Node *tree = create_tree(histogram, &num_leaves); //this works
        // printf("LEAF : %d",num_leaves);
        // printf("FILE : %d",file_size);
        //create code table from tree.
        Code *code = calloc(sizeof(Code), 256);
        fill_code_table(code, tree, 0, 0);
        // for (int i = 0; i < 256; i++) {
        //     if(code[i].code_length != 0) {
        //         printf("Code: %d \n Code Length: %d \n", code[i].code, code[i].code_length);
        //     }
        // }
        inbuf = read_open(infile);
        BitWriter *outbuf = bit_write_open(outfile);
        //printf("\n wtf is going on \n");
        huff_compress_file(outbuf, inbuf, file_size, num_leaves, tree, code);
        read_close(&inbuf);
        bit_write_close(&outbuf);
        free(code);
        tree_free(tree);
    } else {
        if (i == 0) {
            printf("huff:  -i option is required\n");
        } else {
            printf("huff:  -o option is required\n");
        }
    }
}
