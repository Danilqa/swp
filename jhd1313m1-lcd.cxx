/*
 * Author: Yevgeniy Kiveish <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "jhd1313m1.hpp"
#include <unistd.h>
#include <math.h>
#include "mraa/aio.h"

int
main(int argc, char **argv)
{
//! [Interesting]
    // 0x62 RGB_ADDRESS, 0x3E LCD_ADDRESS
    mraa_aio_context adc_a0;
    uint16_t adc_value = 0;
    float adc_value_float = 0.0;
    float resistance = 0.0;
    float temperature = 0.0;
    int B = 3975;
    upm::Jhd1313m1 *lcd = new upm::Jhd1313m1(0, 0x3E, 0x62);
    adc_a0 = mraa_aio_init(0);
    if (adc_a0 == NULL) {
        return 1;
    }

    for (;;) {
        adc_value = mraa_aio_read(adc_a0);
        //adc_value_float = mraa_aio_read_float(adc_a0);
        resistance = (float)(1023 - adc_value)*10000 / (float) adc_value;
        temperature = 1 / (log(resistance/10000)/B+1/298.15)-273.15;
        //fprintf(stdout, "ADC A1 read %d\n", adc_value);
        if((float)temperature<16)
        {
            lcd->setCursor(0,0);
            lcd->setColor(0,0,255);
            lcd->clear();
            lcd->write("Cold!");
            fprintf(stdout, "ADC A1 read float - %.5f\n", (float)temperature);
        }
        else if((float)temperature>30)
        {
            lcd->setCursor(0,0);
            lcd->setColor(255,0,0);
            lcd->clear();
            lcd->write("Hot!");
            fprintf(stdout, "ADC A1 read float - %.5f\n", (float)temperature);
        }
        else
        {
            lcd->setCursor(0,0);
            lcd->setColor(0,255,0);
            lcd->clear();
            lcd->write("Kaef)))");
            fprintf(stdout, "ADC A1 read float - %.5f\n", (float)temperature);
        }
	if(adc_value > 1000)
		printf("Detect vibration\n");
	   usleep(50000);
    }
    
    lcd->setCursor(0,0);
    lcd->write("Hello World");

    printf("Sleeping for 5 seconds\n");
    sleep(5);
    delete lcd;
    mraa_aio_close(adc_a0);
//! [Interesting]
    return 0;
}
