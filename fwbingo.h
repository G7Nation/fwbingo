/*
 * number of times to shuffle, where a single "shuffle" involves chosing
 * two random indices in the "g_squares" array, and swapping them.
 * default to 8192.
 */
#define NUM_SHUFFLE (1 << 13)

/* 
 * names shouldn't be longer than this, just to help with displaying
 * the text in the grid.  for now, we'll filter anything longer than
 * this out, but we could also use the value to dynamically size the
 * grid.
 */
#define MAX_NAME_LEN 16

/*
 * type could be something like "javascript" or "programming language"
 * i don't intend to display them, but it would be nice to be able
 * filter based on keywords
 */
#define MAX_TYPE_LEN 32

/*
 * Min width of squares
 */
#define MIN_SQ_WIDTH (MAX_NAME_LEN + 2)

/*
 * Min height of squares
 */
#define MIN_SQ_HEIGHT 3

/*
 * truncates a call to printf
 */
#define _PRINT_LEN(x) #x
#define PRINT_LEN(x) _PRINT_LEN(x)

/* a name to be displayed in the grid, plus a type for filtering */
struct square {
    char name[MAX_NAME_LEN];
    char type[MAX_TYPE_LEN]; /* TODO currently unused, but meant for filtering */
};

/* macro for printing out formatted messages with some useful information
   such as file and line in which the message is printed.  variatic macros
   are a gcc extension, so if we're using a different compiler, switch to 
   regular printf, and print __FILE__ and __LINE__ as a separate statement.
   __FILE__ and __LINE__ macros go back to at least ANSI C 89, so should 
   be more or less universal among different compilers */
#if (defined(__GNUC__) || defined(__GNUG__)) && \
     !(defined(__clang__) || defined(__INTEL_COMPILER))
    #ifdef DEBUG
        #define FWB_LOG(msg, ...) \
            printf(msg " - %s:%d\n", ##__VA_ARGS__, __FILE__, __LINE__)
    #else
        #define FWB_LOG(msg, ...) \
            printf(msg, ##__VA_ARGS__)
    #endif
#else
    #ifdef DEBUG
        #define FWB_LOG printf("%s:%d\n", __FILE__, __LINE__);printf
    #else
        #define FWB_LOG printf
    #endif
#endif
