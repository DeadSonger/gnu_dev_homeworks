#ifndef PCRE2_CODE_UNIT_WIDTH
#define PCRE2_CODE_UNIT_WIDTH 8
#endif

#include <stdio.h>
#include <string.h>
#include <pcre2.h>
#include "own_regexp.h"


int matchRegexp(const unsigned char* pattern, const unsigned char* subject, WINDOW* output)
{
    pcre2_code *re;

    int errnum;
    int i, rc;

    PCRE2_SIZE erroffs;
    PCRE2_SIZE *ovector;
    PCRE2_SIZE subject_length;

    pcre2_match_data *match_data;

    subject_length = (PCRE2_SIZE)strlen((char *)subject);

    int flags = PCRE2_UCP; 
#ifdef WITHOUT_UTF
#pragma message("PCRE2 compile regexp without PCRE2_UCP flag")
    flags = 0;
#endif

    re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, flags, &errnum, &erroffs, NULL);

    if (re == NULL) {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errnum, buffer, sizeof(buffer));
        wprintw(output, "  PCRE2 compilation failed at offset %d: %s\n", (int)erroffs,
               buffer);
        return 1;
    }

    match_data = pcre2_match_data_create_from_pattern(re, NULL);

    rc = pcre2_match(re, subject, subject_length, 0, 0, match_data, NULL);

    if (rc < 0) {
        switch(rc) {
        case PCRE2_ERROR_NOMATCH:
            wprintw(output, "  No match\n");
            break;
        default:
            wprintw(output, "  Matching error %d\n", rc);
            break;
        }
        pcre2_match_data_free(match_data);   /* Release memory used for the match */
        pcre2_code_free(re);                 /*   data and the compiled pattern. */
        return 1;
    }

    ovector = pcre2_get_ovector_pointer(match_data);

    for (i = 0; i < rc; i++)
        wprintw(output, "  %2ld: %.*s\n", ovector[2*i], 
			     (int)(ovector[2*i+1] - ovector[2*i]),
			     subject + ovector[2*i]);

    pcre2_match_data_free(match_data);  /* Release the memory that was used */
    pcre2_code_free(re);                /* for the match data and the pattern. */

    return 0;
}
