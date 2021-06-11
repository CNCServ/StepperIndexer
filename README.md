# StepperIndexer
Pilotage de moteur Pas à Pas en Position et en vitesse en tache de fond

Installation :
-	Copier StepperIndexer.h et StepperIndexer.cpp dans le dossier « Documents\Arduino\libraries\StepperIndexer »

Version Béta reste à faire :
-	Optimisation des calculs en virgule flottante pour gagner en fréquence,
-	Décélération différente de l’accélération,
-	Ajout de drapeaux : « Vitesse Atteinte », « Position Atteinte », « vitesse zéro »..
-	Améliorer la stabilité des pulses à vitesse élevée. ( l'irregularité des step peut générer des bruits un peu bizzares mais n'a pas d'importance sur le fonctionnement en lui même)
-	….

Usage :
-	StepperSetupHardWare() : 
Doit être appelé dans Setup pour initialiser l’interruption timer.

-	TStepperIndexer(int8_t pin_Step, int8_t pin_Dir) : constructeur 
    Les pins de sortie pour Step est Dir doivent être comprise entre 8 et 13.
    On peut déclarer jusqu’à 3 moteurs.
    La fréquence maximale en Step/s dépend du nombre de moteurs, :
    o	1 moteur -> 12000  Step/s ;
    o	2 moteurs -> 6000 Step/s ; 
    o	3 moteurs -> 4000 Step/s

-	SetAccel(uint16_t Accel) :
    Défini l’accélération  en Step/s², la valeur maximum est 65535 ( entier 16bits non signé)

-	SetSpeed(int16_t Speed) :
    Commande en vitesse en Step/s, la valeur de Speed peut être negative ou positive en fonction du sens désiré. Speed est limité en fonction de la fréquence Maximale.
    Si Speed est à Zéro le moteur est stoppé.

-	GoToTarget(int32_t Target,uint16_t Speed);
    Déplacement à la position Target à la vitesse Speed. Speed est toujours positif, le sens dépend la position actuelle. Le déplacement peut-être interrompu avec Set Speed(0).   

-	Position , variable contenant la position du moteur, elle peut-être aussi écrite comme par exemple pour une prise d’origine.


Sketch ARDUINO Stepper indexer démo :
       
Moniteur sur Port série : 38400,8,N,1

« a » -> rotation  en positif à 1000 Step/s,

« b » -> rotation en négatif à 12000 Step/s,

« r » -> retour à la position 0 à 10000 Step/s,

« t» -> déplacement à la position 60000 à 8000 Step/s.

« z » -> Stop


Sur le moniteur arduino, penser a faire « Enter » après chaque commande.


    

