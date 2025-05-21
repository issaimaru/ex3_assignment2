#include <stdarg.h>

#include "io.h"

void putline(const char *s)
{
	char ch;

	while (ch = *s++)
		io_putch(ch);
}

int puts(const char *s)
{
	putline(s);
	io_putch('\n');

	return 1;
}

int putchar(int c)
{
    io_putch((char)c);
    return c;
}

#include <stdint.h>
static void puthex(unsigned long long num, char pad, int width)
{
	if (num >= 0x10) {
		puthex(num >> 4, pad, width - 1);
	} else {
		while (--width > 0)
			io_putch(pad);
	}
	num &= 0xf;
	if (num < 0xa) {
		io_putch('0' + num);
	} else {
		io_putch('a' + num - 0xa);
	}
}

static void putdec(long long num, int sign, char pad, int width)
{
	if (num >= 10) {
		putdec(num / 10, sign, pad, width - 1);
	} else {
		while (--width > 0)
			io_putch(pad);
		if (sign && pad == ' ')
			io_putch('-');
	}
	io_putch('0' + (num % 10));
}

#if defined(ENABLE_FLOAT_PRINTF)
static double putfloat(double num, int width)
{
	uint64_t inum;

	if (num >= 1000000000.0) {
		num = putfloat(num / 1000000000.0, width) * 1000000000.0;
		width = 9;
	}
	inum = (uint64_t)num;
	putdec(inum, 0, '0', width);
	return num - (double)inum;
}
#endif

int printf(const char *s, ...)
{
	int ret;
	va_list ap;
	int fmt = 0;
	int l = 0;
	int ll = 0;
	char pad = ' ';
	int width = 0;

	va_start(ap, s);

	for (; *s; s++) {
		if (fmt) {
			switch (*s) {
			case 'l':
				if (s[1] == 'l') {
					ll = 1;
					s++;
				} else {
					l = 1;
				}
				break;
			case 'p':
				l = 1;
				io_putch('0');
				io_putch('x');
				// fall thru
			case 'x': {
				long long num = l ? va_arg(ap, unsigned long) : (ll ? va_arg(ap, unsigned long long) : va_arg(ap, unsigned int));

				puthex(num, pad, width);
				fmt = 0;
				break;
			}
			case 'd': {
				long long num = l ? va_arg(ap, long) : (ll ? va_arg(ap, long long) : va_arg(ap, int));
				int sign = 0;

				if (num < 0) {
					if (pad == '0')
						io_putch('-');
					num = -num;
					sign = 1;
					width--;
				}
				putdec(num, sign, pad, width);
				fmt = 0;
				break;
			}
#if defined(ENABLE_FLOAT_PRINTF)
			case 'f': {
				double num = va_arg(ap, double);

				num = putfloat(num, 0);
				io_putch('.');
				putdec((long)(num * 1000000), 0, ' ', 0);
				fmt = 0;
				break;
			}
#endif
			case 's': {
				const char *s2 = va_arg(ap, const char *);

				putline(s2);
				fmt = 0;
				break;
			}
			case 'c':
				io_putch((char)va_arg(ap, int));
				fmt = 0;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				width = width * 10 + *s - '0';
				break;
			}
		} else if (*s == '%') {
			fmt = 1;
			l = 0;
			ll = 0;
			pad = ' ';
			width = 0;
			if (s[1] == '0') {
				pad = '0';
				s++;
			}
		} else {
			io_putch(*s);
		}
	}
	va_end(ap);
	return ret;
}

