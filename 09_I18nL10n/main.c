#include <libgen.h>
#include <libintl.h>
#include <locale.h>

#include <stdio.h>
#include <string.h>

#define _(STRING) gettext(STRING)

#ifndef LOCALE_PATH
#define LOCALE_PATH "."
#endif

int main(int argc, char** argv)
{

	setlocale(LC_ALL, "");
	bindtextdomain ("game", LOCALE_PATH);
	textdomain ("game");

	printf(_("Please guess a number from 1 to 100:\n"));

	char* yes_ans = _("yes");
	char* no_ans = _("no");

	char ans[256] = {0};

	int start = 1;
	int end = 100;
	int val = (start + end) / 2;

	while (1)
	{
		printf(_("Is your number over %d?\n"), val);
		printf(_("Enter [%s|%s]: "), yes_ans, no_ans);
		if (scanf("%255s", ans) != 1)
		{
			fprintf(stderr, _("Something went wrong, are you really playing by the rules?\n"));
			return 1;
		}
		if (strncmp(ans, yes_ans, strlen(yes_ans)) == 0)
		{
			start = val + 1;
			val = (start + end) / 2;	
		}
		else if (strncmp(ans, no_ans, strlen(no_ans)) == 0)
		{
			end = val;
			val = (start + end) / 2;
		}
		else
		{
			printf(_("I'm sorry, but I don't understand your answer. Let's try again.\n"));
			continue;
		}

		if (end == start)
		{
			printf(_("Your number is %d.\n"), val);
			break;
		}
	}

	return 0;
}