// Need G4P library
import g4p_controls.*;
import processing.serial.*;

int i=0;
char LF=10;
Serial Port;


boolean firstContact = false; 
String comPort=null,portName, volume, flowRate, dose, onTime, offTime,buffer;
Float floatVolume,floatFlowRate;
int intDose,intOnTime,intOffTime,delimiterPosition;

public void setup() {
  size(600, 300, JAVA2D);
  createGUI();
  
  customGUI();

  // Place your setup code here
}
/*void serialEvent(Serial myPort)
{
 // read the serial buffer:
 String myString = myPort.readStringUntil('\n');
 println("--->"+myString);
myPort.clear();
}*/

public void draw() {
  background(230);
  }

// Use this method to add additional statements
// to customise the GUI controls
public void customGUI() {
  String portName[] = Serial.list();
 comTextField.setText(portName[0]);

}


void serialEvent(Serial myPort){
  // read the serial buffer:
  String myString = myPort.readStringUntil('\n');
  // if you got any bytes other than the linefeed:
  if (myString != null) {
println(myString); 
  myString = trim(myString);
 println(myString);
    // if you haven't heard from the microncontroller yet, listen:
   /* if (firstContact == false) {
      if (myString.equals("hello"))
      {
        println("Stuck here!!!");
        myPort.clear();          // clear the serial port buffer
        firstContact = true;     // you've had first contact from the microcontroller
        myPort.write('A');
      }
    }
    */
    if(myString.equals("R"))
    {
      myPort.clear();
      buffer="";
    println("Data sent sucessufully!");
   }
    // if you have heard from the microcontroller, proceed:

    else if((myString.charAt(0)==('S'))){
       //println(myString);
  //  // split the string at the commas
      // and convert the sections into integers:
    println("updating values to text fields");
    //String  temp[] = myString.split(myString, ',');
      //print out the values you got:
     int temp=0;
     int delimiterPosition; 
  delimiterPosition=myString.indexOf(",");
 buffer=myString.substring(delimiterPosition);
  println(delimiterPosition);
if(delimiterPosition!=-1)
  {
  buffer=myString.substring(1,delimiterPosition);
  dosesLeftTextField.setText(buffer);
 temp=delimiterPosition;
 println(buffer);
 println(temp);
}
  delimiterPosition=myString.indexOf(",",temp+1);
  println(delimiterPosition);
  if(delimiterPosition!=-1)
  {
  buffer=myString.substring(temp+1,delimiterPosition);
     targetTextField.setText(buffer);
  temp=delimiterPosition;
println(buffer);
 println(temp);
 }   
 delimiterPosition=myString.indexOf(",",temp+1);
  println(delimiterPosition);
 //print(temp[0]);
if(delimiterPosition!=-1)
  {
  buffer=myString.substring(temp+1,delimiterPosition);
    dispenedTextField.setText(buffer);
     temp=delimiterPosition;
println(buffer);
 println(temp);
 }
delimiterPosition=myString.indexOf(",",temp+1);
  println(delimiterPosition);
 if(delimiterPosition!=-1)
  {
  buffer=myString.substring(temp+1,delimiterPosition);
   speedTextField.setText(buffer);
     temp=delimiterPosition;
println(buffer);
 println(temp);
 } 
  }
  else if((myString.charAt(0)==('V'))){
       //println(myString);
  //  // split the string at the commas
      // and convert the sections into integers:
    println("updating values to text fields");
    //String  temp[] = myString.split(myString, ',');
      //print out the values you got:
     int delimiterPosition; 
  delimiterPosition=myString.indexOf(",");
 buffer=myString.substring(delimiterPosition);
  println(delimiterPosition);
if(delimiterPosition!=-1)
  {
  buffer=myString.substring(1,delimiterPosition);
    dispenedTextField.setText(buffer);
 }
  }
    else if((myString.charAt(0)==('D'))){
       //println(myString);
  //  // split the string at the commas
      // and convert the sections into integers:
    println("updating values to text fields");
    //String  temp[] = myString.split(myString, ',');
      //print out the values you got:
     int delimiterPosition; 
  delimiterPosition=myString.indexOf(",");
 buffer=myString.substring(delimiterPosition);
  println(delimiterPosition);
if(delimiterPosition!=-1)
  {
  buffer=myString.substring(1,delimiterPosition);
   dosesLeftTextField.setText(buffer);
 }
 }
  else if((myString.charAt(0)==('T'))){
       //println(myString);
  //  // split the string at the commas
      // and convert the sections into integers:
    println("updating values to text fields");
    //String  temp[] = myString.split(myString, ',');
      //print out the values you got:
     int delimiterPosition; 
  delimiterPosition=myString.indexOf(",");
 buffer=myString.substring(delimiterPosition);
  println(delimiterPosition);
if(delimiterPosition!=-1)
  {
  buffer=myString.substring(1,delimiterPosition);
    targetTextField.setText(buffer);
 }
  }
   else if((myString.charAt(0)==('R'))){
       //println(myString);
  //  // split the string at the commas
      // and convert the sections into integers:
    println("updating values to text fields");
    //String  temp[] = myString.split(myString, ',');
      //print out the values you got:
     int delimiterPosition; 
  delimiterPosition=myString.indexOf(",");
 buffer=myString.substring(delimiterPosition);
  println(delimiterPosition);
if(delimiterPosition!=-1)
  {
  buffer=myString.substring(1,delimiterPosition);
    speedTextField.setText(buffer);
 }
  }

  }

}

  
  