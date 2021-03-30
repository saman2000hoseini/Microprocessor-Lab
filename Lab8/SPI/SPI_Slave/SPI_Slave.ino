#include <SPI.h>

char buf [100];
volatile byte pos;
volatile boolean process_it;

void setup (void)
{
  Serial.begin(9600);

  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);

  pinMode(MISO, OUTPUT);

  pos = 0;
  process_it = false;

  SPI.attachInterrupt();
  
}

ISR (SPI_STC_vect)
{
  byte c = SPDR;
  if (pos < (sizeof (buf) - 1))
    buf[pos++] = c;

  if (c == '\n')
    process_it = true;
}

void loop (void)
{
  if (process_it)
  {
    buf[pos] = 0;
    Serial.println(buf);
    pos = 0;
    process_it = false;
  }
}
