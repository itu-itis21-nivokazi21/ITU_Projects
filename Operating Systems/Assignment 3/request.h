#ifndef __REQUEST_H__
#define __REQUEST_H__


enum result {
    OK,
    NotFound,
    Forbidden,
    Error
};

typedef struct __Buffer_t {
    int fd;
    int is_static;
    off_t size;
    char pathname[MAXBUF];
    char cgiargs[MAXBUF];
    int handling;
} Buffer_t;

void requestHandle(int fd);
void requestServeDynamic(int fd, char *filename, char *cgiargs);
void requestServeStatic(int fd, char *filename, int filesize);

#endif // __REQUEST_H__
