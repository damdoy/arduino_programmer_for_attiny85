#include <SPI.h>
#include "blinky_image.h"

#define N_RESET_PIN 8

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  
  pinMode(N_RESET_PIN, OUTPUT); //reset
  pinMode(LED_BUILTIN, OUTPUT);
  SPI.setClockDivider(SPI_CLOCK_DIV32); //dont care too much about spi speed
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
}

void loop()
{
  digitalWrite(N_RESET_PIN, HIGH);
  delay(500);
  digitalWrite(N_RESET_PIN, LOW);
  delay(500);
  //prog mode
  SPI.transfer(0xAC);
  SPI.transfer(0x53);
  uint8_t echo_byte = SPI.transfer(0x00);
  SPI.transfer(0x00);
  
  Serial.print("echo back: ");
  Serial.print(echo_byte, HEX);
  Serial.print("\n");
  
  delay(500);
  //chip erase
  SPI.transfer(0xAC);
  SPI.transfer(0x80);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  
  delay(500);
  //read low fuses
  SPI.transfer(0x50);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  uint8_t answer_byte = SPI.transfer(0x00);
  Serial.print("low fuses: ");
  Serial.print(answer_byte, HEX);
  Serial.print("\n");
  
  delay(500);
  //read high fuses
  SPI.transfer(0x50);
  SPI.transfer(0x08);
  SPI.transfer(0x00);
  answer_byte = SPI.transfer(0x00);
  Serial.print("high fuses: ");
  Serial.print(answer_byte, HEX);
  Serial.print("\n");
  
  delay(500);
  //read signature
  SPI.transfer(0x30);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  uint8_t data_recv = SPI.transfer(0x00);
  Serial.print("read signature: ");
  Serial.print(data_recv, HEX);
  Serial.print("\n");
  
  delay(500);
  
  uint16_t global_byte_counter = 0;
  uint16_t page_word_addr = 0;
  uint16_t page_addr = 0;
  
  while(global_byte_counter < attiny85_src_out_bin_len)
  {
    if(page_word_addr == 32)
    {
      //write current page to memory
      SPI.transfer(0x4C);
      SPI.transfer(page_addr>>3);
      SPI.transfer(page_addr<<5);
      SPI.transfer(0x00);
      
      Serial.print("write page");
      Serial.print(page_addr);
      Serial.print("\n");
      
      delay(500);
      page_word_addr = 0;
      page_addr++;
    }
    //write low byte in the page
    SPI.transfer(0x40);
    SPI.transfer(0x00);
    SPI.transfer(page_word_addr);
    SPI.transfer(attiny85_src_out_bin[global_byte_counter]);
    
    global_byte_counter++;
    //only do the high byte if not at the end of the program
    if(global_byte_counter < attiny85_src_out_bin_len)
    {
      //write high byte in the page
      SPI.transfer(0x48);
      SPI.transfer(0x00);
      SPI.transfer(page_word_addr);
      SPI.transfer(attiny85_src_out_bin[global_byte_counter]);
      
      global_byte_counter++;
      page_word_addr++;
    }
  }
  //save last page in the flash
  SPI.transfer(0x4C);
  SPI.transfer(page_addr>>3);
  SPI.transfer(page_addr<<5);
  SPI.transfer(0x00);
  page_addr++;
  delay(500);
  
  Serial.print("finished\n");
  
  //read first byte from flash, just for confirm
  SPI.transfer(0x20);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  uint8_t mem_read = SPI.transfer(0x00);
  Serial.print("mem_read: ");
  Serial.print(mem_read, HEX);
  Serial.print("\n");
  
  //release reset, attiny should now run the firmware
  digitalWrite(N_RESET_PIN, HIGH);
  delay(500);
  
  while(1)
  {
    
  }
}
