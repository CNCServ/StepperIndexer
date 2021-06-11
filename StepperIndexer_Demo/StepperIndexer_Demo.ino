#include <StepperIndexer.h>




//*********************************************
//        Indexeur pour Moteur Pas à Pas
//*********************************************
//  version: V1.0
// Auteur: Christian TROUILLET

// NE PAS MODIFIER SVP

int32_t cptmes2;
int32_t cptmes3;

 TStepperIndexer MyStepper(8,9);
 
void setup() {
 
  Serial.begin(38400, SERIAL_8N1 );

 StepperSetupHardWare();
MyStepper.SetAccel(30000);
  establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {

  
  delay(100);
  
 if (Serial.available() > 0) {

    
   int8_t brecept = Serial.read();
   if (brecept == 'a')
      MyStepper.SetSpeed(1000);
   if (brecept == 'b')
      MyStepper.SetSpeed(-12000);
   if (brecept == 'z')
      MyStepper.SetSpeed(0);
    if (brecept == 't')
      MyStepper.GoToTarget(60000,8000);
      if (brecept == 'r')
      MyStepper.GoToTarget(0,10000);
    
 }  
   
    cptmes2=MyStepper.Position;
    
     Serial.print((int32_t)(cptmes2)); 
     Serial.write("\n ");
     
    
  
}

void establishContact() {
  while (Serial.available() <= 0) {

    delay(5);
  }
}
