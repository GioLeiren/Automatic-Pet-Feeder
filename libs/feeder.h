#ifndef __FEEDER_H_
#define __FEEDER_H_

void feederInitialize();
void feederInstructions();
void printConfigTime();
void printConfigFeeder();
void printTime(char vect[6]);
int adjustOpening();
void printOptions();
void printConfigOpening();
void printConfigOpeningMagnitude();
void printConfigOpeningTime();
void printOpeningMagnitude();
void printOpeningTime();
void printInvalidCommand();
void printCancellingCommand();
void printPleaseConfigure();
void showRealTime();

#endif
