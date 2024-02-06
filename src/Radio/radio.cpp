#include "radio.hh"

RF24 radio(RADIO_CE_PIN, RADIO_CSN_PIN);

void radioSetup(uint8_t addressIDX)
{
    if (!radio.begin())
    {
        Serial.println("Radio begin failed PINS:(" + String(RADIO_CE_PIN) + " | " + String(RADIO_CSN_PIN) + ")");
        return;
    }
    radio.setPALevel(RF24_PA_HIGH);
    radio.openWritingPipe(RADIO_ADDRESSES[addressIDX]);
    radio.openReadingPipe(1, RADIO_ADDRESSES[!addressIDX]);
    radio.printDetails();
    Serial.printf("Radio setup done AddresIDX: %d | CH.Def: %d\n", addressIDX, radio.getChannel());
}