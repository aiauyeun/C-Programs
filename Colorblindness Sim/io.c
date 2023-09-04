#include "io.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct buffer {
    int fd; // file descriptor from open() or creat()
    int offset; // offset into buffer a[]
    // next valid byte (readiext empty location (writing)ng)
    // n
    int num_remaining; // number of bytes remaining in buffer (reading)
    uint8_t a[BUFFER_SIZE]; // buffer
};

//opens a file with read perms. Returns NULL if unsucc. Creates a new Buffer with fd set to filename's
//and everything else 0
Buffer *read_open(const char *filename) {
    int fudee = open(filename, O_RDONLY);
    if (fudee < 0) {
        return NULL;
    }
    Buffer *b = (Buffer *) malloc(sizeof(Buffer));
    b->fd = fudee;
    b->offset = 0;
    b->num_remaining = 0;
    return b;
}

//closes the buffer stored in the pointer. Frees the buffer and sets the input pointer to NULL!
//pointer to pointer input allows for us
//to modify the pointer ig :/
void read_close(Buffer **pbuf) {
    close((*pbuf)->fd);
    free(*pbuf);
    *pbuf = NULL;
}

//opens the file to do write perms prob same as read open
Buffer *write_open(const char *filename) {
    int fudee = creat(filename, 0664);
    if (fudee < 0) {
        return NULL;
    }
    Buffer *b = (Buffer *) malloc(sizeof(Buffer));
    b->fd = fudee;
    b->offset = 0;
    b->num_remaining = 0;
    return b;
}

//write accumulated bytes in *pbuf->a to the file in *pbuf->fd then close the file. Free Buff
//free buffer
void write_close(Buffer **pbuf) {
    uint8_t *start = (*pbuf)->a;
    int num_bytes = (*pbuf)->offset;

    do {
        ssize_t rc = write((*pbuf)->fd, start, num_bytes);
        if (rc < 0) {
            return;
        }
        start += rc; // skip past the bytes that were just written
        num_bytes -= rc; // how many bytes are left?
    } while (num_bytes > 0);

    //write((*pbuf)->fd, (*pbuf)->a, (BUFFER_SIZE - (*pbuf)->num_remaining));
    close((*pbuf)->fd);
    free(*pbuf);
    *pbuf = NULL;
}

//if buffer is full, refill it with stuff from the target file in the buffer. set *x to the next
//byte in the buffer and increment buf->offset by 1
//this fuction is to set *x to the val of the next byte avail!!!
bool read_uint8(Buffer *buf, uint8_t *x) {
    if (buf->num_remaining == 0) {
        //fills the buffer
        ssize_t rc = read(buf->fd, buf->a, sizeof(buf->a));
        if (rc < 0) {
            printf(
                "fuck u fucked up looks like some shitty error happened with reading 8 bit shit");
            return false;
        }
        if (rc == 0) {
            return false;
        }
        //num remaining = bytes read from read funct!
        buf->num_remaining = rc;
        buf->offset = 0;
    }
    //copy next valid byte in buffer to x and then increment offset to next avail and numremain accordingly
    *x = buf->a[buf->offset];
    //next unread byte here!
    buf->offset += 1;
    //one less valid byte left!
    buf->num_remaining -= 1;
    return true;
}

//call uint8 2x to read 2 bytes to see if we have 16 bits to read or if the file is over
//if false is returned by either one, return false.
//else: copy 2nd byte into new uint16_t var. t
bool read_uint16(Buffer *buf, uint16_t *x) {
    //set up dummy bytes to check if there are 16 bits to read in the file
    uint8_t byte1, byte2;
    //read uint8 2x. if theres no more file to read return false
    if (read_uint8(buf, &byte1) == false || read_uint8(buf, &byte2) == false) {
        return false;
    }
    //serialization time!!!
    //shift 2nd bit left by 8 orred with firstst bit
    uint16_t shifted = byte2 << 8;
    uint16_t result = shifted | byte1;
    *x = result;
    return true;
}

//same thing but now 32 bits!
bool read_uint32(Buffer *buf, uint32_t *x) {
    uint16_t byte1, byte2;
    if (read_uint16(buf, &byte1) == false || read_uint16(buf, &byte2) == false) {
        return false;
    }
    uint32_t shifted = byte2 << 16;
    uint32_t serialized = shifted | byte1;
    *x = serialized;
    return true;
}

void write_uint8(Buffer *buf, uint8_t x) {
    //check if buffer is full
    if (buf->offset == BUFFER_SIZE) {
        uint8_t *start = buf->a;
        int num_bytes = buf->offset;

        //this will write shit into the fd file until the buffer is empty!
        do {
            //write num_bytes bytes into start.
            ssize_t rc = write(buf->fd, start, num_bytes);
            if (rc < 0) {
                printf("fucked up writing loser");
                return;
            }
            start += rc;
            num_bytes -= rc;
        } while (num_bytes > 0);
        //empty bytes back to 0
        buf->offset = 0;
    }
    //set next free byte to x!
    buf->a[buf->offset] = x;
    //increment offset by 1
    buf->offset += 1;
    //I dont want to think rn but will we have to increment num_remaining? wait nvm thats not even used for writing.
}

void write_uint16(Buffer *buf, uint16_t x) {
    write_uint8(buf, x);
    write_uint8(buf, (x >> 8));
}

void write_uint32(Buffer *buf, uint32_t x) {
    write_uint16(buf, x);
    write_uint16(buf, (x >> 16));
}
