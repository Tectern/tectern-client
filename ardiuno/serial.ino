//pin for led
int led = 10;

//pins used for relays
const int sensor = 0;
const int power = 8;
const int relayUp = 13;
const int relayDown = 12;
const int buttonDown = 9;
const int buttonUp = 8;

//reading in command
char buffer[1024];

// int parameter number
int parm_val;

// the setup routine runs once when you press reset:
void setup()
{
  // make the led pin an output pin
  pinMode(led, OUTPUT);
  
  // set up the buttons
  pinMode(buttonDown, INPUT);
  pinMode(buttonUp, INPUT);
  
  //setup the pins for height relays
  pinMode(relayUp, OUTPUT);
  pinMode(relayDown, OUTPUT);
  pinMode(power, OUTPUT);
  digitalWrite(power, HIGH);
  // Turn off relays
  digitalWrite(relayUp, LOW);
  digitalWrite(relayDown, LOW);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// Blink the LED for length milliseconds, t times
void blinkLED(int t, int length)
{
  for(int i = 0; i < t; i++)
   {
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(length);               // wait for a second
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
      delay(length);               // wait for a second
   }
}

void loop()
{
  //What are my buttons doing?
  while(digitalRead(buttonUp)==LOW)
  {
    digitalWrite(relayUp, HIGH);
    digitalWrite(relayDown, LOW);
  }
  while(digitalRead(buttonDown)==LOW)
  {
    digitalWrite(relayUp, LOW);
    digitalWrite(relayDown, HIGH);
  }
  //Turn off relays
  digitalWrite(relayUp, LOW);
  digitalWrite(relayDown, LOW);
  digitalWrite(led, LOW);
  
  // Are there readable commands on the Serial port?
  if(Serial.available() > 0 )
  {
    //read in value
    Serial.readBytesUntil(';', buffer, 1024);
     
    // Decorate read string for use with library functions
    String command = String(buffer);
     
    // Blink Command Definition
    if(command.startsWith("blink"))
    {
     // define buffer for parsing params
     char blinkbuff[1024];
     
     command.substring(6).toCharArray(blinkbuff,1024);
     
     parm_val = atoi(blinkbuff);
     
     //Serial.println("Blinking...\n");
     blinkLED(parm_val, 1000);
     //Serial.println(String("Done.\n"));
    }
    else if(command.startsWith("setHeight"))
    {
      // define buffer for parsing params
      char blinkbuff[1024];
     
      command.substring(10).toCharArray(blinkbuff,1024);
      
      parm_val = atoi(blinkbuff);
      
      int currentHeight = analogRead(sensor);
      int targetHeight = parm_val;
      digitalWrite(led, HIGH);
      while(currentHeight != targetHeight)
      {
        currentHeight = analogRead(sensor);
        Serial.print(String("Target Height: ")
            + String(targetHeight) + String("\n"));
        Serial.print(String("Current Height: ")
            + String(currentHeight) + String("\n"));
        if(currentHeight < targetHeight)
        {//Move UP to the target height
          digitalWrite(relayUp, HIGH);
          digitalWrite(relayDown, LOW);
        }
        else if(currentHeight > targetHeight)
        {//Move DOWN to the target height
          digitalWrite(relayUp, LOW);
          digitalWrite(relayDown, HIGH);
        }
      }
      // Turn off relays
      digitalWrite(relayUp, LOW);
      digitalWrite(relayDown, LOW);
      digitalWrite(led, LOW);
      //Serial.print(String("Height set to ")
      //    + String(currentHeight) + String("\n"));
    }
    else if(command.length()>0)
    {
        blinkLED(1,100);
        //Serial.print(String("Command unrecognized.  Please, try again.\n"));
    }
  } 
}
