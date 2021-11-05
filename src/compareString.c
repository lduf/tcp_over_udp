#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include "includes/compareString.h"

int compareString(char string[], char rgx[]){
	printf("regex used : %s\n", rgx);
	regex_t regex;
	int reti;
	char msgbuf[100];

	int res = 0;

	/* Compile regular expression */
	reti = regcomp(&regex, rgx, REG_EXTENDED);
	if (reti) {
	    fprintf(stderr, "Could not compile regex\n");
	    exit(1);
	}

	/* Execute regular expression */
	reti = regexec(&regex, string, 0, NULL, 0);
	if (!reti) {
	    res = 1;
	}
	else if (reti == REG_NOMATCH) {
	    res = 0;
	}
	else {
	    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
	    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
	    exit(1);
	}

	/* Free memory allocated to the pattern buffer by regcomp() */
	regfree(&regex);
	return res;
}