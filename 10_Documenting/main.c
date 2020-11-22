/** @file */ 

#include <libgen.h>
#include <libintl.h>
#include <locale.h>

#include <stdio.h>
#include <string.h>

//! Definition to mark strings to localization
#define _(STRING) gettext(STRING)

//! Definition to setup path for searching for the localization files
#ifndef LOCALE_PATH
#define LOCALE_PATH "."
#endif

//! Array of first 100 roman numbert to convertion
const char* ROMAN_NUMBERS[99] = {
	"I", "II", "III", "IV", "V", "VI", "VII", "VIII", 
	"IX", "X", "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII",
	"XVIII", "XIX", "XX", "XXI", "XXII", "XXIII", "XXIV", "XXV",
	"XXVI", "XXVII", "XXVIII", "XXIX", "XXX", "XXXI", "XXXII", "XXXIII",
	"XXXIV", "XXXV", "XXXVI", "XXXVII", "XXXVIII", "XXXIX", "XL", "XLI",
	"XLII", "XLIII", "XLIV", "XLV", "XLVI", "XLVII", "XLVIII", "XLIX",
	"L", "LI", "LII", "LIII", "LIV", "LV", "LVI", "LVII", "LVIII", "LIX", 
	"LX", "LXI", "LXII", "LXIII", "LXIV", "LXV", "LXVI", "LXVII", "LXVIII", 
	"LXIX", "LXX", "LXXI", "LXXII", "LXXIII", "LXXIV", "LXXV", "LXXVI", "LXXVII", 
	"LXXVIII", "LXXIX", "LXXX", "LXXXI", "LXXXII", "LXXXIII", "LXXXIV", "LXXXV", "LXXXVI", 
	"LXXXVII", "LXXXVIII", "LXXXIX", "XC", "XCI", "XCII", "XCIII", "XCIV", "XCV", "XCVI", 
	"XCVII", "XCVIII", "XCIX", "C"};


/*! Function to convert roman number to integer
	\param roman is roman number to convert
	\return arabic equivalent to input number
*/
int romanToArabic(const char* roman)
{
	for (int i = 0; i < 99; ++i)
	{
		if (strcmp(ROMAN_NUMBERS[i], roman) == 0)
		{
			return i + 1;
		}
	}
	return -1;
}

/*! Function to convert integer to roman number
	\param arabic is integer to convert
	\return roman number that is equivalent to input integer
*/
const char* arabicToRoman(int arabic)
{

	return ROMAN_NUMBERS[arabic - 1];
}

/*! Print program version to standart output*/
void printVersion()
{
	printf(
		"game 1.0\n"
	);
}


/*! Pattern for output help for --help and --help-dx options*/
#define HELP_PATTERN  _(\
			"It is a simple game \"guess the number\" in which you are asked to guess a number, and your adversary will try to guess it by asking you questions%1$s%1$s"\
			"Usage: game [--help] [-r]%1$s%1$s"\
			"Options:%1$s"\
			"\t--help,  -- print help message%1$s"\
			"\t-r,      -- use roman numbers in game%1$s%1$s"\
			"Examles:%1$s"\
			"\tgame        play game%1$s"\
			"\tgame -r     play game using roman number%1$s"\
		)

/*! Print help message to standart output*/
void printHelp()
{
	printf(HELP_PATTERN, "\n");
}

/*! Print help message for doxygen start page to standart output*/
void printHelpDoxygen()
{
	printf("/*!\n\\mainpage\n");
	printf(HELP_PATTERN, "\n\n");
	printf("*/\n");
}

void askUser(int number, int useRoman)
{
	if (useRoman == 0)
	{
		printf(_("Is your number over %d?\n"), number);
	}
	else
	{
		printf(_("Is your number over %s?\n"), arabicToRoman(number));
	}
}

void printAnswer(int number, int useRoman)
{
	if (useRoman == 0)
	{
		printf(_("Your number is %d.\n"), number);
	}
	else
	{
		printf(_("Your roman number is %s.\n"), arabicToRoman(number));
	}
}

int main(int argc, char** argv)
{

	setlocale(LC_ALL, "");
	bindtextdomain ("game", LOCALE_PATH);
	textdomain ("game");

	int useRoman = 0;

	int argPos = 1;
	if (argc != 1)
	{
		while (argPos < argc)
		{
			if (strcmp(argv[argPos], "--help") == 0)
			{
				printHelp();
				return 0;
			}
			else if (strcmp(argv[argPos], "--help-dx") == 0)
			{
				printHelpDoxygen();
				return 0;
			}
			else if (strcmp(argv[argPos], "--version") == 0)
			{
				printVersion();
				return 0;
			}
			else if (strcmp(argv[argPos], "-r") == 0)
			{
				useRoman = 1;
			}
			argPos += 1;
		}
	}

	printf(_("Please guess a number from 1 to 100:\n"));

	char* yes_ans = _("yes");
	char* no_ans = _("no");

	char ans[256] = {0};

	int start = 1;
	int end = 100;
	int val = (start + end) / 2;

	while (1)
	{
		askUser(val, useRoman);
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
			printAnswer(val, useRoman);
			break;
		}
	}

	return 0;
}