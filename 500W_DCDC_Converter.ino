//defining relay output pin
int dc = 2;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);

}

void loop() {
  // variables
  int wait = 1000;
  //max current is 136% of nominal current
  //nominal current = 10.42A
  //10.42 * 1.36 = 14.172 A
  //Current accross shunt resistor = 14.172
  //Voltage = 14.172 * 0.003 = 0.0425V
  //Gain of 50 therefore 0.0425 * 50 = 2.126V
  //equivilant analog reading: ( 2.126 / 5 ) * 1024 = 435
  int maxVoltage = 435;
  
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  
  //pin goes to high until hall sensor pucks up an excessive ammount of current
  digitalWrite(dc, HIGH);

  //read analog signal for shunt sensor IC
  int shuntRead = analogRead(A3);

  //overcurret relay will turn off if offTrigger defined as "True", defined later
  bool offTrigger = false;

  //trigger if it goes over allowable current
  if ( shuntRead > maxVoltage ) {
    int startTime = millis();
    int endTime = startTime + 2000;

    //begins countdown exits if voltage goes back down OR offTrigger variable (described later) is true
    while ( ( shuntRead < maxVoltage ) || ( offTrigger == true ) ) {

      //if shuntRead remains high for 2 seconds (as defined by last while statement, offTrigger will be true)
      if ( millis() > endTime ) {
        offTrigger = true;
      }
      
      //final shunt read
      shuntRead = analogRead(A3);
    }
  }

  //if offTrigger is true, we need to turn off relay for 2 seconds;
  if (offTrigger == true) {
    digitalWrite(dc, LOW);
    delay(1000);
  }
}
