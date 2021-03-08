/*

*/

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 256 // Popular NeoPixel ring size

#include <Adafruit_NeoPixel.h>
#define NUM_LEDS 256
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

 uint32_t blue = strip.Color(0, 0, 255);
 uint32_t red = strip.Color(255, 0, 0);
 uint32_t green = strip.Color(0, 255, 0);
 uint32_t purple = strip.Color(102, 0, 102);
 uint32_t white = strip.Color(255, 255, 255);
 uint32_t black = strip.Color(0,0,0);

void showStrip();
void setAll(byte, byte, byte);
void strobe(byte, byte, byte, unsigned long, unsigned long);
void sparkle(byte, byte, byte, unsigned long, unsigned long);
void cylonBounce(byte, byte, byte, int, unsigned long, unsigned long);
void FadeInOut(byte, byte, byte, unsigned long, unsigned long);
void meteorRain(byte r, byte g, byte b, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, unsigned long wait, unsigned long n );
void fadeToBlack(int ledNo, byte fadeValue);

/**
 * Loops continously, but this is only going to run once for the whole 'song' instead
 * of running the loop like we did last time
 */
void loop() {
  
  strip.fill(blue, 0, 300);
  strip.show();
  delay(5000);
  strip.fill(black, 0, 300);
  strip.show();
  delay(5000);

  //FadeInOut(255, 255, 0, 2000, 10000);
  cylonBounce(0, 0, 255, 2, 20, 5000);
  
  runningLights(50, 50, 255, 20, 5000);
  // Strobe red every 0.2 seconds for 5 seconds
  strobe(255, 0, 0, 200, 5000);
  // Sparkle random green every 0.2 seconds for 5 seconds
  sparkle(0, 255, 0, 20, 5000);
  
  strobe(255, 0, 0, 100, 500);
  strobe(0, 255, 0, 100, 500);
  strobe(0, 0, 255, 100, 500);

  meteorRain(255, 255, 255, 10, 64, true, 30, 3000);
  

  
  setAll(0, 0, 0);
  delay(5000);
}



/**
 * Strobe a colour (`r`, `g`, `b`) every `wait` ms for `n` ms
 */
void strobe(byte r, byte g, byte b, unsigned long wait, unsigned long n) {
	unsigned long startTime = millis();
  
  // Keep looping until it has been n ms
	while (startTime + n > millis())
  {
    setAll(r, g, b);
    showStrip();
    delay(wait);
    setAll(0, 0, 0);
    showStrip();
    delay(wait);
  }
}

/**
 * Sparkle a random pixel for `wait` ms for `n` ms
 */
void sparkle(byte r, byte g, byte b, unsigned long wait, unsigned long n) {
  unsigned long startTime = millis();
  
  while (startTime + n > millis())
  {
    int pixel = random(NUMPIXELS);
  	setPixel(pixel, r, g, b);
  	showStrip();
  	delay(wait);
  	setPixel(pixel, 0, 0, 0);
  }
}

/**
 * Scroll through each pixel
 */
void runningLights(byte r, byte g, byte b, unsigned long wait, unsigned long n) {
  unsigned long startTime = millis();
  int position = 0;
  while (startTime + n > millis())
  {
    position++;
    for (int i = 0; i < NUM_LEDS; i++)
    {
     	setPixel(i ,((sin(i + position) * 127 + 128) / 255) * r,
                  ((sin(i + position) * 127 + 128) / 255) * g,
                  ((sin(i + position) * 127 + 128) / 255) * b);
    }
    showStrip();
    if (startTime + n < millis())
      return;
    delay(wait);
  }
  
  
}

void cylonBounce(byte r, byte g, byte b, int EyeSize, unsigned long wait, unsigned long n) {
  unsigned long startTime = millis();
  
	while (startTime + n > millis())
  {
  	for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    	setAll(0,0,0);
    	setPixel(i, r/10, g/10, b/10);
    	for(int j = 1; j <= EyeSize; j++) {
    	  setPixel(i+j, r, g, b);
    	}
    	setPixel(i+EyeSize+1, r/10, g/10, b/10);
    	showStrip();
    	delay(wait);
  	}

  	delay(wait);
//^^^ what does this need to be?  
  	for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    	setAll(0,0,0);
    	setPixel(i, r/10, g/10, b/10);
    	for(int j = 1; j <= EyeSize; j++) {
    	  setPixel(i+j, r, g, b);
    	}
    	setPixel(i+EyeSize+1, r/10, g/10, b/10);
    	showStrip();
    	delay(wait);
    }
    delay(wait);
  }
}

void meteorRain(byte r, byte g, byte b, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, unsigned long wait, unsigned long n ) {  
  setAll(0,0,0);
  	unsigned long startTime = millis();
  
  // Keep looping until it has been n ms
	while (startTime + n > millis())
 
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, r, g, b);
      }
    }
   
    showStrip();
    delay(wait);
  }
}

  /*void FadeInOut(byte r, byte g, byte b, unsigned long wait, unsigned long n) {
  float r, g, b;
    
    	while (startTime + n > millis())
      {
     
  for(int k = 0; k < 256; k=k+1) {
    r = (k/256.0)*r;
    g = (k/256.0)*g;
    b = (k/256.0)*b;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*r;
    g = (k/256.0)*g;
    b = (k/256.0)*b;
    setAll(r,g,b);
    showStrip();
  }
 }

}
*/
//this is the part i had no idea on how you changed it ^^^
// which part?
//cause it just had wave delay but you added 2 wait and N
// oh ok
// wait is just wave delay renamed
// and n is the maximum time to run for
// oh okay

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

/*void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}*/

/*void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++); {
    setAll(red,green,blue);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
}*/

/*void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
 
  for(int j=0; j<NUM_LEDS*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
     
      showStrip();
      delay(WaveDelay);
  }
}
  void FadeInOut(byte red, byte green, byte blue) {
  float r, g, b;
     
  for(int k = 0; k < 256; k=k+1) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
 }
 */
/*void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
//^^ so how do you know which one is wait and which one is N ? as in is speedDelay wait and return changes to N
//void CylonBounce(byte r, byte g, byte b, int EyeSize, unsigned long wait, unsigned long n)??
// or is it the other way around?
  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
 
  delay(ReturnDelay);
}*/

/*void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
 
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      }
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}
*/ 
void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

// Loop with more than 2 effects
void oldLoop()
{
   //Show timer
  Serial.print("Timer: ");
  Serial.println(millis());

  strip.clear();

  // Don't forget break in a switch
  // replace 3 with however many effects you have
  // add new effect by adding a new case (count case from 0 but not %)
  
  //switch (millis() / 5000 % 5)
  //{
  // case 0:
  //    Sparkle(255, 255, 255, 0);
  //    break;
  //  case 1:
  //    Strobe(255, 255, 255, 10, 50, 1000);
  //    break;
  //  case 2:
  //   	meteorRain(0xff,0xff,0xff,10, 64, true, 30);
  //    break;
  //  case 3:
  //  	FadeInOut(0xff, 0x90, 0x00);
  //  	FadeInOut(0xff, 0x00, 0x00); // red
  //  	FadeInOut(0xff, 0xff, 0xff); // white
  //  	FadeInOut(0x00, 0x00, 0xff); // blue
  //    break;
  //  case 4:
  //  RunningLights(0xff,0xff,0x00, 50);
  //    break;
  //  case 5:
  //CylonBounce(0xff, 1, 1, 4, 10, 50);
  //    break;
  //   default:
  //   Serial.println("Oh no this is out of range");
  //}
}


/*void CylonBounce(byte r, byte g, byte b, int EyeSize, unsigned long wait, unsigned long n)	
  unsigned long startTime = millis();
  
	while (startTime + n > millis())
    
  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(wait);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(wait);
  }
 
  delay(ReturnDelay);
}*/
