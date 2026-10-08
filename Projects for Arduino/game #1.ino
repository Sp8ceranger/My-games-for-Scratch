/*
Created by Google Search & me (https://github.com/Sp8ceranger/) 
Vertion on Scratch: https://scratch.mit.edu/projects/1388211667/
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

int pinBoutonGauche = 2;
int pinBoutonDroite = 3;
int pinBoutonSaut   = 4;

int joueurX = 0;
int joueurY = 3;
int x2 = 0;
int y2 = 0;
int phaseSaut = 0;
bool enTrainDeSauter = false;
unsigned long tempsEtapeSaut = 0; // Pour gérer la durée du saut en l'air
int score = 0;
String vertion = "1.4.3";

int menu = 0;
int menu2 = 0;

byte trucAatraper[8] = {
  B00000, B00000, B00000, B00100, B01110, B00100, B00000, B00000
};
byte sprite[8] = {
  B00000, B00000, B00100, B01010, B00100, B01110, B00100, B01010
};
byte bouton[8] = {
  B00000, B00000, B01110, B11111, B11111, B11111, B01110, B00000
};
byte fleche[8] = {
  B00000, B00000, B00100, B01000, B10111, B01000, B00100, B00000
};

void setup() {
  lcd.init();
  lcd.backlight();
  
  pinMode(pinBoutonGauche, INPUT_PULLUP);
  pinMode(pinBoutonDroite, INPUT_PULLUP);
  pinMode(pinBoutonSaut, INPUT_PULLUP);
  
  lcd.createChar(0, sprite);
  lcd.createChar(1, bouton);
  lcd.createChar(2, fleche);
  lcd.createChar(3, trucAatraper);

  x2 = random(0, 20);
  y2 = random(1, 4);

  lcd.setCursor(0, 0);
  lcd.print("   Spaceranger &");
  lcd.setCursor(0, 1);
  lcd.print("   Google Search");
  lcd.setCursor(0, 2);
  lcd.print("    presents...");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("- 374 lines of code");
  lcd.setCursor(0, 1);
  lcd.print("- 4 day");
  lcd.setCursor(0, 2);
  lcd.print("- 1 vertion on");
  lcd.setCursor(0, 3);
  lcd.print("  Scratch");
  delay(4000);
  lcd.clear();

  // Premier affichage du menu principal
  lcd.setCursor(0, 0); lcd.print("START");
  lcd.setCursor(0, 1); lcd.print("SETTING");
  lcd.setCursor(0, 3); lcd.print(vertion);
  lcd.setCursor(8, menu); lcd.write(byte(2)); 

  while (true) {
    bool principalAChange = false;

    // --- Navigation via les broches physiques réelles ---
    // Broche 2 (Bouton physique Gauche) -> Descend dans le menu
    if (digitalRead(2) == LOW) {
      menu++;
      if (menu > 1) menu = 0;
      principalAChange = true;
      while(digitalRead(2) == LOW); // Attend relâchement du bouton physique 2
      delay(50);
    }
    // Broche 3 (Bouton physique Droite) -> Monte dans le menu
    if (digitalRead(3) == LOW) {
      menu--;
      if (menu < 0) menu = 1;
      principalAChange = true;
      while(digitalRead(3) == LOW); // Attend relâchement du bouton physique 3
      delay(50);
    }

    if (principalAChange) {
      lcd.setCursor(8, 0); lcd.print(" ");
      lcd.setCursor(8, 1); lcd.print(" ");
      lcd.setCursor(8, menu); lcd.write(byte(2));
    }
    
    // Broche 4 (Bouton physique Saut) -> VALIDER
    if (digitalRead(4) == LOW) {
      while(digitalRead(4) == LOW); // Attend relâchement du bouton physique 4
      delay(100);

      if (menu == 0) {
        break; // Quitte le menu principal -> lance le jeu dans le loop()
      } 
      
      if (menu == 1) {
        // --- ENTRÉE DANS LE SOUS-MENU SETTING ---
        lcd.clear();
        lcd.setCursor(0, 0); lcd.print(".."); 
        lcd.setCursor(0, 1); lcd.print("right : " + String(pinBoutonDroite));
        lcd.setCursor(0, 2); lcd.print("left  : " + String(pinBoutonGauche));
        lcd.setCursor(0, 3); lcd.print("up    : " + String(pinBoutonSaut));
        
        menu2 = 0; 
        lcd.setCursor(11, menu2); lcd.write(byte(2));

        while (true) {
          bool settingAChange = false;

          // Navigation dans SETTING avec les boutons physiques fixes 2 et 3
          if (digitalRead(2) == LOW) {
            menu2++;
            if (menu2 > 3) menu2 = 0;
            settingAChange = true;
            while(digitalRead(2) == LOW); 
            delay(50);
          }
          if (digitalRead(3) == LOW) {
            menu2--;
            if (menu2 < 0) menu2 = 3;
            settingAChange = true;
            while(digitalRead(3) == LOW); 
            delay(50);
          }
          
          if (settingAChange) {
            lcd.setCursor(11, 0); lcd.print(" ");
            lcd.setCursor(11, 1); lcd.print(" ");
            lcd.setCursor(11, 2); lcd.print(" ");
            lcd.setCursor(11, 3); lcd.print(" ");
            lcd.setCursor(11, menu2); lcd.write(byte(2));
          }
          
          // Validation dans SETTING avec le bouton physique fixe 4
          if (digitalRead(4) == LOW) {
            while(digitalRead(4) == LOW); 
            delay(100);
            
            if (menu2 == 0) { 
              break; // Sort proprement de SETTING
            }
            
            lcd.setCursor(11, menu2); lcd.print(" ");
            lcd.setCursor(14, menu2); lcd.write(byte(2)); // Flèche d'édition
            delay(200); 

            // CONFIGURATION DU BOUTON DROITE
            if (menu2 == 1) {
              while (true) {
                if (digitalRead(2) == LOW) { 
                  while(digitalRead(2)==LOW); 
                  if (pinBoutonGauche == 2) pinBoutonGauche = pinBoutonDroite; 
                  if (pinBoutonSaut == 2) pinBoutonSaut = pinBoutonDroite;
                  pinBoutonDroite = 2; break; 
                }
                if (digitalRead(3) == LOW) { 
                  while(digitalRead(3)==LOW); 
                  if (pinBoutonGauche == 3) pinBoutonGauche = pinBoutonDroite;
                  if (pinBoutonSaut == 3) pinBoutonSaut = pinBoutonDroite;
                  pinBoutonDroite = 3; break; 
                }
                if (digitalRead(4) == LOW) { 
                  while(digitalRead(4)==LOW); 
                  if (pinBoutonGauche == 4) pinBoutonGauche = pinBoutonDroite;
                  if (pinBoutonSaut == 4) pinBoutonSaut = pinBoutonDroite;
                  pinBoutonDroite = 4; break; 
                }
              }
            }
            
            // CONFIGURATION DU BOUTON GAUCHE
            if (menu2 == 2) {
              while (true) {
                if (digitalRead(2) == LOW) { 
                  while(digitalRead(2)==LOW); 
                  if (pinBoutonDroite == 2) pinBoutonDroite = pinBoutonGauche; 
                  if (pinBoutonSaut == 2) pinBoutonSaut = pinBoutonGauche;
                  pinBoutonGauche = 2; break; 
                }
                if (digitalRead(3) == LOW) { 
                  while(digitalRead(3)==LOW); 
                  if (pinBoutonDroite == 3) pinBoutonDroite = pinBoutonGauche;
                  if (pinBoutonSaut == 3) pinBoutonSaut = pinBoutonGauche;
                  pinBoutonGauche = 3; break; 
                }
                if (digitalRead(4) == LOW) { 
                  while(digitalRead(4)==LOW); 
                  if (pinBoutonDroite == 4) pinBoutonDroite = pinBoutonGauche;
                  if (pinBoutonSaut == 4) pinBoutonSaut = pinBoutonGauche;
                  pinBoutonGauche = 4; break; 
                }
              }
            }
            
            // CONFIGURATION DU BOUTON SAUT (UP)
            if (menu2 == 3) {
              while (true) {
                if (digitalRead(2) == LOW) { 
                  while(digitalRead(2)==LOW); 
                  if (pinBoutonDroite == 2) pinBoutonDroite = pinBoutonSaut; 
                  if (pinBoutonGauche == 2) pinBoutonGauche = pinBoutonSaut;
                  pinBoutonSaut = 2; break; 
                }
                if (digitalRead(3) == LOW) { 
                  while(digitalRead(3)==LOW); 
                  if (pinBoutonDroite == 3) pinBoutonDroite = pinBoutonSaut;
                  if (pinBoutonGauche == 3) pinBoutonGauche = pinBoutonSaut;
                  pinBoutonSaut = 3; break; 
                }
                if (digitalRead(4) == LOW) { 
                  while(digitalRead(4)==LOW); 
                  if (pinBoutonDroite == 4) pinBoutonDroite = pinBoutonSaut;
                  if (pinBoutonGauche == 4) pinBoutonGauche = pinBoutonSaut;
                  pinBoutonSaut = 4; break; 
                }
              }
            }
            
            // Mise à jour matérielle des broches
            pinMode(pinBoutonGauche, INPUT_PULLUP);
            pinMode(pinBoutonDroite, INPUT_PULLUP);
            pinMode(pinBoutonSaut, INPUT_PULLUP);
            
            // Actualisation de l'affichage
            lcd.setCursor(0, 1); lcd.print("right : " + String(pinBoutonDroite) + "   ");
            lcd.setCursor(0, 2); lcd.print("left  : " + String(pinBoutonGauche) + "   ");
            lcd.setCursor(0, 3); lcd.print("up    : " + String(pinBoutonSaut) + "   ");
            
            lcd.setCursor(14, menu2); lcd.print(" ");
            lcd.setCursor(11, menu2); lcd.write(byte(2));
            
            delay(200); 
          }
          delay(20);
        }
        
        // Réaffichage du menu principal
        lcd.clear();
        lcd.setCursor(0, 0); lcd.print("START");
        lcd.setCursor(0, 1); lcd.print("SETTING");
        lcd.setCursor(0, 3); lcd.print(vertion);
        lcd.setCursor(8, menu); lcd.write(byte(2));
      }
    }
    delay(20);
  }
}


void loop() {
  // Votre jeu démarrera ici une fois qu'on aura appuyé sur START !
  lcd.clear();
  startJeu();
}
void startJeu() {
  lcd.clear();
  joueurX = 0;
  joueurY = 3;
  phaseSaut = 0;

  // Affichage initial du sprite
  lcd.setCursor(joueurX, joueurY);
  lcd.write(byte(0)); 
  lcd.setCursor(x2, y2);
  lcd.write(byte(3)); 
  lcd.setCursor(0, 0);
  lcd.print("score : ");
  lcd.print(score);

    while (true) {
    bool action = false;
    int ancienX = joueurX;
    int ancienY = joueurY;

    // --- Déplacement Gauche ---
    if (digitalRead(pinBoutonGauche) == LOW) {
      joueurX--;
      action = true;
      delay(200); 
    }
    
    // --- Déplacement Droite ---
    if (digitalRead(pinBoutonDroite) == LOW) {
      joueurX++;
      action = true;
      delay(200);
    }

    // CORRECTION ÉTAPE 1 : On gère les bords de l'écran DIRECTEMENT ICI
    if (joueurX == -1) { joueurX = 19; }
    if (joueurX == 20) { joueurX = 0;  }

    // --- IMPULSION DU SAUT ---
    if (digitalRead(pinBoutonSaut) == LOW && phaseSaut == 0) {
      joueurY = 2; 
      phaseSaut = 1;
      tempsEtapeSaut = millis();
      action = true;
    }

    // --- GESTION DE LA TRAJECTOIRE (Montée et Descente) ---
    if (phaseSaut == 1 && (millis() - tempsEtapeSaut > 150)) {
      joueurY = 1;
      phaseSaut = 2;
      tempsEtapeSaut = millis();
      action = true;
    }
    else if (phaseSaut == 2 && (millis() - tempsEtapeSaut > 350)) {
      joueurY = 2;
      phaseSaut = 3;
      tempsEtapeSaut = millis();
      action = true;
    }
    else if (phaseSaut == 3 && (millis() - tempsEtapeSaut > 150)) {
      joueurY = 3;
      phaseSaut = 0; 
      action = true;
    }

    // CORRECTION ÉTAPE 2 : On teste la collision avec la vraie valeur corrigée du joueur
    if (joueurX == x2 && joueurY == y2) {
      // Efface visuellement l'ancienne étoile avant de changer ses coordonnées
      lcd.setCursor(x2, y2);
      lcd.print(" ");

      x2 = random(0, 20);
      y2 = random(1, 4);
      score = score + 1;
      
      // Met à jour le score
      lcd.setCursor(8, 0); // On commence l'écriture après "score : "
      lcd.print(score);
      
      // Dessine la nouvelle étoile
      lcd.setCursor(x2, y2);
      lcd.write(byte(3)); 
    }

    // --- Rafraîchissement de l'écran ---
    if (action) {
      // On efface l'ancienne position
      lcd.setCursor(ancienX, ancienY);
      lcd.print(" "); 

      // On redessine l'étoile si le joueur vient de passer dessus et l'a effacée sans l'attraper
      lcd.setCursor(x2, y2);
      lcd.write(byte(3));

      // On dessine le joueur à sa nouvelle position
      lcd.setCursor(joueurX, joueurY);
      lcd.write(byte(0)); 
    }
    
    delay(20); 
  }
}
