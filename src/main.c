// #include <stdio.h>

extern volatile unsigned char hardware;

extern void gotoxy(unsigned char, unsigned char);
extern char putchar(char);
extern char puts(const char*);

#pragma disable_warning 115
#pragma clang diagnostic ignored "-Wmain-return-type"

void main(void)
{
	puts("Hello, world!");
	puts("Line 2: Line feed\ntest");
	puts("Not line 4: \rLine 3: Carriage return test");
	puts("Not\b\b\b\b\b\bLine 4: Backspace test");
	puts("Line 5:\tTab tests\t\t!\t1234567\tt");
	puts("1234567\t1234567\t123\ttest");
	puts("1234567\t1234567\t1\t\t\t\ttest");
	puts("Line 6: \vVertical tab test");
	puts("Line 7: Scroll\ntest");

	switch (hardware)
	{
		case 0:
			puts("Line 8: DMG system");
			break;
		case 1:
			puts("Line 8: CGB system");
			break;
		case 2:
			puts("Line 8: AGB system");
			break;
		default:
			puts("Line 8: Unknown system");
			break;
	}
}