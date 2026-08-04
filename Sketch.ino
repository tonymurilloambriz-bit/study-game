#include <Arduboy2.h>
#include <EEPROM.h>       
#include "effects.h"      
#include "music.h"
#include <ArduboyFX.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled); 

// GAME STATES: We now read the question first, then answer it
enum QuizState { STATE_READ_QUESTION, STATE_CHOOSE_ANSWER, STATE_RESULT, STATE_FINAL_SCORE };
QuizState currentState = STATE_READ_QUESTION;

int currentQuestionIndex = 0;
int score = 0;
int highScore = 0;        
char chosenAnswer = ' ';
bool isCorrect = false;

// Timer variables
int timerMax = 300;       // 10 seconds to choose an answer
int timerCurrent = 300;   

// Simple layout variables
const char* currentQ = "";
const char* currentA = "";
const char* currentB = "";
const char* currentC = "";
char correctAns = ' ';

const int EEPROM_HIGH_SCORE_ADDR = 100;

// ==========================================
// YOUR LONG PARAGRAPH QUESTIONS DATA
// ==========================================
void loadQuestion(int index) {
  switch(index) {
    
    case 0:
      currentQ = "First is to last as beginning is to?";
      currentA = "^ entrance";      
      currentB = "> last";     
      currentC = "v end";   
      correctAns = 'C';          
      break;
      

      case 1:
      currentQ = "Dirty is to clean as interested is to ?";
      currentA = "^ bored";  
      currentB = "> certain"; 
      currentC = "v funny";    
      correctAns = 'A';          
      break;

      case 2:
      currentQ = "Hair is to head as leaves are to  ?";
      currentA = "^ grow";  
      currentB = "> green"; 
      currentC = "v tree";    
      correctAns = 'C';   
      break;

      case 3:
       currentQ = "Finger is to hand as toe is to ?";
      currentA = "^ foot";  
      currentB = "> nail"; 
      currentC = "v skin";    
      correctAns = 'A';   
      break;    

      case 4:
      currentQ = "Elevator is to lift a scale is to ?";
      currentA = "^ pounds";  
      currentB = "> machine"; 
      currentC = "v weigh";    
      correctAns = 'C';   
      break;    

       

      case 6:
      currentQ = "Desk is to chair as jacket is to ?";
      currentA = "^ tie";  
      currentB = "> torso"; 
      currentC = "v pants";    
      correctAns = 'C';   
      break;   

      case 7:
      currentQ = "Bill is taller than Ingrid Ingrid is taller than Charles Charles is taller than Bill if the first two statements are true the third is  ?";
      currentA = "^ true";  
      currentB = "> false"; 
      currentC = "v uncertain";    
      correctAns = 'B';   
      break;  

      
      case 9:
      currentQ = "Gerald has blonde hair Susan has blonde hair. All the kids in General's class have blonde hair. If the first two statements are true the third is.   ?";
      currentA = "^ true";  
      currentB = "> false"; 
      currentC = "v uncertain";    
      correctAns = 'C';   
      break;  

      

      case 10:
      currentQ = "Which word does not belong with the others";
      currentA = "^ Run";  
      currentB = "> Leap"; 
      currentC = "v sit";    
      correctAns = 'C';   
      break;  

      case 11:
      currentQ = "Which word does not belong with the others";
      currentA = "^ Eraser ";  
      currentB = "> Marker"; 
      currentC = "v pen";    
      correctAns = 'A';   
      break;  

      case 13:
      currentQ = "Which word does not belong with the others";
      currentA = "^ Earth";  
      currentB = "> Moon"; 
      currentC = "v venus";    
      correctAns = 'B';   
      break;  

    

      case 15:
      currentQ = "Principal most nearly means ";
      currentA = "^ school leader";  
      currentB = "> elected offical"; 
      currentC = "v ruler";    
      correctAns = 'B';   
      break; 

      case 16:
      currentQ = "Contmept most nearly means ";
      currentA = "^ passoin";  
      currentB = "> hatred"; 
      currentC = "v confusion";    
      correctAns = 'C';   
      break; 

      case 17:
      currentQ = "Absurd most nearly means ";
      currentA = "^ convincing";  
      currentB = "> silly"; 
      currentC = "v weak";    
      correctAns = 'B';   
      break; 

      
      
  }
}

#define TOTAL_QUESTIONS 18

void setup() {
  arduboy.begin();
  arduboy.audio.on(); 
  arduboy.setFrameRate(30);
  arduboy.setTextWrap(true); // CRITICAL: This allows paragraph wrapping!
  
  highScore = EEPROM.read(EEPROM_HIGH_SCORE_ADDR);
  if(highScore > TOTAL_QUESTIONS) { highScore = 0; } 
  
  loadQuestion(currentQuestionIndex); 
}

void loop() {
  if (!arduboy.nextFrame()) return;
  arduboy.pollButtons();
  arduboy.clear();

  // Progress Bar tracking your progress at the bottom row of pixels
  if (currentState != STATE_FINAL_SCORE) {
    int barWidth = (currentQuestionIndex * 128) / TOTAL_QUESTIONS;
    arduboy.drawFastHLine(0, 63, barWidth, WHITE);
  }

  switch (currentState) {

    case STATE_READ_QUESTION:
      // 1. Show ONLY the long paragraph question cleanly
      arduboy.setCursor(0, 0);
      arduboy.print(currentQ);
      
      // Prompt at the bottom
      arduboy.setCursor(0, 52); 
      arduboy.print("B: See Choices");

      if (arduboy.justPressed(B_BUTTON)) {
        timerCurrent = timerMax; // Reset arcade timer right before choices load
        currentState = STATE_CHOOSE_ANSWER;
      }
      break;

    case STATE_CHOOSE_ANSWER:
      // 2. Start countdown timer
      if (timerCurrent > 0) {
        timerCurrent--;
      } else {
        chosenAnswer = 'X'; 
        isCorrect = false;
        sound.tones(blip);
        currentState = STATE_RESULT;
      }

      // Draw the shrinking timer bar at the top
      arduboy.fillRect(0, 0, (timerCurrent * 128) / timerMax, 3, WHITE);

      // Display the target question at the top (abbreviated or wrapped)
      arduboy.setCursor(0, 6);
      arduboy.print("Select your answer:");
      
      // Display choices clearly in the center of the screen
      arduboy.setCursor(0, 20); arduboy.print(currentA);
      arduboy.setCursor(0, 32); arduboy.print(currentB);
      arduboy.setCursor(0, 44); arduboy.print(currentC);

      if (arduboy.justPressed(UP_BUTTON))    { chosenAnswer = 'A'; sound.tones(hit); currentState = STATE_RESULT; }
      if (arduboy.justPressed(RIGHT_BUTTON)) { chosenAnswer = 'B'; sound.tones(hit); currentState = STATE_RESULT; }
      if (arduboy.justPressed(DOWN_BUTTON))  { chosenAnswer = 'C'; sound.tones(hit); currentState = STATE_RESULT; }
      
      if (currentState == STATE_RESULT && chosenAnswer != 'X') {
        if (chosenAnswer == correctAns) { isCorrect = true; score++; } else { isCorrect = false; }
      }
      break;

    case STATE_RESULT:
      arduboy.setCursor(24, 6);
      if (isCorrect) {
        arduboy.print("CORRECT!");
        arduboy.drawRect(4, 4, 12, 12, WHITE);
        arduboy.drawLine(6, 10, 9, 13, WHITE);
        arduboy.drawLine(9, 13, 13, 6, WHITE);
      } else {
        if (chosenAnswer == 'X') { arduboy.print("TIME'S UP!"); } 
        else { arduboy.print("WRONG!"); }
        
        arduboy.drawRect(4, 4, 12, 12, WHITE);
        arduboy.drawLine(6, 6, 14, 14, WHITE);
        arduboy.drawLine(14, 6, 6, 14, WHITE);
        
        arduboy.setCursor(0, 22);
        arduboy.print("Correct target:");
        arduboy.setCursor(0, 34);
        if (correctAns == 'A') arduboy.print("UP Button");
        if (correctAns == 'B') arduboy.print("RIGHT Button");
        if (correctAns == 'C') arduboy.print("DOWN Button");
      }

      arduboy.setCursor(0, 52);
      arduboy.print("B: Continue");

      if (arduboy.justPressed(B_BUTTON)) {
        currentQuestionIndex++;
        if (currentQuestionIndex >= TOTAL_QUESTIONS) {
          if (score > highScore) {
            highScore = score;
            EEPROM.write(EEPROM_HIGH_SCORE_ADDR, highScore);
          }
          currentState = STATE_FINAL_SCORE;
        } else {
          loadQuestion(currentQuestionIndex); 
          currentState = STATE_READ_QUESTION; // Loop back to the next long question
        }
      }
      break;

    case STATE_FINAL_SCORE:
      arduboy.setCursor(0, 2);
      arduboy.print("QUIZ FINISHED!");
      
      arduboy.setCursor(0, 18);
      arduboy.print("Your Score: "); arduboy.print(score); arduboy.print("/"); arduboy.print(TOTAL_QUESTIONS);
      
      arduboy.setCursor(0, 32);
      arduboy.print("HI-SCORE: "); arduboy.print(highScore); arduboy.print("/"); arduboy.print(TOTAL_QUESTIONS);

      if (score == TOTAL_QUESTIONS) {
        arduboy.setCursor(0, 44);
        arduboy.print("PERFECT BRAIN!");
      }

      arduboy.setCursor(0, 54);
      arduboy.print("A: Restart");

      if (arduboy.justPressed(A_BUTTON)) {
        currentQuestionIndex = 0;
        score = 0;
        loadQuestion(currentQuestionIndex);
        currentState = STATE_READ_QUESTION;
      }
      break;
  }

  arduboy.display();
}