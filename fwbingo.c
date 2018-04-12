#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "fwbingo.h"

/* global list of all values for the grid */
int g_len;
struct square *g_squares;

/*
 * swap function for shuffling the deck
 */
void swap_squares(struct square *sq1, struct square *sq2)
{
    struct square tmp;
    memcpy(&tmp, sq1, sizeof(struct square));
    memcpy(sq1, sq2, sizeof(struct square));
    memcpy(sq2, &tmp, sizeof(struct square));
}

/*
 * simple wrapper to open a file
 */
FILE *open_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        FWB_LOG("failed to open file: %s", filename);
    }
    return fp;
}

/*
 * return file-line count, rewind the file
 */
int count_file_lines(FILE *fp)
{
    char c;
    int nlines = 0;
    if (fp == NULL)
        return -1;

    for (c=getc(fp); c != EOF; c=getc(fp))
        if (c == '\n')
            nlines++;
    rewind(fp);
    return nlines;
}

/*
 * shuffle the frameworks for a random draw
 */
void shuffle(struct square *squares, int max)
{
    int i, x, y;

    /* we do this enough times that a "self-swap" doesn't matter much */
    for (i=0; i<NUM_SHUFFLE; i++) {
        x = rand() % max;
        y = rand() % max;
        swap_squares(&squares[x], &squares[y]);
    }
}

/* parse a file and return a list of square structs */
struct square *parse_file(FILE *fp)
{
    int i, j;
    int nlines;
    char *nl; /* for replacing newline chars in framework names */
    char line[MAX_NAME_LEN];
    struct square *squares = NULL;

    if (fp == NULL) {
        FWB_LOG("fp is NULL");
        return NULL;
    }

    /* fp has been checked for NULL already, no need to check nlines == -1 */
    nlines = count_file_lines(fp);

    squares = calloc(nlines, sizeof(struct square));
    if (squares == NULL) {
        FWB_LOG("failed to allocate squares of size %ld", nlines);
        goto out;
    }

    /* 
     * there may be more lines than valid values, so i indexes the line
     * and j indexes the squares array
     */
    for (i=0, j=0; i<nlines; i++) {
        if (fgets(line, MAX_NAME_LEN, fp) == NULL) {
            FWB_LOG("failed to fgets");
            free(squares);
            return NULL;
        }

        /* ignore comments and empty lines */
        if (line[0] != '#' && line[0] != '\n') {
            /* get rid of newline char if it exists */
            if ((nl = strchr(line, '\n')) != NULL) {   
                *nl = '\0';
            }
            
            /* strncpy doesn't null-terminate */
            strncpy(squares[j].name, line, MAX_NAME_LEN);
            squares[j].name[MAX_NAME_LEN-1] = '\0';
            j++;
        }
        
    }
    g_len = j; /* number squares */
out:
    fclose(fp); /* not sure if you can pass NULL here? */
    return squares;
}

/* backup function */
int main(int argc, void *argv[])
{
    int i, j, index;
    char out[MAX_NAME_LEN];
    FILE *fp;
    time_t t;
    struct square *g_squares;
    srand(time(&t));
    if ((fp = open_file("fwb_list.txt")) == NULL) {
        return 1;
    }

    g_len = count_file_lines(fp);

    g_squares = parse_file(fp);
    if (g_squares == NULL) {
        printf("ERROR: failed to allocate g_squares\n");
        exit(1);
    }

    /* get a random draw w/o replacement */
    shuffle(g_squares, g_len);

    for (i=0, index=0; i<5; i++) {
        for (j=0; j<5; j++, index++) {
            /* 20s equal to MAX_NAME_LEN */
            printf("%" PRINT_LEN(MAX_NAME_LEN) "s", g_squares[index].name);
        }
        printf("\n");
    }
    printf("\n");


    free(g_squares);
    return 0;   
}
