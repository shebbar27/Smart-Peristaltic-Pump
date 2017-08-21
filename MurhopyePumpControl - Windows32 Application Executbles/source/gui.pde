/* =========================================================
 * ====                   WARNING                        ===
 * =========================================================
 * The code in this tab has been generated from the GUI form
 * designer and care should be taken when editing this file.
 * Only add/edit code inside the event handlers i.e. only
 * use lines between the matching comment tags. e.g.

 void myBtnEvents(GButton button) { //_CODE_:button1:12356:
     // It is safe to enter your event code here  
 } //_CODE_:button1:12356:
 
 * Do not rename this tab!
 * =========================================================
 */

public void panel1_Click1(GPanel source, GEvent event) { //_CODE_:panel1:477812:
  println("panel1 - GPanel >> GEvent." + event + " @ " + millis());
} //_CODE_:panel1:477812:

public void textfield1_change1(GTextField source, GEvent event) { //_CODE_:comTextField:566604:
  //  println("textfield1 - GTextField >> GEvent." + event + " @ " + millis());
    if(event==GEvent.ENTERED || event==GEvent.LOST_FOCUS)
{
  comPort=comTextField.getText();
  comPort=comPort.toUpperCase();
  portName=comPort;
   println(comPort);
}
} //_CODE_:comTextField:566604:

public void button1_click1(GButton source, GEvent event) { //_CODE_:connectButton:525284:
 //println("button1 - GButton >> GEvent." + event + " @ " + millis());
//String portName = Serial.list()[5];
 comPort=comTextField.getText();
  comPort=comPort.toUpperCase();
  portName=comPort;
 Port = new Serial(this, portName, 9600); //starts the Serial port - COM4
  Port.bufferUntil('\n'); 
  println(comPort + "- Port Connected!");
  
} //_CODE_:connectButton:525284:

public void textfield3_change1(GTextField source, GEvent event) { //_CODE_:flowRateTextField:314594:
  // println("textfield3 - GTextField >> GEvent." + event + " @ " + millis());
  if(event==GEvent.ENTERED || event==GEvent.LOST_FOCUS)
  {
    /*if(!flowRateTextField.isEnabled())
    {
     flowRateTextField.setEnabled(true);
    }
    volumeTextField.setEnabled(false);
  */  flowRate=flowRateTextField.getText();
  floatFlowRate= new Float(flowRate).floatValue();
  if(floatFlowRate<=150.00)
  {
    flowRateSlider.setValue(floatFlowRate);
    flowRate=2+flowRate+',';
    buffer=flowRate;
    Port.write(buffer);
    delay(250);
    println(flowRate);
  }
}
} //_CODE_:flowRateTextField:314594:

public void custom_slider1_change1(GCustomSlider source, GEvent event) { //_CODE_:flowRateSlider:424601:
 // println("custom_slider1 - GCustomSlider >> GEvent." + event + " @ " + millis());
  float rate=source.getValueF();
  flowRate=String.valueOf(rate);
  flowRateTextField.setText(flowRate);
} //_CODE_:flowRateSlider:424601:

public void textfield4_change1(GTextField source, GEvent event) { //_CODE_:doseTextField:531894:
//  println("textfield4 - GTextField >> GEvent." + event + " @ " + millis());
    if(event==GEvent.ENTERED || event==GEvent.LOST_FOCUS)
{
  dose=doseTextField.getText();
  //intDose=Integer.parseInt(dose);
  dose=3+dose+',';
buffer=dose;
Port.write(buffer);
  delay(250);
println(buffer);
  }} //_CODE_:doseTextField:531894:

public void textfield5_change1(GTextField source, GEvent event) { //_CODE_:onTimeTextField:255598:
 //   println("textfield5 - GTextField >> GEvent." + event + " @ " + millis());
    if(event==GEvent.ENTERED || event==GEvent.LOST_FOCUS)
{onTime=onTimeTextField.getText();
  intOnTime=Integer.parseInt(onTime);
  onTime=4+onTime+',';
buffer=onTime;
Port.write(buffer);
  delay(250);
println(onTime);
}} //_CODE_:onTimeTextField:255598:

public void textfield6_change1(GTextField source, GEvent event) { //_CODE_:offTimeTextField:378253:
  //println("textfield6 - GTextField >> GEvent." + event + " @ " + millis());
    if(event==GEvent.ENTERED || event==GEvent.LOST_FOCUS)
{
  offTime=offTimeTextField.getText();
  intOffTime=Integer.parseInt(offTime);
  offTime=5+offTime+',';
buffer=offTime;
Port.write(buffer);
  delay(250);
  println(offTime);

}} //_CODE_:offTimeTextField:378253:

public void button3_click1(GButton source, GEvent event) { //_CODE_:StartButton:695473:
  //println("button3 - GButton >> GEvent." + event + " @ " + millis());
//Port.write(7);
//Port.write(2);
//Port.write(",");
buffer=String.valueOf(7);
//buffer+=1;
buffer+=',';
println(buffer);
Port.write("7.");
Port.write('\n');
delay(250);
//Port.clear();

} //_CODE_:StartButton:695473:

public void imgTogButton1_click1(GImageToggleButton source, GEvent event) { //_CODE_:directionTogButton:733443:
buffer=String.valueOf(6);
//buffer+=1;
buffer+=',';
println(buffer);
Port.write(buffer);
//Port.write(6);
//Port.write(1);
//Port.write(",");
//  println("imgTogButton1 - GImageToggleButton >> GEvent." + event + " @ " + millis());
} //_CODE_:directionTogButton:733443:

public void panel2_Click1(GPanel source, GEvent event) { //_CODE_:dosingPanel:324279:
  //println("panel2 - GPanel >> GEvent." + event + " @ " + millis());
} //_CODE_:dosingPanel:324279:

public void textfield15_change1(GTextField source, GEvent event) { //_CODE_:dosesLeftTextField:878196:
  //println("textfield15 - GTextField >> GEvent." + event + " @ " + millis());
} //_CODE_:dosesLeftTextField:878196:

public void textfield16_change1(GTextField source, GEvent event) { //_CODE_:targetTextField:829107:
  //println("textfield16 - GTextField >> GEvent." + event + " @ " + millis());
} //_CODE_:targetTextField:829107:

public void textfield17_change1(GTextField source, GEvent event) { //_CODE_:dispenedTextField:253079:
  //println("textfield17 - GTextField >> GEvent." + event + " @ " + millis());
} //_CODE_:dispenedTextField:253079:

public void textfield18_change1(GTextField source, GEvent event) { //_CODE_:speedTextField:551697:
  //println("textfield18 - GTextField >> GEvent." + event + " @ " + millis());
} //_CODE_:speedTextField:551697:

public void button4_click1(GButton source, GEvent event) { //_CODE_:stopButton:842231:
 // println("button4 - GButton >> GEvent." + event + " @ " + millis());
Port.write("8.");
Port.write('\n');
Port.clear();
delay(250);
} //_CODE_:stopButton:842231:



// Create all the GUI controls. 
// autogenerated do not edit
public void createGUI(){
  G4P.messagesEnabled(false);
  G4P.setGlobalColorScheme(GCScheme.BLUE_SCHEME);
  G4P.setCursor(ARROW);
  surface.setTitle("Murhopye Scentific Company");
  panel1 = new GPanel(this, 30, 10, 300, 60, "Com port Connection :");
  panel1.setText("Com port Connection :");
  panel1.setTextBold();
  panel1.setOpaque(true);
  panel1.addEventHandler(this, "panel1_Click1");
  comConnection = new GToggleGroup();
  comLable = new GLabel(this, 20, 28, 80, 20);
  comLable.setText("Com Port :");
  comLable.setTextBold();
  comLable.setOpaque(false);
  comTextField = new GTextField(this, 105, 28, 80, 20, G4P.SCROLLBARS_NONE);
  comTextField.setPromptText("Enter com port ");
  comTextField.setOpaque(true);
  comTextField.addEventHandler(this, "textfield1_change1");
  connectButton = new GButton(this, 200, 22, 80, 30);
  connectButton.setText("Connect");
  connectButton.setTextBold();
  connectButton.addEventHandler(this, "button1_click1");
  panel1.addControl(comLable);
  panel1.addControl(comTextField);
  panel1.addControl(connectButton);
  flowRateLable = new GLabel(this, 15, 102, 80, 30);
  flowRateLable.setText("Flow Rate        ( ml/min ) :");
  flowRateLable.setTextBold();
  flowRateLable.setOpaque(false);
  flowRateTextField = new GTextField(this, 100, 105, 80, 20, G4P.SCROLLBARS_NONE);
  flowRateTextField.setPromptText("Enter flowrate");
  flowRateTextField.setOpaque(true);
  flowRateTextField.addEventHandler(this, "textfield3_change1");
  flowRateSlider = new GCustomSlider(this, 13, 148, 170, 50, "red_yellow18px");
  flowRateSlider.setShowValue(true);
  flowRateSlider.setShowLimits(true);
  flowRateSlider.setLimits(75.0, 0.0, 150.0);
  flowRateSlider.setShowTicks(true);
  flowRateSlider.setNumberFormat(G4P.DECIMAL, 2);
  flowRateSlider.setOpaque(false);
  flowRateSlider.addEventHandler(this, "custom_slider1_change1");
  doseLable = new GLabel(this, 210, 100, 80, 25);
  doseLable.setText("Number of Doses :");
  doseLable.setTextBold();
  doseLable.setOpaque(false);
  onTimeLable = new GLabel(this, 195, 150, 100, 20);
  onTimeLable.setText("On Time ( Sec ) :");
  onTimeLable.setTextBold();
  onTimeLable.setOpaque(false);
  offTimeLable = new GLabel(this, 200, 200, 100, 20);
  offTimeLable.setText("Off Time (Sec) :");
  offTimeLable.setTextBold();
  offTimeLable.setOpaque(false);
  doseTextField = new GTextField(this, 300, 103, 80, 20, G4P.SCROLLBARS_NONE);
  doseTextField.setPromptText("Provide number");
  doseTextField.setOpaque(true);
  doseTextField.addEventHandler(this, "textfield4_change1");
  onTimeTextField = new GTextField(this, 300, 150, 80, 20, G4P.SCROLLBARS_NONE);
  onTimeTextField.setPromptText("Sec");
  onTimeTextField.setOpaque(true);
  onTimeTextField.addEventHandler(this, "textfield5_change1");
  offTimeTextField = new GTextField(this, 300, 200, 80, 20, G4P.SCROLLBARS_NONE);
  offTimeTextField.setPromptText("Sec");
  offTimeTextField.setOpaque(true);
  offTimeTextField.addEventHandler(this, "textfield6_change1");
  StartButton = new GButton(this, 250, 250, 80, 30);
  StartButton.setText("Start");
  StartButton.setTextBold();
  StartButton.setLocalColorScheme(GCScheme.GREEN_SCHEME);
  StartButton.addEventHandler(this, "button3_click1");
  directionTogButton = new GImageToggleButton(this, 86, 223);
  directionTogButton.addEventHandler(this, "imgTogButton1_click1");
  label16 = new GLabel(this, 3, 235, 80, 20);
  label16.setText("Direction : ");
  label16.setTextBold();
  label16.setOpaque(false);
  label17 = new GLabel(this, 130, 258, 50, 20);
  label17.setText("CW");
  label17.setTextBold();
  label17.setOpaque(false);
  label18 = new GLabel(this, 130, 213, 50, 20);
  label18.setText("CCW");
  label18.setTextBold();
  label18.setOpaque(false);
  dosingPanel = new GPanel(this, 390, 5, 200, 290, "                  Dose In Progressl");
  dosingPanel.setText("                  Dose In Progressl");
  dosingPanel.setTextBold();
  dosingPanel.setOpaque(true);
  dosingPanel.addEventHandler(this, "panel2_Click1");
  togGroup2 = new GToggleGroup();
  doseLeftLabel = new GLabel(this, 30, 40, 80, 20);
  doseLeftLabel.setText("Doses Left :");
  doseLeftLabel.setTextBold();
  doseLeftLabel.setOpaque(false);
  targetLabel = new GLabel(this, 20, 90, 85, 25);
  targetLabel.setText("Target ( ml ) :");
  targetLabel.setTextBold();
  targetLabel.setOpaque(false);
  dispensedLabel = new GLabel(this, 10, 140, 100, 20);
  dispensedLabel.setText("Dispensed (ml) :");
  dispensedLabel.setTextBold();
  dispensedLabel.setOpaque(false);
  dosesLeftTextField = new GTextField(this, 115, 40, 80, 20, G4P.SCROLLBARS_NONE);
  dosesLeftTextField.setPromptText("Press");
  dosesLeftTextField.setOpaque(true);
  dosesLeftTextField.addEventHandler(this, "textfield15_change1");
  targetTextField = new GTextField(this, 115, 90, 80, 20, G4P.SCROLLBARS_NONE);
  targetTextField.setPromptText("Start");
  targetTextField.setOpaque(true);
  targetTextField.addEventHandler(this, "textfield16_change1");
  dispenedTextField = new GTextField(this, 115, 140, 80, 20, G4P.SCROLLBARS_NONE);
  dispenedTextField.setPromptText("Button");
  dispenedTextField.setOpaque(true);
  dispenedTextField.addEventHandler(this, "textfield17_change1");
  speedTextField = new GTextField(this, 115, 193, 80, 20, G4P.SCROLLBARS_NONE);
  speedTextField.setPromptText("-------------");
  speedTextField.setOpaque(true);
  speedTextField.addEventHandler(this, "textfield18_change1");
  stopButton = new GButton(this, 65, 240, 80, 30);
  stopButton.setText("STOP !");
  stopButton.setTextBold();
  stopButton.setLocalColorScheme(GCScheme.RED_SCHEME);
  stopButton.addEventHandler(this, "button4_click1");
  speedLable = new GLabel(this, 15, 193, 90, 20);
  speedLable.setText("Speed (RPM) :");
  speedLable.setTextBold();
  speedLable.setOpaque(false);
  dosingPanel.addControl(doseLeftLabel);
  dosingPanel.addControl(targetLabel);
  dosingPanel.addControl(dispensedLabel);
  dosingPanel.addControl(dosesLeftTextField);
  dosingPanel.addControl(targetTextField);
  dosingPanel.addControl(dispenedTextField);
  dosingPanel.addControl(speedTextField);
  dosingPanel.addControl(stopButton);
  dosingPanel.addControl(speedLable);
}

// Variable declarations 
// autogenerated do not edit
GPanel panel1; 
GToggleGroup comConnection; 
GLabel comLable; 
GTextField comTextField; 
GButton connectButton; 
GLabel flowRateLable; 
GTextField flowRateTextField; 
GCustomSlider flowRateSlider; 
GLabel doseLable; 
GLabel onTimeLable; 
GLabel offTimeLable; 
GTextField doseTextField; 
GTextField onTimeTextField; 
GTextField offTimeTextField; 
GButton StartButton; 
GImageToggleButton directionTogButton; 
GLabel label16; 
GLabel label17; 
GLabel label18; 
GPanel dosingPanel; 
GToggleGroup togGroup2; 
GLabel doseLeftLabel; 
GLabel targetLabel; 
GLabel dispensedLabel; 
GTextField dosesLeftTextField; 
GTextField targetTextField; 
GTextField dispenedTextField; 
GTextField speedTextField; 
GButton stopButton; 
GLabel speedLable; 