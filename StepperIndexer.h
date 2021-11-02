#ifndef StepperIndexer_h
#define StepperIndexer_h
//*********************************************
//        Indexeur pour Moteur Pas à Pas
//*********************************************
//  version: V1.0
// Auteur: Christian TROUILLET CNCServ    
//Librairie permettant de piloter un ou plusieurs moteurs Pas à Pas en position et/ou en vitesse.
// - Prise en compte des accélérations et de décellations en temps réel.
// - fonction non bloquante
#include <Arduino.h>
#define FREQ_MAX 12000  //frequence maximum 12 kHZ pour un moteur
#define VAL_CPTMAX 100000000 // valeur du compteur pour faire un pas
void StepperSetupHardWare();     //initialisation de l'interruption timer
class TStepperIndexer
{
  public:
    int32_t Position;    // position du moteur, peut être lu et écrit.
    uint8_t TargetAtteined; //la position cible est atteinte
    uint8_t  SpeedZero; // le moteur est a l'arrêt
    TStepperIndexer(int8_t pin_Step, int8_t pin_Dir);
   // int32_t GetTest();
    void GoToTarget(int32_t Target,uint16_t Speed);
    void SetSpeed(int16_t Speed);
    void SetAccel(uint16_t Accel);
    void Interrupt();
  private:
    uint32_t _Accel;       // acceleration par cycle 
    int8_t Stade;    // il y a 2 étapes dans l'interruption, une pour generer le pulse en sortie et une pour le remettre a zéro.
                      // les calcul sont réparti sur les 2 étapes
    int8_t ModeTarget;  // Indique qu'il faut prendre en compte la position cible
    int8_t b_Deccel;     // a dévelloper
    int32_t _Speed;       // vitesse de consigne = vitesse en Step/s X 
    int32_t _Target;     // position cible
    int32_t SpeedInterpolate;  // vitesse instantanée
    int32_t CptInterpolate;  // quand ce compteur atteint  VAL_CPTMAX on fait un Step
    int Direction;
    int Pulse;                //à 1 il faut generer un pulse
    
    float Coef_Speed;        // coeficient pour convertir la vitesse de consigne 
    float FSpeed;            // vitesse en flottant en step par cycle, donc toujours <= à 1 , elle est ensuite élevée au ²
    float FAccel_div2;       // = 0.5a
    float VMax;              // vitesse maximum au ² calculée en fonction de la distance restante -> D * 0.5 * a
    uint8_t _pin_Step;          // pin de pour la sortie Step entre 8 et 13
    uint8_t _pin_Dir;          // pin de pour la sortie Step entre 8 et 13
    volatile uint8_t *_out_Step;  
    volatile uint8_t *_out_Dir;        
};    
    
#endif
