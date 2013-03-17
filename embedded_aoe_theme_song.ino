double f1o2 = 1.0/2.0;
double f1o3 = 1.0/3.0;
double f1o4 = 1.0/4.0;
double f1o8 = 1.0/8.0;
double f1o16 = 1.0/16.0;
double f1o32 = 1.0/32.0;
double f1o64 = 1.0/64.0;

double song[][2] = {
  {24, f1o8},  // first
  {24, f1o8},
  {24, f1o8},
  {24, f1o8},
  {24, f1o4},
  {12, f1o4},
  {17, f1o2 + f1o8},
  {20, f1o8},
  {19, f1o8},
  {17, f1o8},
  {19, 3.0*f1o4},
  {12, f1o4},
  {17, f1o2 + f1o8},
  {20, f1o8},
  {19, f1o8},  
  {17, f1o8},  
  {22, f1o4 * 3.0},
  {12, f1o4},
  {17, f1o8 + f1o2},  // second
  {20, f1o8},
  {19, f1o8},
  {17, f1o8},
  {19, 1.0},
  {19, f1o8 + f1o2},
  {20, f1o8},
  {19, f1o8},
  {15, f1o8},
  {17, 1.0},
  {22, f1o4},
  {21, f1o4 * f1o3},
  {22, f1o4 * f1o3},
  {24, f1o4 * f1o3},
  {25, f1o4},
  {24, f1o4},
  {22, f1o4},
  {18, f1o4},
  {17, f1o4},
  {10, f1o4},
  {22, f1o4 * f1o3 * 2.0},   // third
  {22, f1o4 * f1o3},
  {24, f1o4 * f1o3},
  {25, f1o4 * f1o3},
  {27, f1o4},
  {25, f1o8 + f1o16},
  {24, f1o32},
  {25, f1o32},
  {24, f1o4},
  {21, f1o4},
  {22, f1o2},
  {22, f1o4 * f1o3},
  {17, f1o4 * f1o3},
  {17, f1o4 * f1o3},
  {17, f1o4 * f1o3 * 2.0},
  {20, f1o4 * f1o3},
  {24, f1o4 * f1o3},
  {25, f1o4 * f1o3},
  {24, f1o4 * f1o3},
  {22, f1o4 * f1o3 * 2.0},
  {18, f1o4 * f1o3},
  {17, f1o4},
  {18, f1o4},
  {17, f1o4},
  {10, f1o4},
  {22, f1o4 * f1o3 * 2.0},  // fourth
  {22, f1o4 * f1o3},
  {24, f1o4 * f1o3},
  {25, f1o4 * f1o3},
  {27, f1o4},
  {25, f1o8 + f1o16},
  {24, f1o32},
  {25, f1o32},
  {24, f1o4},
  {21, f1o4},
  {22, f1o4},
  {19, f1o4},
  {24, f1o2 + f1o8},
  {27, f1o8},
  {26, f1o8},
  {24, f1o8},
  {26, f1o4 * 3.0},
  {19, f1o4},
  {24, f1o2 + f1o8},
  {27, f1o8},
  {26, f1o8},
  {24, f1o8},
  {29, f1o4 * 3.0},  // fifth
  {19, f1o4},
  {24, f1o2 + f1o8},
  {27, f1o8},
  {26, f1o8},
  {24, f1o8},
  {26, 1.0},
  {26, f1o4 + f1o8},
  {27, f1o8},
  {26, f1o8},
  {22, f1o8},
  {24, 1.0}
};

int tempoPin = 0;
int pitchPin = 1;
int buzzPin = 3;

int minTempo = 10;
int maxTempo = 320;

int minPitch = -12;
int maxPitch = 12;

void setup(){
  Serial.begin(9600);
  pinMode(buzzPin, OUTPUT);
}

void loop(){
  
  int size = sizeof(song)/sizeof(double[2]);
  for(int i = 0; i < size; i++){
    
    double bpm = map(analogRead(tempoPin), 0, 1024, minTempo, maxTempo);
    double pitch = map(analogRead(pitchPin), 0, 1024, minPitch, maxPitch);
    
    int note = (int) song[i][0];
    double fraction = song[i][1];
    int duration = tempoNote(fraction, bpm);
    
    Serial.print("note:");
    Serial.print(i);
    Serial.print("/");
    Serial.print(size);
    Serial.print(" bpm:");
    Serial.print(bpm);
    Serial.print(" pitch:");
    Serial.print(pitch);
    Serial.print(" note:");
    Serial.println(note);
    
    int silence = getLengthOfMeasure(bpm) * f1o64;
    playSound(freqOfNote(note, pitch), duration - silence);
    delay(silence);
  }
  
  Serial.println("finished");
}

int getLengthOfMeasure(int bpm){
  double measurePerMinute = (double) bpm / 4.0;
  double secondPerMeasure = 60.0 / measurePerMinute;
  return (int) (secondPerMeasure * 1000.0);
}

int tempoNote(double fractionOfMeasure, int bpm){
  return fractionOfMeasure * (double) getLengthOfMeasure(bpm);
}

void playSound(int freqHZ, int durationMS){
  // from Hz (seconds) to microseconds period
  double period = (1.0 / (double) freqHZ) * 1000000.0;
  double halfPeriod = period / 2.0;
  for(double i = 0.0; i < (double) durationMS * 1000.0; i += period){
     digitalWrite(buzzPin, HIGH);
     delayMicroseconds((int) halfPeriod);
     digitalWrite(buzzPin, LOW);
     delayMicroseconds((int) halfPeriod);
  }
}

/*
 keyNote from -48 to 48
*/
int freqOfNote(int keyNote, int pitch){
  double scaledNote = (double) (keyNote + pitch) / 12.0;
  double power2 = pow(2.0, scaledNote);
  double freq = power2 * 440.0;
  return (int) freq;
}

