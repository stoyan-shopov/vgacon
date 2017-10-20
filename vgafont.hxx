#ifndef VGAFONT_HXX
#define VGAFONT_HXX

#include <QImage>

extern class VGAFont
{
public:
/*
Color palette:

CGA	EGA	VGA	RGB	Web	Example
0x0	0x0	0,0,0	0,0,0	#000000	black
0x1	0x1	0,0,42	0,0,170	#0000aa	blue
0x2	0x2	00,42,00	0,170,0	#00aa00	green
0x3	0x3	00,42,42	0,170,170	#00aaaa	cyan
0x4	0x4	42,00,00	170,0,0	#aa0000	red
0x5	0x5	42,00,42	170,0,170	#aa00aa	magenta
0x6	0x14	42,21,00	170,85,0	#aa5500	brown
0x7	0x7	42,42,42	170,170,170	#aaaaaa	gray
0x8	0x38	21,21,21	85,85,85	#555555	dark gray
0x9	0x39	21,21,63	85,85,255	#5555ff	bright blue
0xA	0x3A	21,63,21	85,255,85	#55ff55	bright green
0xB	0x3B	21,63,63	85,255,255	#55ffff	bright cyan
0xC	0x3C	63,21,21	255,85,85	#ff5555	bright red
0xD	0X3D	63,21,63	255,85,255	#ff55ff	bright magenta
0xE	0x3E	63,63,21	255,255,85	#ffff55	yellow
0xF	0x3F	63,63,63	255,255,255	#ffffff	white
*/
	enum VGA_COLOR_CODE
	{
		BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, GRAY, DARK_GRAY, BRIGHT_BLUE, BRIGHT_GREEN, BRIGHT_CYAN, BRIGHT_RED, BRIGHT_MAGENTA, YELLOW, WHITE,
		COLOR_CODES_COUNT,
	};
	enum
	{
		FOREGROUND_COLOR_INDEX		= 1,
		BACKGROUND_COLOR_INDEX		= 0,
	};
	int width(void) const { return VGA_FONT_WIDTH; }
	int height(void) const { return VGA_FONT_HEIGHT; }
	static QRgb vgaRGBColor(unsigned palette_index) { return color_codes[std::min(palette_index, sizeof color_codes / sizeof * color_codes - 1)]; }
	QImage imageForCharacter(uint8_t character_code, int foreground_color_code, int background_color_code);
private:
	static const QRgb color_codes[COLOR_CODES_COUNT];
	enum
	{
		VGA_FONT_WIDTH	=	9,
		VGA_FONT_HEIGHT	=	14,

		FONT_BITMAP_CHARACTER_ROWS	= 4,
		FONT_BITMAP_CHARACTER_COLUMNS	= 64,
		FONT_X_OFFSET			= 0,
		FONT_Y_OFFSET			= 0,
	};
	const char * font_image_file_name = ":/vga-font-9x14.png";
	/*! \todo	for some reason, initializing the font image with an in-class initializer does not work on android */
	QImage font_image = QImage(font_image_file_name);
} vga_font;

#endif // VGAFONT_HXX
