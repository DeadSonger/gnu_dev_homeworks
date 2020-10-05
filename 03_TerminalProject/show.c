#include <ncurses.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>


#define char_t char

#define WINDOW_OFFSET 3

typedef struct FileInfo_
{
	char_t* buffer;
	size_t numLines;
	size_t* linesOffsets;
} FileInfo;

typedef struct State_
{
	WINDOW* window;
	FileInfo* fileInfo;
	size_t symbolsForLineNumber;
	size_t xOffset;
	size_t yOffset;
	size_t screenWidth;
	size_t screenHeight;
	bool needExit;
} State;

void printHelp(const char* filename)
{
	printf(
		"%s - simple file reader based on ncurses lib\n"
		"\tUsing: %s <filename> - open file <filename> to read\n"
		"\t       %s -h         - view this help message\n", 
		filename, filename, filename);
}

int readFile(const char* filename, char_t** buffer, size_t* bufferSize)
{
	assert(bufferSize != NULL);
	assert(buffer != NULL);

	*bufferSize = 0;

	FILE* file = fopen(filename, "r");
	if (file == NULL)
		return 1;

#ifndef SEEK_END
	#error "SEEK_END is undefineded"
#endif

	if (fseek(file, 0, SEEK_END) != 0)
	{
		fclose(file);		
		return 2;
	}

	fpos_t pos;

	if (fgetpos(file, &pos) != 0)
	{
		fclose(file);
		return 3;
	}

	*bufferSize = pos.__pos;
	
	*buffer = malloc((*bufferSize + 1) * sizeof(char_t));
	if (*buffer == NULL)
	{
		fclose(file);
		return 4;
	}

	rewind(file);
	
	int a;
	if ((a = fread(*buffer, sizeof(char_t), *bufferSize, file)) != *bufferSize)
	{		
		fclose(file);
		return 6;
	}
	
	fclose(file);
	return 0;
}

int generateFileInfo(char_t* buffer, size_t bufferSize, FileInfo* fileInfo)
{
	assert(buffer != NULL);
	assert(fileInfo != 0);

	size_t numLines = 0;
	for (size_t i = 0; i < bufferSize; ++i)
	{
		if (buffer[i] == '\n')
			numLines += 1;
	}

	size_t* linesOffset = malloc((numLines + 1) * sizeof(size_t));
	if (linesOffset == NULL)
		return 1;
	
	linesOffset[0] = 0;

	size_t lineIdx = 0;
	for (size_t i = 0; i < bufferSize; ++i)
	{
		if (buffer[i] == '\n')
		{
			buffer[i] = 0;
			lineIdx += 1;
			linesOffset[lineIdx] = i + 1;
		}
	}

	fileInfo->buffer = buffer;
	fileInfo->numLines = numLines;
	fileInfo->linesOffsets = linesOffset;

	return 0;
}

void initState(State* state, WINDOW* window, FileInfo* fileInfo)
{
	assert(state != NULL);

	state->fileInfo = fileInfo;

	state->window = window;
	getmaxyx(window, state->screenHeight, state->screenWidth);


	size_t tmp = fileInfo->numLines;
	state->symbolsForLineNumber = 0;
	while (tmp > 0)
	{
		tmp /= 10;
		state->symbolsForLineNumber += 1;
	}

	state->xOffset = 0;
	state->yOffset = 0;
	state->needExit = false;
}

void prepareScreen(const State* state)
{
	assert(state != NULL);

	werase(state->window);	
	
	box(state->window, 0, 0);
	char format[256] = {0};
	sprintf(format, "%%%ldld: %%.%ds", state->symbolsForLineNumber, state->screenWidth - 2);
	for (size_t i = 0; i < state->screenHeight - 2; ++i)
	{
		size_t lineNumber = state->yOffset + i;
		if (lineNumber >= state->fileInfo->numLines)
			break;

		size_t len = state->fileInfo->linesOffsets[lineNumber + 1] - state->fileInfo->linesOffsets[lineNumber];
		if (state->xOffset > len)
		{
			mvwprintw(state->window, i+1, 1, format, lineNumber+1, "");
			continue;
		}

		const char* line = state->fileInfo->buffer + state->fileInfo->linesOffsets[lineNumber] + state->xOffset;
		mvwprintw(state->window, i+1, 1, format, lineNumber+1, line); 
	}
	wmove(state->window, 0, 0);
	wrefresh(state->window);
}

void processInput(State* state)
{
	assert(state != NULL);
	
	switch(getch())
	{
	case 'q':
	case 'Q':
	case 27:	
		state->needExit = true;
		break;
	assert(state != NULL);
	case KEY_PPAGE:
		if (state->yOffset > state->screenHeight)
			state->yOffset -= state->screenHeight;
		else
			state->yOffset = 0;
		break;

	case KEY_NPAGE:
		if (state->yOffset + 2 * state->screenHeight < state->fileInfo->numLines)
			state->yOffset += state->screenHeight;
		else if (state->fileInfo->numLines < state->screenHeight)
			state->yOffset = 0;
		else
			state->yOffset = state->fileInfo->numLines - state->screenHeight;

	case KEY_UP:
		if (state->yOffset > 0)
			state->yOffset -= 1;
		break;

	case KEY_DOWN:
	case ' ':
		if (state->yOffset + state->screenHeight < state->fileInfo->numLines)
			state->yOffset += 1;
		break;

	case KEY_RIGHT:
		state->xOffset +=1;
		break;

	case KEY_LEFT:
		if (state->xOffset != 0)
			state->xOffset -= 1;
		break;
	}
}

int main(int argc, char** argv)
{
	if (argc == 1 || (argc == 2 && strcmp(argv[1], "-h") == 0))
	{
		printHelp(argv[0]);
		return 0;
	}

	const char* filename = argv[1];
	char_t *buffer = NULL;
	size_t	bufferSize = 0;
	int errVal = readFile(filename, &buffer, &bufferSize);
	if (errVal != 0)
	{
		printf("errVal = %d in func readFile\n", errVal);
		return 1;
	}

	FileInfo fileInfo;
	errVal = generateFileInfo(buffer, bufferSize, &fileInfo);
	if (errVal != 0)
	{
		printf("errVal = %d in func generateFileInfo\n", errVal);
		return 1;
	}

	WINDOW* mainWin = initscr();

	if (mainWin == NULL)
	{
		return 1;
	}
	
	noecho();
	keypad(mainWin, true);
	scrollok(mainWin, true);
	cbreak();

	size_t mainWinWidth, mainWinHeight;
	getmaxyx(mainWin, mainWinHeight, mainWinWidth);

	WINDOW* workWin = newwin(mainWinHeight - 2 * WINDOW_OFFSET, mainWinWidth - 2 * WINDOW_OFFSET, WINDOW_OFFSET, WINDOW_OFFSET); 
	
	if (workWin == NULL)
		return 1;

	keypad(workWin, true);
	scrollok(mainWin, true);

	mvwprintw(mainWin, 0, 1, "FileName: %s    lines: %ld", filename, fileInfo.numLines);

	State state;
	initState(&state, workWin, &fileInfo);
	

	wrefresh(mainWin);

	while (!state.needExit)
	{
		prepareScreen(&state);
		processInput(&state);
	}

	delwin(mainWin);
	endwin();

	return 0;	
}

