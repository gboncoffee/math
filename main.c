#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include"compiler_interface.h"
#define EXP_BUF_INIT_SIZE 0x40 // literally arbitrary

/*
 * Copyright (c) 2022 Gabriel G. de Brito
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of 
 * this software and associated documentation files (the “Software”), to deal 
 * in the Software without restriction, including without limitation the rights to 
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
 * of the Software, and to permit persons to whom the Software is furnished to do 
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */

/*
 * author Gabriel G. de Brito gabrielgbrito@icloud.com
 * version 0.0.0
 * since Jul 31, 2022
 */

/*
 * REPL for the math vm
 */

int main(int argc, char *argv[]) {

    if (argc > 1) {
        printf("TODO: implement evaluation of cli args");
    }


    char *buf = (char*) malloc(sizeof(char) * EXP_BUF_INIT_SIZE);
    int buf_size = EXP_BUF_INIT_SIZE;
    int buf_loc  = 0;
    char c;
    printf("-> ");
    fflush(stdout);
    for (;;) {

        c = getchar();

        if (c == EOF) {
            free(buf);
            putchar('\n');
            break;
        }

        if (c == '\n') {

            buf[buf_loc] = '\0';
            eval(buf);
            free(buf);

            buf = (char*) malloc(sizeof(char) * EXP_BUF_INIT_SIZE);
            buf_size = EXP_BUF_INIT_SIZE;
            buf_loc  = 0;
            errno = 0; // we set it to zero so it'll not complain of older errors

            printf("-> ");
            fflush(stdout);
            continue;

        } else {
            buf[buf_loc] = c;
        }

        buf_loc++;

        // increase buffer
        if (buf_loc == buf_size) {
            buf = (char *) realloc(buf, 
                    ((buf_size / EXP_BUF_INIT_SIZE) + 1) * sizeof(signed long)
                    );
            buf_size += EXP_BUF_INIT_SIZE;
        }
    }

    return(errno);
}
