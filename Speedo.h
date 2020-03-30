PROGMEM const byte PulsesPerRevolution = 4;  // Set how many pulses there are on each revolution. Default: 2.
PROGMEM const unsigned long ZeroTimeout = 350000;  // For high response time, a good value would be 100000.
PROGMEM const byte numReadings = 2;
volatile unsigned long LastTimeWeMeasured;  // Stores the last time we measured a pulse so we can calculate the period.
volatile unsigned long PeriodBetweenPulses = ZeroTimeout+1000; 
volatile unsigned long PeriodAverage = ZeroTimeout+1000;  // Stores the period between pulses in microseconds in total, if we are taking multiple pulses.
unsigned long FrequencyRaw;  // Calculated frequency, based on the period. This has a lot of extra decimals without the decimal point.
unsigned long RPM;  // Raw RPM without any processing.
unsigned int PulseCounter = 1;
unsigned long LastTimeCycleMeasure = LastTimeWeMeasured;
unsigned long CurrentMicros = micros();
unsigned int AmountOfReadings = 5;
unsigned int ZeroDebouncingExtra;
unsigned long PeriodSum;
//


void readPulses()  // The interrupt runs this to calculate the period between pulses:
{
  PeriodBetweenPulses = micros() - LastTimeWeMeasured;  // Current "micros" minus the old "micros" when the last pulse happens.
  LastTimeWeMeasured = micros();  // Stores the current micros so the next time we have a pulse we would have something to compare with.
  if(PulseCounter >= AmountOfReadings)  // If counter for amount of readings reach the set limit:
  {
    PeriodAverage = PeriodSum / AmountOfReadings;  // Calculate the final period dividing the sum of all readings by the
    PulseCounter = 1;  // Reset the counter to start over. The reset value is 1 because its the minimum setting allowed (1 reading).
    PeriodSum = PeriodBetweenPulses;  // Reset PeriodSum to start a new averaging operation.
    int RemapedAmountOfReadings = map(PeriodBetweenPulses, 40000, 5000, 1, 10);  // Remap the period range to the reading range.
    RemapedAmountOfReadings = constrain(RemapedAmountOfReadings, 1, 10);  // Constrain the value so it doesn't go below or above the limits.
    AmountOfReadings = RemapedAmountOfReadings;  // Set amount of readings as the remaped value.
  }
  else
  {
    PulseCounter++;  // Increase the counter for amount of readings by 1.
    PeriodSum = PeriodSum + PeriodBetweenPulses;  // Add the periods so later we can average.
  }
}  // End of Pulse_Event.



int getRpm() 
{
  LastTimeCycleMeasure = LastTimeWeMeasured;  // Store the LastTimeWeMeasured in a variable.
  CurrentMicros = micros();  // Store the micros() in a variable.
  if(CurrentMicros < LastTimeCycleMeasure)
  {
    LastTimeCycleMeasure = CurrentMicros;
  }
  FrequencyRaw = 10000000000 / PeriodAverage;  // Calculate the frequency using the period between pulses.
  
  if(PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure > ZeroTimeout - ZeroDebouncingExtra)
  {  // If the pulses are too far apart that we reached the timeout for zero:
    FrequencyRaw = 0;  // Set frequency as 0.
    ZeroDebouncingExtra = 2000;  // Change the threshold a little so it doesn't bounce.
  }
  else
  {
    ZeroDebouncingExtra = 0;  // Reset the threshold to the normal value so it doesn't bounce.
  }
  RPM = FrequencyRaw / PulsesPerRevolution * 60;  // Frequency divided by amount of pulses per revolution multiply by
                                                  // 60 seconds to get minutes.
  RPM = RPM / 10000;  // Remove the decimals.
  return RPM;
}
//
//void readPulses()  // The interrupt runs this to calculate the period between pulses:
//{
//  PeriodBetweenPulses = micros() - LastTimeWeMeasured;  // Current "micros" minus the old "micros" when the last pulse happens.
//  LastTimeWeMeasured = micros();  // Stores the current micros so the next time we have a pulse we would have something to compare with.
//  if(PulseCounter >= AmountOfReadings)  // If counter for amount of readings reach the set limit:
//  {
//    PeriodAverage = PeriodSum / AmountOfReadings;  // Calculate the final period dividing the sum of all readings by the
//    PulseCounter = 1;  // Reset the counter to start over. The reset value is 1 because its the minimum setting allowed (1 reading).
//    PeriodSum = PeriodBetweenPulses;  // Reset PeriodSum to start a new averaging operation.
//    int RemapedAmountOfReadings = map(PeriodBetweenPulses, 40000, 5000, 1, 10);  // Remap the period range to the reading range.
//    RemapedAmountOfReadings = constrain(RemapedAmountOfReadings, 1, 10);  // Constrain the value so it doesn't go below or above the limits.
//    AmountOfReadings = RemapedAmountOfReadings;  // Set amount of readings as the remaped value.
//  }
//  else
//  {
//    PulseCounter++;  // Increase the counter for amount of readings by 1.
//    PeriodSum = PeriodSum + PeriodBetweenPulses;  // Add the periods so later we can average.
//  }
//}  // End of Pulse_Event.
