/* $Id$ */

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "types.h"
#include "libemu.h"
#include "../global.h"
#include "font.h"
#include "gui.h"
#include "../os/strings.h"
#include "../unknown/unknown.h"
#include "../house.h"
#include "../structure.h"
#include "../unit.h"
#include "../os/math.h"
#include "../sprites.h"
#include "../codec/format80.h"
#include "../pool/pool.h"
#include "../pool/house.h"
#include "../pool/structure.h"
#include "../pool/unit.h"
#include "../string.h"
#include "../tools.h"
#include "widget.h"

extern void emu_GUI_CopyFromBuffer();
extern void emu_GUI_CopyToBuffer();
extern void f__1DD7_022D_0015_1956();
extern void f__1DD7_0B53_0025_36F7();
extern void f__22A6_034F_000C_5E0A();
extern void f__22A6_04A5_000F_3B8F();
extern void f__22A6_1102_004C_B069();
extern void f__22A6_127B_0036_F8C9();
extern void emu_Tools_Malloc();
extern void emu_Tools_Free();
extern void f__23E1_0334_000B_CF65();
extern void f__24D0_000D_0039_C17D();
extern void f__2598_0000_0017_EB80();
extern void f__259E_0006_0016_858A();
extern void f__2599_000B_0047_21FD();
extern void f__259E_0021_001A_E253();
extern void f__259E_0040_0015_5E4A();
extern void f__2605_000C_006D_F8B2();
extern void f__2642_0002_005E_87F6();
extern void f__2642_0069_0008_D517();
extern void f__29E8_07FA_0020_177A();
extern void f__2B6C_0137_0020_C73F();
extern void f__2B6C_0169_001E_6939();
extern void f__B483_04CB_0015_EBB4();
extern void f__B48B_0000_001E_7E97();
extern void f__B48B_03A4_0005_619A();
extern void f__B4B8_0D23_0010_BA99();
extern void f__B4B8_110D_000D_FD5C();
extern void f__B4DA_02E0_0023_E297();
extern void f__B4DA_0AB8_002A_AAB2();
extern void f__B4E9_0050_003F_292A();
extern void f__B511_0E44_000C_24F5();
extern void f__B518_0B1D_0014_307D();
extern void f__B518_0EB1_000E_D2F5();
extern void f__B518_14D4_0013_5ED7();
extern void f__B518_14F2_003E_977C();
extern void f__B518_0558_0010_240A();
extern void emu_Input_HandleInput();
extern void emu_Input_History_Clear();
extern void emu_Input_Keyboard_NextKey();
extern void emu_File_ReadChunkOrLengthFile();
extern void emu_GUI_DrawFilledRectangle();
extern void emu_GUI_DrawChar();
extern void emu_GUI_DrawLine();
extern void emu_GUI_Widget_DrawBorder();
extern void emu_Unknown_07AE_0000();
extern void overlay(uint16 cs, uint8 force);

/**
 * Draw a wired rectangle.
 * @param left The left position of the rectangle.
 * @param top The top position of the rectangle.
 * @param right The right position of the rectangle.
 * @param bottom The bottom position of the rectangle.
 * @param colour The colour of the rectangle.
 */
void GUI_DrawWiredRectangle(uint16 left, uint16 top, uint16 right, uint16 bottom, uint16 colour)
{
	emu_push(colour);
	emu_push(top);
	emu_push(right);
	emu_push(top);
	emu_push(left);
	emu_push(emu_cs); emu_push(0x001E); emu_cs = 0x22A6; emu_GUI_DrawLine();
	emu_sp += 10;

	emu_push(colour);
	emu_push(bottom);
	emu_push(right);
	emu_push(bottom);
	emu_push(left);
	emu_push(emu_cs); emu_push(0x0038); emu_cs = 0x22A6; emu_GUI_DrawLine();
	emu_sp += 10;

	emu_push(colour);
	emu_push(bottom);
	emu_push(left);
	emu_push(top);
	emu_push(left);
	emu_push(emu_cs); emu_push(0x004E); emu_cs = 0x22A6; emu_GUI_DrawLine();
	emu_sp += 10;

	emu_push(colour);
	emu_push(bottom);
	emu_push(right);
	emu_push(top);
	emu_push(right);
	emu_push(emu_cs); emu_push(0x0068); emu_cs = 0x22A6; emu_GUI_DrawLine();
	emu_sp += 10;
}

/**
 * Display a text.
 * @param str The text to display.
 * @param arg0A ??.
 * @param ... The args for the text.
 */
void GUI_DisplayText(const char *str, uint16 arg0A, ...)
{
	char buffer[80];
	buffer[0] = '\0';

	if (str != NULL) {
		va_list ap;

		va_start(ap, arg0A);
		vsnprintf(buffer, sizeof(buffer), str, ap);
		va_end(ap);
	}

	if (arg0A == 0xFFFF) {
		g_global->variable_3734 = 0xFFFF;
		g_global->variable_3736 = 0xFFFF;
		g_global->variable_3738 = 0xFFFF;

		g_global->variable_3644[0] = '\0';
		g_global->variable_3694[0] = '\0';
		g_global->variable_36E4[0] = '\0';

		g_global->variable_373A = 0;
		g_global->variable_373C = 0;
		return;
	}

	if (arg0A == 0xFFFE) {
		if (g_global->variable_373A == 0) {
			g_global->variable_3736 = 0xFFFF;
			g_global->variable_3694[0] = '\0';
		}
		g_global->variable_3738 = 0xFFFF;
		g_global->variable_36E4[0] = '\0';
	}

	if (g_global->variable_373A != 0) {
		uint16 loc04;
		uint16 loc06;

		if (buffer[0] != '\0') {
			if (strcasecmp(buffer, g_global->variable_3694) != 0 && (int16)arg0A >= (int16)g_global->variable_3738) {
				strcpy(g_global->variable_36E4, buffer);
				g_global->variable_3738 = arg0A;
			}
		}
		if ((int32)g_global->variable_373C > (int32)g_global->variable_76AC) return;

		emu_push(7);
		emu_push(emu_cs); emu_push(0x0AA4); emu_cs = 0x07AE; emu_Unknown_07AE_0000();
		emu_sp += 2;

		loc04 = emu_ax;

		if (g_global->variable_38C4 != 0) {
			uint16 loc02;

			emu_push(2);
			emu_push(emu_cs); emu_push(0x0ABB); emu_cs = 0x2598; f__2598_0000_0017_EB80();
			emu_sp += 2;

			loc02 = emu_ax;

			assert(g_global->variable_6668.csip == 0x22A60D31);

			emu_push(g_global->variable_6D59);
			emu_push(23);
			emu_push(319);
			emu_push(0);
			emu_push(0);
			emu_push(emu_cs); emu_push(0x0ADB); emu_cs = 0x22A6; emu_GUI_DrawFilledRectangle();
			emu_sp += 10;

			GUI_DrawText_Wrapper(
				g_global->variable_3694,
				g_global->variable_992D << 3,
				2,
				g_global->variable_8ADA & 0xFF,
				0,
				0x012
			);

			GUI_DrawText_Wrapper(
				g_global->variable_3644,
				g_global->variable_992D << 3,
				13,
				g_global->variable_8AD8 & 0xFF,
				0,
				0x012
			);

			g_global->variable_38C4 = 0;

			emu_push(loc02);
			emu_push(emu_cs); emu_push(0x0B34); emu_cs = 0x2598; f__2598_0000_0017_EB80();
			emu_sp += 2;
		}

		emu_push(7);
		emu_push(emu_cs); emu_push(0x0B3E); emu_cs = 0x2642; f__2642_0002_005E_87F6();
		emu_sp += 2;

		if (g_global->variable_3740 + g_global->variable_9931 > 24) {
			loc06 = 24 - g_global->variable_3740;
		} else {
			loc06 = g_global->variable_9931;
		}

		emu_push(0);
		emu_push(2);
		emu_push(loc06);
		emu_push(g_global->variable_992F);
		emu_push(g_global->variable_992B);
		emu_push(g_global->variable_992D);
		emu_push(g_global->variable_3740);
		emu_push(g_global->variable_992D);
		emu_push(emu_cs); emu_push(0x0B7D); emu_cs = 0x24D0; f__24D0_000D_0039_C17D();
		emu_sp += 16;

		emu_push(emu_cs); emu_push(0x0B85); emu_cs = 0x2642; f__2642_0069_0008_D517();

		emu_push(loc04);
		emu_push(emu_cs); emu_push(0x0B8D); emu_cs = 0x07AE; emu_Unknown_07AE_0000();
		emu_sp += 2;

		if (g_global->variable_3740 != 0) {
			if ((int16)g_global->variable_3738 <= (int16)g_global->variable_3736) {
				g_global->variable_373C = g_global->variable_76AC + 1;
			}
			g_global->variable_3740--;
			return;
		}

		strcpy(g_global->variable_3644, g_global->variable_3694);

		g_global->variable_8AD8 = g_global->variable_8ADA;
		g_global->variable_3734 = (g_global->variable_3736 != 0) ? g_global->variable_3736 - 1 : 0;

		strcpy(g_global->variable_3694, g_global->variable_36E4);

		g_global->variable_3736 = g_global->variable_3738;
		g_global->variable_8ADA = g_global->variable_8ADC;
		g_global->variable_36E4[0] = '\0';
		g_global->variable_3738 = 0xFFFF;
		g_global->variable_38C4 = 1;
		g_global->variable_373C = g_global->variable_76AC + ((int16)g_global->variable_3736 <= (int16)g_global->variable_3734 ? 900 : 1);
		g_global->variable_373A = 0;
		return;
	}

	if (buffer[0] != '\0') {
		if (strcasecmp(buffer, g_global->variable_3644) != 0 && strcasecmp(buffer, g_global->variable_3694) != 0 && strcasecmp(buffer, g_global->variable_36E4) != 0) {
			if ((int16)arg0A >= (int16)g_global->variable_3736) {
				strcpy(g_global->variable_36E4, g_global->variable_3694);

				g_global->variable_8ADC = g_global->variable_8ADA;
				g_global->variable_3738 = g_global->variable_3736;

				strcpy(g_global->variable_3694, buffer);

				g_global->variable_8ADA = 12;
				g_global->variable_3736 = arg0A;
			} else if ((int16)arg0A >= (int16)g_global->variable_3738) {
				strcpy(g_global->variable_36E4, buffer);
				g_global->variable_3738 = arg0A;
				g_global->variable_8ADC = 12;
			}
		}
	} else {
		if (g_global->variable_3644[0] == '\0' && g_global->variable_3694[0] == '\0') return;
	}

	if ((int16)g_global->variable_3736 <= (int16)g_global->variable_3734 && (int32)g_global->variable_373C >= (int32)g_global->variable_76AC) return;

	g_global->variable_373A = 1;
	g_global->variable_3740 = 10;
	g_global->variable_373C = 0;
}

/**
 * Draw a string to the screen.
 *
 * @param string The string to draw.
 * @param left The most left position where to draw the string.
 * @param top The most top position where to draw the string.
 * @param fgColour The foreground colour of the text.
 * @param bgColour The background colour of the text.
 */
void GUI_DrawText(char *string, int16 left, int16 top, uint8 fgColour, uint8 bgColour)
{
	uint8 *data;
	uint16 height;
	uint16 heightOffset;
	uint16 widthOffset;
	uint16 x;
	uint16 y;
	char *s;

	data = (uint8 *)emu_get_memorycsip(g_global->variable_99F3);
	if (data == NULL) return;

	heightOffset = ((uint16 *)data)[2];
	widthOffset  = ((uint16 *)data)[4];
	height = data[heightOffset + 4];

	if (left < 0) left = 0;
	if (top  < 0) top  = 0;
	if (left > 320) return;
	if (top  > 200) return;

	/* XXX -- This of course is not something that should stay in here very long */
	emu_get_memory8(0x2BC2, 0x00, 0x9) = fgColour;
	emu_get_memory8(0x2BC2, 0x00, 0x8) = bgColour;

	emu_push(1);
	emu_push(0);
	emu_push(0x2BC2); emu_push(0x8);
	emu_push(emu_cs); emu_push(0x00DE); emu_cs = 0x22A6; f__22A6_1102_004C_B069();
	emu_sp += 8;

	s = string;
	x = left;
	y = top;
	while (*s != '\0') {
		uint16 width;

		if (*s == '\n' || *s == '\r') {
			x = left;
			y += height + g_global->variable_6C6E;

			while (*s == '\n' || *s == '\r') s++;
		}

		width = data[widthOffset + *s] + g_global->variable_6C6C;

		if (x + width > 320) {
			x = left;
			y += height + g_global->variable_6C6E;
		}
		if (y > 200) break;

		emu_push(y); emu_push(x);
		emu_push(*s);
		emu_push(emu_cs); emu_push(0x00DE); emu_cs = 0x22A6; emu_GUI_DrawChar();
		emu_sp += 6;

		x += width;
		s++;
	}
}

/**
 * Draw a string to the screen, and so some magic.
 *
 * @param string The string to draw.
 * @param left The most left position where to draw the string.
 * @param top The most top position where to draw the string.
 * @param fgColour The foreground colour of the text.
 * @param bgColour The background colour of the text.
 * @param flags The flags of the string.
 */
void GUI_DrawText_Wrapper(char *string, int16 left, int16 top, uint8 fgColour, uint8 bgColour, uint16 flags, ...)
{
	uint8 arg12low = flags & 0xF;
	uint8 arg2mid  = flags & 0xF0;

	if ((arg12low != g_global->variable_376C && arg12low != 0) || string == NULL) {
		csip32 loc04;

		switch (arg12low) {
			case 1:
				loc04 = g_global->new6pFnt;
				break;

			case 2:
				loc04 = g_global->new8pFnt2;
				break;

			default:
				loc04 = g_global->new8pFnt;
				break;
		}

		emu_push(loc04.s.cs); emu_push(loc04.s.ip);
		emu_push(emu_cs); emu_push(0x1F56); emu_cs = 0x2605; f__2605_000C_006D_F8B2();
		emu_sp += 4;

		g_global->variable_376C = arg12low;
	}

	if ((arg2mid != g_global->variable_376E && arg2mid != 0) || string == NULL) {
		memset(g_global->variable_8ADE, 0, 16);

		switch (arg2mid) {
			case 0x0010:
				g_global->variable_8ADE[2] = 0;
				g_global->variable_8ADE[3] = 0;
				g_global->variable_6C6C = -2;
				break;

			case 0x0020:
				g_global->variable_8ADE[2] = 12;
				g_global->variable_8ADE[3] = 0;
				g_global->variable_6C6C = -1;
				break;

			case 0x0030:
				g_global->variable_8ADE[2] = 12;
				g_global->variable_8ADE[3] = 12;
				g_global->variable_6C6C = -1;
				break;

			case 0x0040:
				g_global->variable_8ADE[2] = 232;
				g_global->variable_8ADE[3] = 0;
				g_global->variable_6C6C = -1;
				break;
		}

		g_global->variable_8ADE[0] = bgColour;
		g_global->variable_8ADE[1] = fgColour;
		g_global->variable_8ADE[4] = 6;

		emu_push(0x353F); emu_push(0x8ADE);
		emu_push(emu_cs); emu_push(0x2006); emu_cs = 0x259E; f__259E_0021_001A_E253();
		emu_sp += 4;

		g_global->variable_376E = arg2mid;
	}

	if (string == NULL) return;

	if (string != g_global->variable_8AEE) {
		char buf[256];
		va_list ap;

		strncpy(buf, string, sizeof(buf));

		va_start(ap, flags);
		vsnprintf(g_global->variable_8AEE, sizeof(g_global->variable_8AEE), buf, ap);
		va_end(ap);
	}

	switch (flags & 0x0F00) {
		case 0x100:
			left -= Font_GetStringWidth(g_global->variable_8AEE) / 2;
			break;

		case 0x200:
			left -= Font_GetStringWidth(g_global->variable_8AEE);
			break;
	}

	GUI_DrawText(g_global->variable_8AEE, left, top, fgColour, bgColour);
}

/**
 * Do something on the given color in the given palette.
 *
 * @param palette The palette to work on.
 * @param color The color to modify.
 * @param reference The color to use as reference.
 */
static bool GUI_Palette_2BA5_00A2(uint8 *palette, uint16 color, uint16 reference)
{
	bool ret = false;
	uint16 i;

	color *= 3;
	reference *= 3;

	for (i = 0; i < 3; i++) {
		if (palette[reference] != palette[color]) {
			ret = true;
			palette[color] += (palette[color] > palette[reference]) ? -1 : 1;
		}
		color++;
		reference++;
	}

	return ret;
}

/**
 * Animate the palette. Only works for some colours or something
 */
void GUI_PaletteAnimate()
{
	uint8 *palette = emu_get_memorycsip(g_global->variable_3C32);

	if (g_global->variable_31CE < g_global->variable_76AC) {
		uint16 color;
		if (g_global->variable_37B2 != 0) {
			color = 15;
		} else {
			color = (g_global->variable_31D2 == 0) ? 15 : 6;
		}

		memcpy(palette + 3 * 239, palette + 3 * color, 3);

		emu_push(g_global->variable_3C32.s.cs); emu_push(g_global->variable_3C32.s.ip);
		emu_push(emu_cs); emu_push(0x05C7); emu_cs = 0x259E; f__259E_0040_0015_5E4A();
		emu_sp += 4;

		g_global->variable_31D2 = (g_global->variable_31D2 == 0) ? 1 : 0;
		g_global->variable_31CE = g_global->variable_76AC + 60;
	}

	if (g_global->variable_31CA < g_global->variable_76AC && g_global->selectionType != 0) {
		GUI_Palette_2BA5_00A2(palette, 255, g_global->variable_31D4);
		GUI_Palette_2BA5_00A2(palette, 255, g_global->variable_31D4);
		GUI_Palette_2BA5_00A2(palette, 255, g_global->variable_31D4);

		if (!GUI_Palette_2BA5_00A2(palette, 255, g_global->variable_31D4)) {
			if (g_global->variable_31D4 == 13) {
				g_global->variable_31D4 = 15;

				if (g_global->selectionType == 2) {
					if (g_global->variable_38EC != 0) {
						g_global->variable_31D4 = ((g_global->variable_38EC & 0x8000) != 0) ? 5 : 15;
					} else {
						g_global->variable_31D4 = 6;
					}
				}
			} else {
				g_global->variable_31D4 = 13;
			}
		}

		emu_push(g_global->variable_3C32.s.cs); emu_push(g_global->variable_3C32.s.ip);
		emu_push(emu_cs); emu_push(0x06BE); emu_cs = 0x259E; f__259E_0040_0015_5E4A();
		emu_sp += 4;

		g_global->variable_31CA = g_global->variable_76AC + 3;
	}

	if (g_global->variable_31C6 < g_global->variable_76AC) {
		GUI_Palette_2BA5_00A2(palette, 223, g_global->variable_31D6);

		if (!GUI_Palette_2BA5_00A2(palette, 223, g_global->variable_31D6)) {
			if (g_global->variable_31D6 == 12) {
				g_global->variable_31D6 = 10;
			} else {
				g_global->variable_31D6 = 12;
			}
		}

		emu_push(g_global->variable_3C32.s.cs); emu_push(g_global->variable_3C32.s.ip);
		emu_push(emu_cs); emu_push(0x073F); emu_cs = 0x259E; f__259E_0040_0015_5E4A();
		emu_sp += 4;

		g_global->variable_31C6 = g_global->variable_76AC + 5;
	}

	emu_push(emu_cs); emu_push(0x075A); emu_cs = 0x3483; overlay(0x3483, 0); emu_Unknown_B483_0470();
}

/**
 * Sets productionStringID to the correct string for the active structure.
 */
void GUI_UpdateProductionStringID()
{
	Structure *s = NULL;

	s = Structure_Get_ByPackedTile(g_global->selectionPosition);

	g_global->productionStringID = 0;

	if (s == NULL) return;

	if (g_structureInfo[s->o.type].flags.s.factory) {
		if (s->o.flags.s.upgrading) {
			g_global->productionStringID = 0x90; /* "Upgrading|%d%% done" */
		} else {
			if (s->o.linkedID != 0xFF) {
				if (s->o.flags.s.onHold) {
					g_global->productionStringID = 0x28; /* "On hold" */
				} else {
					if (s->countDown != 0) {
						g_global->productionStringID = 0x2E; /* "%d%% done" */
					} else {
						if (s->o.type == STRUCTURE_CONSTRUCTION_YARD) {
							g_global->productionStringID = 0x26; /* "Place it" */
						} else {
							g_global->productionStringID = 0x27; /* "Completed" */
						}
					}
				}
			} else {
				g_global->productionStringID = 0x29; /* "Build it" */
			}
		}
	} else {
		if (s->o.type == STRUCTURE_PALACE) g_global->productionStringID = g_houseInfo[s->o.houseID].specialWeapon + 0x29;
	}
}

/**
 * Displays a message and waits for a user action.
 * @param str The text to display.
 * @param spriteID The sprite to draw (0xFFFF for none).
 * @param ... The args for the text.
 * @return ??
 */
uint16 GUI_DisplayModalMessage(char *str, uint16 spriteID, ...)
{
	va_list ap;
	uint16 oldValue_07AE_0000;
	uint16 ret;
	uint16 oldValue_2598_0000;
	uint16 size = 0;

	va_start(ap, spriteID);
	vsnprintf(g_global->variable_87D8, sizeof(g_global->variable_87D8), str, ap);
	va_end(ap);

	emu_push(emu_cs); emu_push(0x02A4); emu_cs = 0x2B6C; f__2B6C_0137_0020_C73F();

	emu_push(0);
	emu_push(emu_cs); emu_push(0x02AC); emu_cs = 0x2598; f__2598_0000_0017_EB80();
	emu_sp += 2;
	oldValue_2598_0000 = emu_ax;

	GUI_DrawText_Wrapper(NULL, 0, 0, 0, 0, 0x22);

	emu_push(1);
	emu_push(emu_cs); emu_push(0x02D7); emu_cs = 0x07AE; emu_Unknown_07AE_0000();
	emu_sp += 2;
	oldValue_07AE_0000 = emu_ax;

	g_global->variable_4062[1][3] = g_global->variable_6C71 * max(GUI_SplitText(g_global->variable_87D8, ((g_global->variable_992F - ((spriteID == 0xFFFF) ? 2 : 7)) << 3) - 6, '\r'), 3) + 18;

	emu_push(1);
	emu_push(emu_cs); emu_push(0x032C); emu_cs = 0x07AE; emu_Unknown_07AE_0000();
	emu_sp += 2;

	if (g_global->variable_3600.csip == 0x0) {
		emu_push(g_global->variable_9931);
		emu_push(g_global->variable_992F);
		emu_push(emu_cs); emu_push(0x0348); emu_cs = 0x22A6; f__22A6_127B_0036_F8C9();
		emu_sp += 4;

		size = emu_ax;

		emu_push(emu_cs); emu_push(0x0351); emu_cs = 0x23E1; f__23E1_0334_000B_CF65();

		if (((int32)((emu_dx << 16) | emu_ax)) >= size) {
			emu_push(0);
			emu_push(0); emu_push(size);
			emu_push(emu_cs); emu_push(0x0367); emu_cs = 0x23E1; emu_Tools_Malloc();
			emu_sp += 6;

			g_global->variable_3600.s.cs = emu_dx;
			g_global->variable_3600.s.ip = emu_ax;
		} else {
			size = 0;
		}
	}

	if (g_global->variable_3600.csip != 0x0) {
		emu_push(g_global->variable_3600.s.cs); emu_push(g_global->variable_3600.s.ip);
		emu_push(g_global->variable_9931);
		emu_push(g_global->variable_992F);
		emu_push(g_global->variable_992B);
		emu_push(g_global->variable_992D);
		emu_push(emu_cs); emu_push(0x03A0); emu_cs = 0x22A6; emu_GUI_CopyToBuffer();
		emu_sp += 12;
	}

	emu_push(1);
	emu_push(1);
	emu_push(1);
	emu_push(emu_cs); emu_push(0x03B4); emu_GUI_Widget_DrawBorder();
	emu_sp += 6;

	if (spriteID != 0xFFFF) {
		GUI_DrawSprite(g_global->variable_6C91, g_sprites[spriteID], 7, 8, 1, 0x4000);

		emu_push(g_global->variable_9931 - 16);
		emu_push(g_global->variable_992F - 7);
		emu_push(g_global->variable_992B + 8);
		emu_push(g_global->variable_992D + 5);
		emu_push(1);
		emu_push(emu_cs); emu_push(0x042F); emu_cs = 0x2599; f__2599_000B_0047_21FD();
		emu_sp += 10;
	} else {
		emu_push(g_global->variable_9931 - 16);
		emu_push(g_global->variable_992F - 2);
		emu_push(g_global->variable_992B + 8);
		emu_push(g_global->variable_992D + 1);
		emu_push(1);
		emu_push(emu_cs); emu_push(0x042F); emu_cs = 0x2599; f__2599_000B_0047_21FD();
		emu_sp += 10;
	}

	g_global->variable_6D59 = 0;
	g_global->variable_9933 = 0;
	g_global->variable_9935 = 0;

	GUI_DrawText(g_global->variable_87D8, g_global->variable_992D << 3, g_global->variable_992B, g_global->variable_6D5B & 0xFF, g_global->variable_6D59 & 0xFF);

	emu_push(g_global->variable_3C32.s.cs); emu_push(g_global->variable_3C32.s.ip);
	emu_push(emu_cs); emu_push(0x046E); emu_cs = 0x259E; f__259E_0040_0015_5E4A();
	emu_sp += 4;

	emu_push(emu_cs); emu_push(0x0475); emu_cs = 0x2B6C; f__2B6C_0169_001E_6939();

	g_global->variable_76B4 = 30;

	while (g_global->variable_76B4 != 0) GUI_PaletteAnimate();

	emu_push(emu_cs); emu_push(0x0496); emu_cs = 0x29E8; emu_Input_History_Clear();

	do {
		GUI_PaletteAnimate();

		emu_push(emu_cs); emu_push(0x04A0); emu_cs = 0x29E8; f__29E8_07FA_0020_177A();
		ret = emu_ax;
	} while (ret == 0 || (ret & 0x800) != 0);

	emu_push(0x841);
	emu_push(emu_cs); emu_push(0x04B9); emu_cs = 0x29E8; emu_Input_HandleInput();
	emu_sp += 2;

	emu_push(emu_cs); emu_push(0x04BF); emu_cs = 0x2B6C; f__2B6C_0137_0020_C73F();

	if (spriteID != 0xFFFF) {
		emu_push(g_global->variable_9931 + 16);
		emu_push(g_global->variable_992F + 7);
		emu_push(g_global->variable_992B - 8);
		emu_push(g_global->variable_992D - 5);
		emu_push(1);
		emu_push(emu_cs); emu_push(0x0505); emu_cs = 0x2599; f__2599_000B_0047_21FD();
		emu_sp += 10;
	} else {
		emu_push(g_global->variable_9931 + 16);
		emu_push(g_global->variable_992F + 2);
		emu_push(g_global->variable_992B - 8);
		emu_push(g_global->variable_992D - 1);
		emu_push(1);
		emu_push(emu_cs); emu_push(0x0505); emu_cs = 0x2599; f__2599_000B_0047_21FD();
		emu_sp += 10;
	}

	if (g_global->variable_3600.csip != 0x0) {
		emu_push(g_global->variable_3600.s.cs); emu_push(g_global->variable_3600.s.ip);
		emu_push(g_global->variable_9931);
		emu_push(g_global->variable_992F);
		emu_push(g_global->variable_992B);
		emu_push(g_global->variable_992D);
		emu_push(emu_cs); emu_push(0x0533); emu_cs = 0x22A6; emu_GUI_CopyFromBuffer();
		emu_sp += 12;
	}

	emu_push(oldValue_07AE_0000);
	emu_push(emu_cs); emu_push(0x053E); emu_cs = 0x07AE; emu_Unknown_07AE_0000();
	emu_sp += 2;

	if (size != 0) {
		emu_push(g_global->variable_3600.s.cs); emu_push(g_global->variable_3600.s.ip);
		emu_push(emu_cs); emu_push(0x0550); emu_cs = 0x23E1; emu_Tools_Free();
		emu_sp += 4;

		g_global->variable_3600.csip = 0;
	} else {
		g_global->variable_3A12 = 1;
	}

	emu_push(oldValue_2598_0000);
	emu_push(emu_cs); emu_push(0x056E); emu_cs = 0x2598; f__2598_0000_0017_EB80();
	emu_sp += 2;

	emu_push(emu_cs); emu_push(0x0574); emu_cs = 0x2B6C; f__2B6C_0169_001E_6939();

	return ret;
}

/**
 * Splits the given text in lines of maxwidth width using the given delimiter.
 * @param str The text to split.
 * @param maxwidth The maximum width the text will have.
 * @param delimiter The char used as delimiter.
 * @return The number of lines.
 */
uint16 GUI_SplitText(char *str, uint16 maxwidth, char delimiter)
{
	uint16 lines = 0;

	if (str == NULL) return 0;

	while (*str != '\0') {
		uint16 width = 0;

		lines++;

		while (width < maxwidth && *str != delimiter && *str != '\r' && *str != '\0') width += Font_GetCharWidth(*str++);

		if (width >= maxwidth) {
			while (*str != 0x20 && *str != delimiter && *str != '\r' && *str != '\0') width -= Font_GetCharWidth(*str--);
		}

		if (*str != '\0') *str++ = delimiter;
	}

	return lines;
}

/**
 * Draws a sprite.
 * @param memory The index of the memory block where the drawing is done.
 * @param sprite_csip The CS:IP of the sprite to draw.
 * @param posX ??.
 * @param posY ??.
 * @param windowID The ID of the window where the drawing is done.
 * @param flags The flags.
 * @param ... The extra args, flags dependant.
 */
void GUI_DrawSprite(uint16 memory, csip32 sprite_csip, int16 posX, int16 posY, uint16 windowID, uint16 flags, ...)
{
	static uint16 s_variable_0E[8]  = {0x050E, 0x0545, 0x050E, 0x0545, 0x07A7, 0x0857, 0x07A7, 0x0857};
	static uint16 s_variable_1E[8]  = {0x050E, 0x0545, 0x050E, 0x0545, 0x07ED, 0x089D, 0x07ED, 0x089D};
	static uint16 s_variable_2E[8]  = {0x0530, 0x0569, 0x0530, 0x0569, 0x0812, 0x08C5, 0x0812, 0x08C5};
	static uint16 s_variable_3E[16] = {0x0580, 0x0584, 0x0599, 0x05C8, 0x05E0, 0x061F, 0x05EA, 0x05C8, 0x0634, 0x0653, 0x0683, 0x06C6, 0x06F9, 0x0777, 0x071E, 0x06C6};
	static uint16 s_variable_5E     = 0;
	static uint16 s_variable_60[8]  = {1, 3, 2, 5, 4, 3, 2, 1};
	static uint16 s_variable_70     = 1;
	static uint16 s_variable_72     = 0x8B55;
	static uint16 s_variable_74     = 0x51EC;

	va_list ap;

	int16  top;
	int16  bottom;
	uint16 width;
	uint16 loc10;
	int16  loc12;
	int16  loc14;
	int16  loc16;
	uint8  loc18;
	int16  loc1A;
	int16  loc1C;
	int16  loc1E;
	int16  loc20;
	uint16 loc22;
	uint8 *loc26 = NULL;
	int16  loc28 = 0;
	int16  loc2A;
	uint16 loc30 = 0;
	uint16 loc32;
	uint16 loc34;
	uint8 *loc38 = NULL;
	int16  loc3A;
	uint8 *loc3E = NULL;
	uint16 loc42;
	uint16 loc44;
	uint16 locbx;

	csip32 memBlock;
	uint8 *buf = NULL;
	uint8 *sprite = NULL;
	uint8 *b = NULL;
	int16  count;

	if (sprite_csip.csip == 0x0) return;

	sprite = emu_get_memorycsip(sprite_csip);

	if ((*sprite & 0x1) != 0) flags |= 0x400;

	va_start(ap, flags);

	if ((flags & 0x2000) != 0) loc3E = va_arg(ap, uint8*);

	if ((flags & 0x100) != 0) {
		loc26 = va_arg(ap, uint8*);
		loc28 = (int16)va_arg(ap, int);
		if (loc28 == 0) flags &= 0xFEFF;
	}

	if ((flags & 0x200) != 0) {
		s_variable_5E = (s_variable_5E + 1) % 8;
		s_variable_70 = s_variable_60[s_variable_5E];
		s_variable_74 = 0x0;
		s_variable_72 = 0x100;
	}

	if ((flags & 0x1000) != 0) s_variable_72 = (uint16)va_arg(ap, int);

	if ((flags & 0x800) != 0) {
		loc18 = (uint8)va_arg(ap, int);
	}

	if ((flags & 0x4) != 0) {
		loc30 = (uint16)va_arg(ap, int);
		loc32 = (uint16)va_arg(ap, int);
	} else {
		loc32 = 0x100;
	}

	va_end(ap);

	loc34 = 0;

	emu_push(emu_ax);
	emu_push(emu_dx);
	emu_push(memory);
	emu_push(emu_cs); emu_push(0x0263); emu_cs = 0x252E; emu_Memory_GetBlock2();
	emu_sp += 2;
	memBlock.s.cs = emu_dx;
	memBlock.s.ip = g_global->variable_4062[windowID][0] << 3;
	emu_pop(&emu_dx);
	emu_pop(&emu_ax);

	buf = emu_get_memorycsip(memBlock);

	if ((flags & 0x4000) == 0) posX -= g_global->variable_4062[windowID][0] << 3;

	width = g_global->variable_4062[windowID][2] << 3;
	top = g_global->variable_4062[windowID][1];

	if ((flags & 0x4000) != 0) posY += g_global->variable_4062[windowID][1];

	bottom = g_global->variable_4062[windowID][1] + g_global->variable_4062[windowID][3];

	loc10 = *(uint16 *)sprite;
	sprite += 2;

	loc12 = *sprite++;

	if ((flags & 0x4) != 0) {
		loc12 *= loc32;
		loc12 >>= 8;
		if (loc12 == 0) return;
	}

	if ((flags & 0x8000) != 0) posY -= loc12 / 2;

	loc1A = *(uint16 *)sprite;
	sprite += 2;

	loc14 = loc1A;

	if ((flags & 0x4) != 0) {
		loc14 += loc30;
		loc14 >>= 8;
		if (loc14 == 0) return;
	}

	if ((flags & 0x8000) != 0) posX -= loc14 / 2;

	loc16 = loc14;

	sprite += 3;

	locbx = *(uint16 *)sprite;
	sprite += 2;

	if ((loc10 & 0x1) != 0 && (flags & 0x2000) == 0) loc3E = sprite;

	if ((flags & 0x400) != 0) {
		sprite += 16;
	}

	if ((loc10 & 0x2) == 0) {
		Format80_Decode(emu_get_memorycsip(g_global->variable_6F18), sprite, locbx);

		sprite = emu_get_memorycsip(g_global->variable_6F18);
	}

	if ((flags & 0x2) == 0) {
		loc2A = posY - top;
	} else {
		loc2A = bottom - posY - loc12;
	}

	if (loc2A < 0) {
		loc12 += loc2A;
		if (loc12 <= 0) return;

		loc2A = -loc2A;

		while (loc2A > 0) {
			loc42 = 0;
			loc38 = sprite;
			count = loc1A;
			loc1C = loc1A;

			/* Call based on memory/register values */
			switch (s_variable_1E[flags & 0xFF]) {
				case 0x050E:
				case 0x0545:
					if (count == 0) break;

					while (count > 0) {
						while (count != 0) {
							count--;
							if (*sprite++ == 0) break;
						}
						if (sprite[-1] != 0 && count == 0) break;

						count -= *sprite++ - 1;
					}

					buf += count * ((s_variable_1E[flags & 0xFF] == 0x050E) ? -1 : 1);
					break;

				default:
					/* In case we don't know the call point yet, call the dynamic call */
					emu_last_cs = 0x2903; emu_last_ip = 0x0384; emu_last_length = 0x0016; emu_last_crc = 0x7D40;
					emu_call();
					return;
			}

			loc34 += loc32;
			if ((loc34 & 0xFF00) == 0) continue;

			loc2A -= loc34 >> 8;
			loc34 &= 0xFF;
		}

		if (loc2A < 0) {
			sprite = loc38;

			loc2A = -loc2A;
			loc34 += loc2A << 8;
		}

		if ((flags & 0x2) == 0) posY = top;
	}

	if ((flags & 0x2) == 0) {
		loc1E = bottom - posY;
	} else {
		loc1E = posY + loc12 - top;
	}

	if (loc1E <= 0) return;

	if (loc1E < loc12) {
		loc12 = loc1E;
		if ((flags & 0x2) != 0) posY = top;
	}

	loc1E = 0;
	if (posX < 0) {
		loc14 += posX;
		loc1E = -posX;
		if (loc1E >= loc16) return;
		posX = 0;
	}

	loc20 = 0;
	loc3A = width - posX;
	if (loc3A <= 0) return;

	if (loc3A < loc14) {
		loc14 = loc3A;
		loc20 = loc16 - loc1E - loc14;
	}

	loc3A = 320;
	loc22 = posY;

	if ((flags & 0x2) != 0) {
		loc3A = - loc3A;
		loc22 += loc12 - 1;
	}

	loc22 *= 2;
	loc22 = emu_get_memory16(g_global->variable_66EC.s.cs, g_global->variable_66EC.s.ip, loc22) + posX;
	buf += loc22;

	if ((flags & 0x1) != 0) {
		uint16 tmp = loc1E;
		loc1E = loc20;
		loc20 = tmp;
		buf += loc14 - 1;
	}

	b = buf;

	if ((flags & 0x4) != 0) {
		loc20 = 0;
		loc44 = loc1E;
		loc1E = (loc44 << 8) / loc30;
		loc42 = -((loc44 << 8) % loc30);
	}

	if ((loc34 & 0xFF00) == 0) {
	l__04A4:
		while (true) {
			loc34 += loc32;

			if ((loc34 & 0xFF00) != 0) break;
			count = loc1A;
			loc1C = loc1A;

			/* Call based on memory/register values */
			switch (s_variable_1E[flags & 0xFF]) {
				case 0x050E:
				case 0x0545:
					if (count == 0) break;

					while (count > 0) {
						while (count != 0) {
							count--;
							if (*sprite++ == 0) break;
						}
						if (sprite[-1] != 0 && count == 0) break;

						count -= *sprite++ - 1;
					}

					buf += count * ((s_variable_1E[flags & 0xFF] == 0x050E) ? -1 : 1);
					break;

				default:
					/* In case we don't know the call point yet, call the dynamic call */
					emu_last_cs = 0x2903; emu_last_ip = 0x04B9; emu_last_length = 0x0018; emu_last_crc = 0x7352;
					emu_call();
					return;
			}
		}
		loc38 = sprite;
	}

	while (true) {
		loc1C = loc1A;
		count = loc1E;

		/* Call based on memory/register values */
		switch (s_variable_0E[flags & 0xFF]) {
			case 0x050E:
			case 0x0545:
				if (count == 0) break;

				while (count > 0) {
					while (count != 0) {
						count--;
						if (*sprite++ == 0) break;
					}
					if (sprite[-1] != 0 && count == 0) break;

					count -= *sprite++ - 1;
				}

				buf += count * ((s_variable_0E[flags & 0xFF] == 0x050E) ? -1 : 1);
				break;

			default:
				/* In case we don't know the call point yet, call the dynamic call */
				emu_last_cs = 0x2903; emu_last_ip = 0x04D1; emu_last_length = 0x0010; emu_last_crc = 0x92CB;
				emu_call();
				return;
		}

		if (loc1C != 0) {
			count += loc14;
			if (count > 0) {
				uint8 v;

				/* Call based on memory/register values */
				switch (s_variable_2E[flags & 0xFF]) {
					case 0x0530:
					case 0x0569:
						while (count > 0) {
							v = *sprite++;
							if (v == 0) {
								buf += *sprite * ((s_variable_2E[flags & 0xFF] == 0x0530) ? 1 : -1);
								count -= *sprite++;
								continue;
							}

							/* Call based on memory/register values */
							switch (s_variable_3E[(flags >> 8) & 0xF]) {
								case 0x0580:
									*buf = v;
									break;

								case 0x0584: {
									int16 i;

									for(i = 0; i < loc28; i++) v = loc26[v];

									*buf = v;

									break;
								}

								case 0x0599:
									s_variable_74 += s_variable_72;

									if ((s_variable_74 & 0xFF00) == 0) {
										*buf = v;
									} else {
										s_variable_74 &= 0xFF;
										*buf = buf[s_variable_70];
									}
									break;

								case 0x05C8: {
									int16 i;

									v = *buf;

									for(i = 0; i < loc28; i++) v = loc26[v];

									*buf = v;

									break;
								}

								case 0x05E0:
									*buf = loc3E[v];
									break;

								case 0x05EA:
									s_variable_74 += s_variable_72;

									if ((s_variable_74 & 0xFF00) == 0) {
										*buf = loc3E[v];
									} else {
										s_variable_74 &= 0xFF;
										*buf = buf[s_variable_70];
									}
									break;

								case 0x061F: {
									int16 i;

									v = loc3E[v];

									for(i = 0; i < loc28; i++) v = loc26[v];

									*buf = v;

									break;
								}

								default:
									/* In case we don't know the call point yet, call the dynamic call */
									emu_last_cs = 0x2903; emu_last_ip = 0x0535; emu_last_length = 0x0008; emu_last_crc = 0xCF42;
									emu_call();
									return;
							}

							buf += ((s_variable_2E[flags & 0xFF] == 0x0530) ? 1 : -1);
							count--;
						}
						break;

					default:
						/* In case we don't know the call point yet, call the dynamic call */
						emu_last_cs = 0x2903; emu_last_ip = 0x04DF; emu_last_length = 0x000E; emu_last_crc = 0x82DD;
						emu_call();
						return;
				}
			}

			count += loc20;
			if (count != 0) {
				/* Call based on memory/register values */
				switch (s_variable_1E[flags & 0xFF]) {
					case 0x050E:
					case 0x0545:
						if (count == 0) break;

						while (count > 0) {
							while (count != 0) {
								count--;
								if (*sprite++ == 0) break;
							}
							if (sprite[-1] != 0 && count == 0) break;

							count -= *sprite++ - 1;
						}

						buf += count * ((s_variable_1E[flags & 0xFF] == 0x050E) ? -1 : 1);
						break;

					default:
						/* In case we don't know the call point yet, call the dynamic call */
						emu_last_cs = 0x2903; emu_last_ip = 0x04E7; emu_last_length = 0x0008; emu_last_crc = 0x5672;
						emu_call();
						return;
				}
			}
		}

		b += loc3A;
		buf = b;

		if (--loc12 == 0) return;

		loc34 -= 0x100;
		if ((loc34 & 0xFF00) == 0) goto l__04A4;
		sprite = loc38;
	}
}

/**
 * Updates the score.
 * @param score The base score.
 * @param harvestedAllied Pointer to the total amount of spice harvested by allies.
 * @param harvestedEnemy Pointer to the total amount of spice harvested by enemies.
 * @param houseID The houseID of the player.
 */
static uint16 Update_Score(int16 score, uint16 *harvestedAllied, uint16 *harvestedEnemy, uint16 houseID)
{
	PoolFindStruct find;
	uint16 locdi = 0;
	int16  loc0A;
	uint16 loc0C = 0;
	uint32 tmp;

	if (score < 0) score = 0;

	find.houseID = houseID;
	find.type    = 0xFFFF;
	find.index   = 0xFFFF;

	while (true) {
		Structure *s;

		s = Structure_Find(&find);
		if (s == NULL) break;

		score += g_structureInfo[s->o.type].variable_20 / 100;
	}

	g_global->variable_38BC++;

	find.houseID = 0xFFFF;
	find.type    = UNIT_HARVESTER;
	find.index   = 0xFFFF;

	while (true) {
		Unit *u;

		u = Unit_Find(&find);
		if (u == NULL) break;

		if (House_AreAllied(Unit_GetHouseID(u), (uint8)g_global->playerHouseID)) {
			locdi += u->amount * 7;
		} else {
			loc0C += u->amount * 7;
		}
	}

	g_global->variable_38BC--;

	tmp = *harvestedEnemy + loc0C;
	*harvestedEnemy = (tmp > 65000) ? 65000 : (tmp & 0xFFFF);

	tmp = *harvestedAllied + locdi;
	*harvestedAllied = (tmp > 65000) ? 65000 : (tmp & 0xFFFF);

	score += House_Get_ByIndex((uint8)houseID)->credits / 100;

	if (score < 0) score = 0;

	loc0A = g_global->campaignID * 45;

	if ((int16)g_global->variable_81EB < loc0A) {
		score += loc0A - g_global->variable_81EB;
	}

	return score;
}

/**
 * Draws a string on a filled rectangle.
 * @param string The string to draw.
 * @param top The most top position where to draw the string.
 */
static void GUI_DrawTextOnFilledRectangle(char *string, uint16 top)
{
	uint16 halfWidth;

	GUI_DrawText_Wrapper(NULL, 0, 0, 0, 0, 0x121);

	halfWidth = (Font_GetStringWidth(string) / 2) + 4;

	emu_push(116);
	emu_push(top + 6);
	emu_push(160 + halfWidth);
	emu_push(top);
	emu_push(160 - halfWidth);
	emu_push(emu_cs); emu_push(0x0E8C); emu_cs = 0x22A6; emu_GUI_DrawFilledRectangle();
	emu_sp += 10;

	GUI_DrawText_Wrapper(string, 160, top, 0xF, 0, 0x121);
}

/**
 * Shows the stats at end of scenario.
 * @param killedAllied The amount of destroyed allied units.
 * @param killedEnemy The amount of destroyed enemy units.
 * @param destroyedAllied The amount of destroyed allied structures.
 * @param destroyedEnemy The amount of destroyed enemy structures.
 * @param harvestedAllied The amount of spice harvested by allies.
 * @param harvestedEnemy The amount of spice harvested by enemies.
 * @param score The base score.
 * @param houseID The houseID of the player.
 */
void GUI_ShowEndStats(uint16 killedAllied, uint16 killedEnemy, uint16 destroyedAllied, uint16 destroyedEnemy, uint16 harvestedAllied, uint16 harvestedEnemy, int16 score, uint16 houseID)
{
	uint16 loc06;
	uint16 loc14;
	uint16 loc16;
	uint16 loc18;
	uint16 loc1A;
	uint16 loc32[3][2][2];
	uint16 i;

	g_global->variable_81EB = ((g_global->tickGlobal - g_global->tickScenarioStart) / 3600) + 1;

	score = Update_Score(score, &harvestedAllied, &harvestedEnemy, houseID);

	loc16 = (g_global->scenarioID == 1) ? 2 : 3;

	emu_push(emu_cs); emu_push(0x005B); emu_cs = 0x2B6C; f__2B6C_0137_0020_C73F();

	emu_push(0);
	emu_push(emu_cs); emu_push(0x0063); emu_cs = 0x34E9; overlay(0x34E9, 0); f__B4E9_0050_003F_292A();
	emu_sp += 2;

	emu_push(2);
	emu_push(emu_cs); emu_push(0x006D); emu_cs = 0x2598; f__2598_0000_0017_EB80();
	emu_sp += 2;
	loc14 = emu_ax;

	emu_push(0);
	emu_push(score);
	emu_push(emu_cs); emu_push(0x007C); emu_cs = 0x3518; overlay(0x3518, 0); f__B518_0B1D_0014_307D();
	emu_sp += 4;

	/* "Spice Harvested By" */
	GUI_DrawTextOnFilledRectangle(String_Get_ByIndex(0x1A), 83);

	/* "Units Destroyed By" */
	GUI_DrawTextOnFilledRectangle(String_Get_ByIndex(0x18), 119);

	if (g_global->scenarioID != 1) {
		/* "Buildings Destroyed By" */
		GUI_DrawTextOnFilledRectangle(String_Get_ByIndex(0x19), 155);
	}

	/* "You:", "Enemy:" */
	loc06 = max(Font_GetStringWidth(String_Get_ByIndex(0x149)), Font_GetStringWidth(String_Get_ByIndex(0x14A)));

	loc18 = loc06 + 19;
	loc1A = 261 - loc18;

	for (i = 0; i < loc16; i++) {
		/* "You:" */
		GUI_DrawText_Wrapper(String_Get_ByIndex(0x149), loc18 - 4,  92 + (i * 36), 0xF, 0, 0x221);

		/* "Enemy:" */
		GUI_DrawText_Wrapper(String_Get_ByIndex(0x14A), loc18 - 4, 101 + (i * 36), 0xF, 0, 0x221);
	}

	Sound_Play(17 + Tools_RandomRange(0, 5));

	emu_push(0);
	emu_push(2);
	emu_push(200);
	emu_push(40);
	emu_push(0);
	emu_push(0);
	emu_push(0);
	emu_push(0);
	emu_push(emu_cs); emu_push(0x01DA); emu_cs = 0x24D0; f__24D0_000D_0039_C17D();
	emu_sp += 16;

	emu_push(emu_cs); emu_push(0x01E2); emu_cs = 0x29E8; emu_Input_History_Clear();

	loc32[0][0][0] = harvestedAllied;
	loc32[0][1][0] = harvestedEnemy;
	loc32[1][0][0] = killedEnemy;
	loc32[1][1][0] = killedAllied;
	loc32[2][0][0] = destroyedEnemy;
	loc32[2][1][0] = destroyedAllied;

	for (i = 0; i < loc16; i++) {
		uint16 loc08 = loc32[i][0][0];
		uint16 loc0A = loc32[i][1][0];

		if (loc08 >= 65000) loc08 = 65000;
		if (loc0A >= 65000) loc0A = 65000;

		loc32[i][0][0] = loc08;
		loc32[i][1][0] = loc0A;

		loc06 = max(loc08, loc0A);

		loc32[i][0][1] = 1 + ((loc06 > loc1A) ? (loc06 / loc1A) : 0);
		loc32[i][1][1] = 1 + ((loc06 > loc1A) ? (loc06 / loc1A) : 0);
	}

	emu_push(45);
	emu_push(emu_cs); emu_push(0x02F6); emu_cs = 0x3518; overlay(0x3518, 0); f__B518_14D4_0013_5ED7();
	emu_sp += 2;

	emu_push(1);
	emu_push(score);
	emu_push(emu_cs); emu_push(0x0303); emu_cs = 0x3518; overlay(0x3518, 0); f__B518_0EB1_000E_D2F5();
	emu_sp += 4;

	emu_push(45);
	emu_push(emu_cs); emu_push(0x030E); emu_cs = 0x3518; overlay(0x3518, 0); f__B518_14D4_0013_5ED7();
	emu_sp += 2;

	for (i = 0; i < loc16; i++) {
		uint16 loc02;

		emu_push(emu_cs); emu_push(0x0319); emu_cs = 0x3518; overlay(0x3518, 0); f__B518_14F2_003E_977C();

		for (loc02 = 0; loc02 < 2; loc02++) {
			uint16 loc12;
			uint16 loc04;
			uint16 locdi;
			uint16 loc0E;
			uint16 loc10;
			uint16 loc0C;

			emu_push(emu_cs); emu_push(0x0326); emu_cs = 0x3518; overlay(0x3518, 0); f__B518_14F2_003E_977C();

			loc12 = (loc02 == 0) ? 255 : 209;
			loc04 = loc18;

			locdi = 93 + (i * 36) + (loc02 * 9);

			loc0E = loc32[i][loc02][0];
			loc10 = loc32[i][loc02][1];

			for (loc0C = 0; loc0C < loc0E; loc0C += loc10) {
				emu_push(226);
				emu_push(locdi + 5);
				emu_push(303);
				emu_push(locdi);
				emu_push(271);
				emu_push(emu_cs); emu_push(0x03A4); emu_cs = 0x22A6; emu_GUI_DrawFilledRectangle();
				emu_sp += 10;

				GUI_DrawText_Wrapper("%u", 287, locdi - 1, 0x14, 0, 0x121, loc0C);

				emu_push(emu_cs); emu_push(0x03CF); emu_cs = 0x3518; overlay(0x3518, 0); f__B518_14F2_003E_977C();

				g_global->variable_76B4 = 1;

				emu_push(loc12);
				emu_push(locdi + 5);
				emu_push(loc04);
				emu_push(locdi);
				emu_push(loc04);
				emu_push(emu_cs); emu_push(0x03F5); emu_cs = 0x22A6; emu_GUI_DrawLine();
				emu_sp += 10;

				loc04++;

				emu_push(0xC);
				emu_push(locdi + 6);
				emu_push(loc04);
				emu_push(locdi + 1);
				emu_push(loc04);
				emu_push(emu_cs); emu_push(0x0419); emu_cs = 0x22A6; emu_GUI_DrawLine();
				emu_sp += 10;

				emu_push(0);
				emu_push(2);
				emu_push(7);
				emu_push(304);
				emu_push(locdi);
				emu_push(loc18);
				emu_push(locdi);
				emu_push(loc18);
				emu_push(emu_cs); emu_push(0x043D); emu_cs = 0x22A6; f__22A6_034F_000C_5E0A();
				emu_sp += 16;

				Driver_Sound_Play(52, 0xFF);

				emu_push(g_global->variable_76B4);
				emu_push(emu_cs); emu_push(0x0453); emu_cs = 0x3518; overlay(0x3518, 0); f__B518_14D4_0013_5ED7();
				emu_sp += 2;
			}

			emu_push(226);
			emu_push(locdi + 5);
			emu_push(303);
			emu_push(locdi);
			emu_push(271);
			emu_push(emu_cs); emu_push(0x0485); emu_cs = 0x22A6; emu_GUI_DrawFilledRectangle();
			emu_sp += 10;

			GUI_DrawText_Wrapper("%u", 287, locdi - 1, 0xF, 0, 0x121, loc0E);

			emu_push(0);
			emu_push(2);
			emu_push(7);
			emu_push(304);
			emu_push(locdi);
			emu_push(loc18);
			emu_push(locdi);
			emu_push(loc18);
			emu_push(emu_cs); emu_push(0x04CC); emu_cs = 0x22A6; f__22A6_034F_000C_5E0A();
			emu_sp += 16;

			Driver_Sound_Play(38, 0xFF);

			emu_push(12);
			emu_push(emu_cs); emu_push(0x04E2); emu_cs = 0x3518; overlay(0x3518, 0); f__B518_14D4_0013_5ED7();
			emu_sp += 2;
		}

		emu_push(60);
		emu_push(emu_cs); emu_push(0x04F8); emu_cs = 0x3518; overlay(0x3518, 0); f__B518_14D4_0013_5ED7();
		emu_sp += 2;
	}

	emu_push(emu_cs); emu_push(0x0507); emu_cs = 0x2B6C; f__2B6C_0169_001E_6939();

	emu_push(emu_cs); emu_push(0x050C); emu_cs = 0x29E8; emu_Input_History_Clear();

	while (true) {
		emu_push(emu_cs); emu_push(0x0511); emu_cs = 0x3518; overlay(0x3518, 0); f__B518_14F2_003E_977C();

		emu_push(emu_cs); emu_push(0x0516); emu_cs = 0x29E8; emu_Input_Keyboard_NextKey();
		if (emu_ax != 0) break;
	}

	emu_push(emu_cs); emu_push(0x051F); emu_cs = 0x29E8; emu_Input_History_Clear();

	emu_push(score);
	emu_push(emu_cs); emu_push(0x0527); emu_cs = 0x3518; overlay(0x3518, 0); f__B518_0558_0010_240A();
	emu_sp += 2;

	memcpy(emu_get_memorycsip(g_global->variable_3C32) + 0x2FD, g_global->variable_81E8, 3);

	emu_push(loc14);
	emu_push(emu_cs); emu_push(0x054C); emu_cs = 0x2598; f__2598_0000_0017_EB80();
	emu_sp += 2;

	emu_push(emu_cs); emu_push(0x0552); emu_cs = 0x1DD7; f__1DD7_0B53_0025_36F7();
}

/**
 * Show pick house screen.
 */
uint16 GUI_PickHouse()
{
	uint16 oldValue_2598_0000;
	Widget *w = NULL;
	csip32 wcsip;
	uint8 *loc314; /* array of 768 bytes, probably a palette */
	uint16 i;
	uint16 ret;

	emu_push(emu_bp);
	emu_bp = emu_sp;
	emu_subw(&emu_sp, 0x314);

	ret = 0x5;
	wcsip.csip = 0x0;
	loc314 = &emu_get_memory8(emu_ss, emu_bp - 0x314, 0);

	memset(loc314, 0, 768);

	emu_push(0); emu_push(0);
	emu_push(emu_cs); emu_push(0x0FE6); emu_cs = 0x1DD7; f__1DD7_022D_0015_1956();
	emu_sp += 4;

	emu_push(5);
	emu_push(emu_cs); emu_push(0x0FF1); emu_cs = 0x3483; overlay(0x3483, 0); f__B483_04CB_0015_EBB4();
	emu_sp += 2;

	Sprites_Load(1, 7, g_sprites);

	while (true) {
		uint16 yes_no;

		for (i = 0; i < 3; i++) {
			Widget *w2;
			csip32 w2csip;

			w2 = GUI_Widget_Allocate(i + 1, g_global->variable_2BAC[i][2], g_global->variable_2BAC[i][0], g_global->variable_2BAC[i][1], 0xFFFF, 0, 0, &w2csip);

			w2->flags  = 0x11C0;
			w2->width  = 96;
			w2->height = 104;

			emu_push(w2csip.s.cs); emu_push(w2csip.s.ip);
			emu_push(wcsip.s.cs); emu_push(wcsip.s.ip);
			emu_push(emu_cs); emu_push(0x1097); emu_cs = 0x348B; overlay(0x348B, 0); f__B48B_0000_001E_7E97();
			emu_sp += 8;
			wcsip.s.cs = emu_dx;
			wcsip.s.ip = emu_ax;
			w = (Widget *)emu_get_memorycsip(wcsip);
		}

		emu_push(1);
		emu_push(0); emu_push(0);
		emu_push(3);
		emu_push(3);
		String_GenerateFilename("HERALD");
		emu_push(0x353F); emu_push(emu_Global_GetIP(g_global->stringFilename, 0x353F));
		emu_push(emu_cs); emu_push(0x10CE); emu_cs = 0x34CA; overlay(0x34CA, 0); emu_File_ReadChunkOrLengthFile();
		emu_sp += 14;

		emu_push(0); emu_push(0);
		emu_push(2);
		emu_push(3);
		emu_push(emu_cs); emu_push(0x10E9); emu_cs = 0x22A6; f__22A6_04A5_000F_3B8F();
		emu_sp += 8;

		emu_push(emu_cs); emu_push(0x10F1); emu_cs = 0x2B6C; f__2B6C_0137_0020_C73F();

		emu_push(0);
		emu_push(2);
		emu_push(200);
		emu_push(40);
		emu_push(0);
		emu_push(0);
		emu_push(0);
		emu_push(0);
		emu_push(emu_cs); emu_push(0x1111); emu_cs = 0x24D0; f__24D0_000D_0039_C17D();
		emu_sp += 16;

		emu_push(0xF);
		emu_push(g_global->variable_3C32.s.cs); emu_push(g_global->variable_3C32.s.ip);
		emu_push(emu_cs); emu_push(0x1125); emu_cs = 0x259E; f__259E_0006_0016_858A();
		emu_sp += 6;

		emu_push(emu_cs); emu_push(0x112D); emu_cs = 0x2B6C; f__2B6C_0169_001E_6939();

		ret = 0xFFFE;

		while (ret == 0xFFFE) {
			uint16 key = GUI_Widget_HandleEvents(w, wcsip);

			GUI_PaletteAnimate();

			if ((key & 0x800) != 0) key = 0;

			switch (key) {
				case 0x8001: ret = 1; break;
				case 0x8002: ret = 2; break;
				case 0x8003: ret = 0; break;
				default: break;
			}
		}

		emu_push(emu_cs); emu_push(0x1181); emu_cs = 0x2B6C; f__2B6C_0137_0020_C73F();

		if (ret == 0xFFFF) {
			emu_push(0xF);
			emu_push(emu_ss); emu_push(emu_bp - 0x314);
			emu_push(emu_cs); emu_push(0x1195); emu_cs = 0x259E; f__259E_0006_0016_858A();
			emu_sp += 6;
			break;
		}

		if (g_global->variable_6D8F != 0) {
			emu_push(ret + 62);
			emu_push(emu_cs); emu_push(0x11AD); emu_cs = 0x3483; overlay(0x3483, 0); emu_Unknown_B483_0363();
			emu_sp += 2;

			do {
				emu_push(emu_cs); emu_push(0x11B5); emu_cs = 0x3483; overlay(0x3483, 0); emu_Unknown_B483_0470();
			} while (emu_ax != 0);
		}

		while (w != NULL) {
			csip32 next = w->next;

			emu_push(wcsip.s.cs); emu_push(wcsip.s.ip);
			emu_push(emu_cs); emu_push(0x11D6); emu_cs = 0x23E1; emu_Tools_Free();
			emu_sp += 4;

			wcsip = next;
			w = (Widget *)emu_get_memorycsip(wcsip);
		}

		emu_push(0xF);
		emu_push(emu_ss); emu_push(emu_bp - 0x314);
		emu_push(emu_cs); emu_push(0x11FB); emu_cs = 0x259E; f__259E_0006_0016_858A();
		emu_sp += 6;

		if (g_global->debugSkipDialogs != 0 || g_global->debugScenario != 0) break;

		{
			csip32 tmp;
			GUI_Widget_Allocate(1, GUI_Widget_GetShortcut(*String_Get_ByIndex(0x6B)), 168, 168, 0, 0, 0, &tmp); /* "Yes" */
			emu_push(tmp.s.cs); emu_push(tmp.s.ip);
		}
		emu_push(wcsip.s.cs); emu_push(wcsip.s.ip);
		emu_push(emu_cs); emu_push(0x126A); emu_cs = 0x348B; overlay(0x348B, 0); f__B48B_0000_001E_7E97();
		emu_sp += 8;
		wcsip.s.cs = emu_dx;
		wcsip.s.ip = emu_ax;
		w = (Widget *)emu_get_memorycsip(wcsip);

		{
			csip32 tmp;
			GUI_Widget_Allocate(2, GUI_Widget_GetShortcut(*String_Get_ByIndex(0x6C)), 240, 168, 2, 0, 0, &tmp); /* "No" */
			emu_push(tmp.s.cs); emu_push(tmp.s.ip);
		}
		emu_push(wcsip.s.cs); emu_push(wcsip.s.ip);
		emu_push(emu_cs); emu_push(0x12C2); emu_cs = 0x348B; overlay(0x348B, 0); f__B48B_0000_001E_7E97();
		emu_sp += 8;
		wcsip.s.cs = emu_dx;
		wcsip.s.ip = emu_ax;
		w = (Widget *)emu_get_memorycsip(wcsip);

		sprintf((char*)g_global->variable_9939, "TEXT%c", *emu_get_memorycsip(g_houseInfo[ret].name));

		String_LoadFile(String_GenerateFilename((char *)g_global->variable_9939), 0, (char *)emu_get_memorycsip(g_global->readBuffer), g_global->readBufferSize);
		String_TranslateSpecial((char *)emu_get_memorycsip(g_global->readBuffer), (char *)emu_get_memorycsip(g_global->readBuffer));

		g_global->playerHouseID = HOUSE_MERCENARY;

		emu_push(0);
		emu_push(emu_cs); emu_push(0x133A); emu_cs = 0x2598; f__2598_0000_0017_EB80();
		emu_sp += 2;
		oldValue_2598_0000 = emu_ax;

		emu_push(emu_cs); emu_push(0x1343); emu_cs = 0x2B6C; f__2B6C_0169_001E_6939();

		emu_push(0);
		emu_push(0); emu_push(0);
		emu_push(g_global->variable_2BBE[ret].s.cs); emu_push(g_global->variable_2BBE[ret].s.ip);
		emu_push(g_global->readBuffer.s.cs); emu_push(g_global->readBuffer.s.ip);
		emu_push(emu_cs); emu_push(0x1366); emu_cs = 0x3511; overlay(0x3511, 0); f__B511_0E44_000C_24F5();
		emu_sp += 14;

		emu_push(1);
		emu_push(g_global->variable_3C32.s.cs); emu_push(g_global->variable_3C32.s.ip);
		emu_push(3);
		emu_push(3);
		String_GenerateFilename("MISC");
		emu_push(0x353F); emu_push(emu_Global_GetIP(g_global->stringFilename, 0x353F));
		emu_push(emu_cs); emu_push(0x1390); emu_cs = 0x34CA; overlay(0x34CA, 0); emu_File_ReadChunkOrLengthFile();
		emu_sp += 14;

		emu_push(emu_cs); emu_push(0x1398); emu_cs = 0x2B6C; f__2B6C_0137_0020_C73F();

		emu_push(0);
		emu_push(2);
		emu_push(24);
		emu_push(26);
		emu_push(0);
		emu_push(0);
		emu_push(0);
		emu_push(0);
		emu_push(emu_cs); emu_push(0x13B8); emu_cs = 0x24D0; f__24D0_000D_0039_C17D();
		emu_sp += 16;

		emu_push(0);
		emu_push(2);
		emu_push(24);
		emu_push(13);
		emu_push(0);
		emu_push(26);
		emu_push(24 * (ret + 1));
		emu_push(0);
		emu_push(emu_cs); emu_push(0x13E4); emu_cs = 0x24D0; f__24D0_000D_0039_C17D();
		emu_sp += 16;

		emu_push(wcsip.s.cs); emu_push(wcsip.s.ip);
		emu_push(emu_cs); emu_push(0x13F2); emu_cs = 0x348B; overlay(0x348B, 0); f__B48B_03A4_0005_619A();
		emu_sp += 4;

		emu_push(emu_cs); emu_push(0x13F9); emu_cs = 0x2B6C; f__2B6C_0169_001E_6939();

		while (true) {
			emu_push(wcsip.s.cs); emu_push(wcsip.s.ip);
			emu_push(1);
			emu_push(0); emu_push(0);
			emu_push(0); emu_push(0);
			emu_push(g_global->variable_2BBE[ret].s.cs); emu_push(g_global->variable_2BBE[ret].s.ip);
			emu_push(emu_cs); emu_push(0x1422); emu_cs = 0x34DA; overlay(0x34DA, 0); f__B4DA_0AB8_002A_AAB2();
			emu_sp += 18;
			yes_no = emu_ax;

			if ((yes_no & 0x8000) != 0) break;
		}

		if (yes_no == 0x8001) {
			emu_push(emu_cs); emu_push(0x143B); emu_cs = 0x1DD7; f__1DD7_0B53_0025_36F7();
		} else {
			emu_push(0xF);
			emu_push(emu_ss); emu_push(emu_bp - 0x314);
			emu_push(emu_cs); emu_push(0x144C); emu_cs = 0x259E; f__259E_0006_0016_858A();
			emu_sp += 6;
		}

		while (w != NULL) {
			csip32 next = w->next;

			emu_push(wcsip.s.cs); emu_push(wcsip.s.ip);
			emu_push(emu_cs); emu_push(0x146C); emu_cs = 0x23E1; emu_Tools_Free();
			emu_sp += 4;

			wcsip = next;
			w = (Widget *)emu_get_memorycsip(wcsip);
		}

		emu_push(emu_cs); emu_push(0x1487); emu_cs = 0x34DA; overlay(0x34DA, 0); f__B4DA_02E0_0023_E297();

		emu_push(emu_cs); emu_push(0x148C); emu_cs = 0x34B8; overlay(0x34B8, 0); f__B4B8_0D23_0010_BA99();

		emu_push(oldValue_2598_0000);
		emu_push(emu_cs); emu_push(0x1494); emu_cs = 0x2598; f__2598_0000_0017_EB80();
		emu_sp += 2;

		while (Driver_Voice_01EB());

		if (yes_no == 0x8001) break;
	}

	Sound_Play(0);

	emu_push(ret);
	emu_push(emu_cs); emu_push(0x14BB); emu_cs = 0x34B8; overlay(0x34B8, 0); f__B4B8_110D_000D_FD5C();
	emu_sp += 2;

	Sprites_Load(0, 7, g_sprites);

	emu_push(emu_cs); emu_push(0x14D8); emu_cs = 0x29E8; emu_Input_History_Clear();

	emu_push(emu_cs); emu_push(0x14DD); emu_cs = 0x2B6C; f__2B6C_0169_001E_6939();

	emu_push(0xF);
	emu_push(emu_ss); emu_push(emu_bp - 0x314);
	emu_push(emu_cs); emu_push(0x14EC); emu_cs = 0x259E; f__259E_0006_0016_858A();
	emu_sp += 6;

	emu_sp = emu_bp;
	emu_pop(&emu_bp);

	return ret;
}

/**
 * Creates a palette mapping: color -> color + reference * intensity.
 *
 * @param palette The palette to create the mapping for.
 * @param colors The resulting mapping.
 * @param reference The color to use as reference.
 * @param intensity The intensity to use.
 */
void GUI_Palette_CreateMapping(uint8 *palette, uint8 *colors, uint8 reference, uint8 intensity)
{
	uint16 index;

	if (palette == NULL || colors == NULL) return;

	colors[0] = 0;

	for (index = 1; index < 256; index++) {
		uint16 i;
		uint8 red   = palette[3 * index + 0] - (((palette[3 * index + 0] - palette[3 * reference + 0]) * (intensity / 2)) >> 7);
		uint8 blue  = palette[3 * index + 1] - (((palette[3 * index + 1] - palette[3 * reference + 1]) * (intensity / 2)) >> 7);
		uint8 green = palette[3 * index + 2] - (((palette[3 * index + 2] - palette[3 * reference + 2]) * (intensity / 2)) >> 7);
		uint8 color = reference;
		uint16 sumMin = 0xFFFF;

		for (i = 1; i < 256; i++) {
			uint16 sum = 0;

			sum += (palette[3 * i + 0] - red)   * (palette[3 * i + 0] - red);
			sum += (palette[3 * i + 1] - blue)  * (palette[3 * i + 1] - blue);
			sum += (palette[3 * i + 2] - green) * (palette[3 * i + 2] - green);

			if (sum > sumMin) continue;
			if ((i != reference) && (i == index)) continue;

			sumMin = sum;
			color = i & 0xFF;
		}

		colors[index] = color;
	}
}
