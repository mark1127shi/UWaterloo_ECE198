#pragma once // only program once
#include <Arduino.h>

void coachSetup(){
    Serial.begin(11520);
    Serial.println("Exercise Coach started");
}

void coachLoop(){

}
