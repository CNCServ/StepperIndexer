
#include <Arduino.h> 
#include "StepperIndexer.h"

int HardWareInit = 0;    // indique que le timer a bien été initialisé
TStepperIndexer *StepperInstance[5];
int NbInstance = 0;  
int CptInstance = 0;
int32_t cptmes=0;
int32_t Speed_Max=12000;
TStepperIndexer::TStepperIndexer(int8_t pin_Step, int8_t pin_Dir) // constructeur
{
   
    StepperInstance[ NbInstance] = this;
     NbInstance++;
  
    _Speed=0;
    CptInterpolate=0;
    _pin_Step = digitalPinToBitMask(pin_Step);
	  _pin_Dir = digitalPinToBitMask(pin_Dir);
	  uint8_t port = digitalPinToPort(pin_Step);
	  _out_Step = portOutputRegister(port);
    port = digitalPinToPort(pin_Dir);
	  _out_Dir = portOutputRegister(port);
     pinMode(pin_Dir, OUTPUT); 
     pinMode(pin_Step, OUTPUT); 
     
    
    
}

void TStepperIndexer::GoToTarget(int32_t Target,uint16_t Speed)
{
   
    _Target = Target;
    TargetAtteined =0;

    if (Target == Position )
	{
      Speed = 0;
	TargetAtteined =1;
     }
    if (Target <  Position )
      Speed = -Speed;
    ModeTarget = 1;
    SetSpeed(Speed) ;
    
}
void TStepperIndexer::SetSpeed(int16_t Speed) 
{
  
   if ( Speed > Speed_Max)
         Speed = Speed_Max;
   if ( Speed < -Speed_Max)
         Speed = -Speed_Max;      
    _Speed = Speed * Coef_Speed;
    if (Speed == 0)
       ModeTarget = 0;
       TargetAtteined = 0;
}
void TStepperIndexer::SetAccel(uint16_t Accel) 
{
    Speed_Max = FREQ_MAX / NbInstance;
   Coef_Speed= (float)VAL_CPTMAX/Speed_Max;
    _Accel = (Accel*Coef_Speed)/Speed_Max;
    FAccel_div2 =  ((float)Accel*2);
    FAccel_div2/= Speed_Max;
    FAccel_div2/= Speed_Max;
}
void TStepperIndexer::Interrupt() 
{
  *_out_Step &=  0xff^_pin_Step; 
  if( ModeTarget )
       {
          if (_Target== Position)
          {
             
                SpeedInterpolate = 0; 
              _Speed = 0; 
              ModeTarget = 0;
              TargetAtteined =1;
          }
       }
  if (! Stade )
  {
       if (Pulse ==2 )
       {
           *_out_Step |=  _pin_Step;
            Pulse =0;
        }
       if ( SpeedInterpolate < _Speed )  {
          SpeedInterpolate += _Accel;
            if ( SpeedInterpolate > _Speed )
              SpeedInterpolate = _Speed; 
       }
       else if (( SpeedInterpolate > _Speed ))   {
          SpeedInterpolate -= _Accel;
            if ( SpeedInterpolate < _Speed )
              SpeedInterpolate = _Speed; 
       }
       CptInterpolate += SpeedInterpolate;
       Stade =1;
       Direction=(( CptInterpolate > 0 ) ? 1 : -1);
       if (Direction == 1)
          *_out_Dir &=  0xff^_pin_Dir;
       else
          *_out_Dir |=  _pin_Dir;
          
       
       if ( CptInterpolate > VAL_CPTMAX)
       {
            CptInterpolate -= VAL_CPTMAX;
            
            Position++;
             if ( CptInterpolate > (VAL_CPTMAX/2))
                Pulse =  1;
            else
                Pulse =  2;
                
       }
       if ( CptInterpolate < -VAL_CPTMAX )
       {
            CptInterpolate += VAL_CPTMAX;
             if ( CptInterpolate < -(VAL_CPTMAX/2))
                Pulse =  1;
            else
                Pulse =  2;
            Position--;
       }
       
        FSpeed = (float )  (SpeedInterpolate) /VAL_CPTMAX; 
        //FSpeed /= Coef_Speed;
        FSpeed *=  FSpeed;
       
   }
   else
    {
       Stade =0;
       if (Pulse ==1 )
       {
           *_out_Step |=  _pin_Step;
            Pulse =0;
        }
       if ( ModeTarget ) 
       {
           if  (_Target >  Position) 
           {
             VMax = (_Target -  Position)  *  FAccel_div2;   
           }
           else
           {
             VMax = (Position - _Target)  *  FAccel_div2;
           }
         
           if ( VMax < FSpeed )
           {
             if (_Speed > 0)
             {
                 if (_Speed > SpeedInterpolate)
                  _Speed = SpeedInterpolate;
                 _Speed -= _Accel;
                 
              }
             else 
             {
                if (_Speed < SpeedInterpolate)
                   _Speed = SpeedInterpolate;
                _Speed += _Accel;
              }
           }
        }
       
         
      
    }
    
    if (SpeedInterpolate != 0)
     SpeedZero = 0;
    else
      SpeedZero=1;
}

void StepperSetupHardWare()
{
  /* DDRD = B11111100; 
     PORTD = 0;
     DDRB = B00110001;
     PORTB = B00001110;
    DDRC = B00001000;
    PORTC = B00110111; */  
  //pinMode(7, OUTPUT); // pour mesurer les durée d'interruption
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 2000000 / (FREQ_MAX*2) ;            // compare match register
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS11);    // 8 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts      
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  //PORTB = B0000001;
  CptInstance++;
  if ( CptInstance >= NbInstance)
    CptInstance = 0;
  StepperInstance[CptInstance]->Interrupt();
//  PORTB = B0000000;

}      
