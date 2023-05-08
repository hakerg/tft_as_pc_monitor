// Visual Micro is in vMicro>General>Tutorial Mode
//
/*
	Name:       tft_as_pc_monitor.ino
	Created:	09.10.2019 18:28:18
	Author:     DESKTOP-Q27SJAP\grzeg
*/

// Define User Types below here or use a .h file
//

#include <ILI9341_due.h>
//#include <fonts/Arial_bold_14.h>
//#include <fonts/SystemFont5x7.h>
#include <URTouch.h>

#define T_SCK   3
#define T_CS    4
#define T_MOSI  5
#define T_MISO  6
#define T_IRQ   7
#define TFT_RST 8
#define TFT_CS  9
#define TFT_DC 10

ILI9341_due tft(TFT_CS, TFT_DC, TFT_RST);
URTouch ts(T_SCK, T_CS, T_MOSI, T_MISO, T_IRQ);

// Define Function Prototypes that use User Types below here or use a .h file
//

// Define Functions below here or use other .ino or cpp files
//

uint16_t xl;
uint16_t y;
uint16_t sizexh;

uint16_t x;
uint16_t sizeX;
uint16_t sizeY;

bool lastMinusOne = true;
unsigned long nextTouch = 0;

inline void writeTouch()
{
	Serial.write('T');
	Serial.write(ts.TP_X & 0xFF);
	Serial.write(ts.TP_X >> 8);
	Serial.write(ts.TP_Y & 0xFF);
	Serial.write(ts.TP_Y >> 8);
	lastMinusOne = (ts.TP_X == -1);
}

inline void checkTouch()
{
	if (millis() >= nextTouch)
	{
		if (ts.dataAvailable())
		{
			ts.read();
			writeTouch();
		}
		else if (!lastMinusOne)
		{
			ts.TP_X = -1;
			writeTouch();
		}
		nextTouch = millis() + 10;
	}
}

/*void setSmallFont()
{
	tft.setFont(SystemFont5x7);
	tft.setTextLetterSpacing(3);
	tft.setTextLineSpacing(1);
}

void setHugeFont()
{
	tft.setFont(Arial_bold_14);
	tft.setTextLetterSpacing(6);
	tft.setTextLineSpacing(2);
}*/

void PerformanceTest()
{
	for (;;)
	{
		unsigned long time = micros();
		tft.fillRect(0, 0, 320, 240, 0xFFFF);
		time = micros() - time;
		Serial.print("Time [us]: ");
		Serial.println(time);
	}
}

// The setup() function runs once each time the micro-controller starts
void setup()
{
	Serial.begin(2000000);
	tft.begin();
	tft.setRotation(iliRotation90);
	tft.fillScreen(ILI9341_BLUE);
	tft.setTextColor(ILI9341_WHITE, ILI9341_BLUE);

	ts.InitTouch();
	ts.setPrecision(PREC_EXTREME);
	Serial.flush();

	//PerformanceTest();

	for (;;)
	{
		Serial.write('K');

		for (int i = 0; i < 200; i++)
		{
			do
			{
				checkTouch();
			} while (Serial.available() < 5);

			xl = Serial.read();
			y = Serial.read();
			sizexh = Serial.read();

			x = xl | ((sizexh & 1) << 8);
			sizeX = (1 << ((sizexh >> 1) & 0xF));
			sizeY = (1 << (sizexh >> 5));

			tft.fillRect(x, y, sizeX, sizeY, (uint16_t)Serial.read() | (((uint16_t)Serial.read()) << 8));
		}
	}
}

// Add the main program code into the continuous loop() function
void loop()
{
}