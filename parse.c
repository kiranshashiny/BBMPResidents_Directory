#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 8192

int is_empty(char *s) {
    while (*s) {
        if (!isspace((unsigned char)*s))
            return 0;
        s++;
    }
    return 1;
}

/* remove Photo */
void remove_photo(char *s) {
    char *p;
    while ((p = strstr(s, "Photo")) != NULL) {
        memmove(p, p + 5, strlen(p + 5) + 1);
    }
}

/* detect voter id line */
int is_voter_id_line(const char *s) {
    return (strstr(s, "$OH") ||
            strstr(s, "SOH") ||
            strstr(s, "OH"));
}

/* NAME line processor */
void process_name_line(char *line) {

    char out[8192] = "";
    char *p = line;

    while (*p) {

        if (strncmp(p, "Name", 4) == 0) {

            char value[256] = "";

            p += 4;

            while (*p && (*p == ':' || isspace((unsigned char)*p)))
                p++;

            int i = 0;

            while (*p && strncmp(p, "Name", 4) != 0) {
                value[i++] = *p;
                p++;
            }

            value[i] = '\0';

            /* trim spaces */
            int start = 0;
            while (isspace((unsigned char)value[start])) start++;

            int end = strlen(value) - 1;
            while (end >= 0 && isspace((unsigned char)value[end])) {
                value[end] = '\0';
                end--;
            }

            strcat(out, "Name : ");
            strcat(out, value);
            strcat(out, " | ");
        }
        else {
            p++;
        }
    }

    /* remove trailing pipe */
    int len = strlen(out);
    if (len >= 3 && out[len - 2] == '|')
        out[len - 3] = '\0';

    printf("%s\n", out);
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: %s input.txt\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");

    if (!fp) {
        printf("Cannot open file\n");
        return 1;
    }

    char line[MAX_LINE];

    while (fgets(line, sizeof(line), fp)) {

        line[strcspn(line, "\r\n")] = '\0';

        if (is_empty(line))
            continue;

        /* RULE 2: remove ID lines */
        if (is_voter_id_line(line))
            continue;

        /* clean Photo */
        remove_photo(line);

        /* NAME line */
        if (strstr(line, "Name :")) {

            process_name_line(line);
            continue;
        }

        /* RELATION line (unchanged from previous logic) */
        if (strstr(line, "Husband's Name") ||
            strstr(line, "Father's Name")) {

            printf("%s\n", line);
            continue;
        }

        /* default */
        printf("%s\n", line);
    }

    fclose(fp);
    return 0;
}
