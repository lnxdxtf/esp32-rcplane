#pragma once
#include <SPI.h>
#include <RF24.h>
#include "../config.hh"

const uint8_t RADIO_ADDRESSES[2][4] = {"000", "001"};
const uint16_t RADIO_MAX_PAYLOAD_SIZE = 32;
const String RADIO_EOF = "EOF";

extern RF24 radio;

void radioSetup(uint8_t addressIDX);