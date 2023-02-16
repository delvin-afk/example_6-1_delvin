
#include "mbed.h"
#include "arm_book_lib.h"

#include "user_interface.h"

#include "code.h"
#include "siren.h"
#include "smart_home_system.h"
#include "fire_alarm.h"
#include "date_and_time.h"
#include "temperature_sensor.h"
#include "gas_sensor.h"
#include "matrix_keypad.h"
#include "display.h"
#include "servo_motor.h"

//=====[Declaration of private defines]========================================

#define DISPLAY_REFRESH_TIME_MS 1000

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalOut incorrectCodeLed(LED3);
DigitalOut systemBlockedLed(LED2);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

char codeSequenceFromUserInterface[CODE_NUMBER_OF_KEYS];
static char codeSequence[CODE_NUMBER_OF_KEYS] = { '1', '8', '0', '5' };


//=====[Declaration and initialization of private global variables]============

static bool incorrectCodeState = OFF;
static bool systemBlockedState = OFF;

static bool codeComplete = false;
static int numberOfCodeChars = 0;

//=====[Declarations (prototypes) of private functions]========================

static void userInterfaceMatrixKeypadUpdate();
static void incorrectCodeIndicatorUpdate();
static void systemBlockedIndicatorUpdate();

static void userInterfaceDisplayInit();
static void userInterfaceDisplayUpdate();


//=====[Implementations of public functions]===================================

void userInterfaceInit()
{
    incorrectCodeLed = OFF;
    systemBlockedLed = OFF;
    matrixKeypadInit( SYSTEM_TIME_INCREMENT_MS );
    userInterfaceDisplayInit();
}

void userInterfaceUpdate()
{
    userInterfaceMatrixKeypadUpdate();
    incorrectCodeIndicatorUpdate();
    systemBlockedIndicatorUpdate();
    userInterfaceDisplayUpdate();
}

bool incorrectCodeStateRead()
{
    return incorrectCodeState;
}

void incorrectCodeStateWrite( bool state )
{
    incorrectCodeState = state;
}

bool systemBlockedStateRead()
{
    return systemBlockedState;
}

void systemBlockedStateWrite( bool state )
{
    systemBlockedState = state;
}

bool userInterfaceCodeCompleteRead()
{
    return codeComplete;
}

void userInterfaceCodeCompleteWrite( bool state )
{
    codeComplete = state;
}

static void userInterfaceMatrixKeypadUpdate()
{
    static int numberOfHashKeyReleased = 0;
    char keyReleased = matrixKeypadUpdate();

    // sets and initiallizes variables

    static int nTries = 3;
    static int initTries = 1;
    static int x_position = 11;
    static int position_pressed = 0;
    static char pressed[5] = {' ', ' ', ' ', ' ', '\0'};

    // prints the  number of tries to the display screen
    char str1[10];
    sprintf(str1, "%d", nTries);
    displayCharPositionWrite(10, 1);
    displayStringWrite(str1);
    initTries = initTries - 1;

// checks if there are still any tries, if not, display system blocked
// and waits for the system to be reset
    if (nTries == 0) {
    displayInit();
    while (true) {
        displayCharPositionWrite(0, 0);
        displayStringWrite("System Blocked");
    }
}
   
// if there are still any tries, stores the sequence of input keys from keypad matrix
// displays each character on the LCD screen as it is being entered
    if (nTries > 0)
    {
        if (keyReleased != '\0') 
        {
            pressed[position_pressed] = keyReleased;
            position_pressed++;

            displayCharPositionWrite(x_position, 0);
            char strng3[2];
            strng3[0] = keyReleased;
            strng3[1] = '\0';
            displayStringWrite(strng3);
            x_position++;

// checks if code is completed, and if the entered code is true
        if(position_pressed == 4){
        bool codeTrue = true;
            for(int i = 0; i < 4; i++){
                if(pressed[i] != codeSequence[i]){
                    codeTrue = false;
                    break; // exit loop early if a mismatch is found
                }
        } // clears screens, displays message if the code is correct
        // resets the screen back to initial display after the gate opens
            if (codeTrue){
                clearScreen();
                displayCharPositionWrite(0,0);
                displayStringWrite("Unlocked:Pass"); 
                openGate();  // open the gate to 90 degrees, delays for 5 seconds before shutting down. 
                nTries = 3;
                position_pressed = 0;
                x_position = 11;
                char emptyArray[5] = {' ', ' ', ' ', ' ', '\0'};
                for(int i = 0; i < 4; i++){
                    displayCharPositionWrite(x_position+i, 0);
                    displayStringWrite(emptyArray);
                }
                clearScreen();
                displayCharPositionWrite(0, 0);
                displayStringWrite("Enter Code:");
                displayCharPositionWrite(0, 1);
                displayStringWrite("nTries:");
                
            } else {
                // clears keypad input and start over
                clearScreen();
                displayCharPositionWrite(4, 0);
                displayStringWrite("Wrong Code!");
                delay(1000);
                clearScreen();
                position_pressed = 0;
                x_position = 11;
                char empty[5] = {' ', ' ', ' ', ' ', '\0'};
                for(int i = 0; i < 4; i++){
                    displayCharPositionWrite(x_position+i, 0);
                    displayStringWrite(empty);

                displayCharPositionWrite(0, 0);
                displayStringWrite("Enter Code:");
                displayCharPositionWrite(0, 1);
                displayStringWrite("triesLeft:");
               
            }
            nTries--;
//             if (nTries == 0) {
//                 displayInit();
//                 while (true) {
//                     displayCharPositionWrite(0, 0);
//                     displayStringWrite("System Blocked");
//     }
// }
            }

        }

        if (sirenStateRead() && !systemBlockedStateRead())
        {
            if (!incorrectCodeStateRead())
            {
                codeSequenceFromUserInterface[numberOfCodeChars] = keyReleased;
                numberOfCodeChars++;
                if (numberOfCodeChars >= CODE_NUMBER_OF_KEYS)
                {
                    codeComplete = true;
                    numberOfCodeChars = 0;
                }
            }
            else
            {
                if (keyReleased == '#')
                {
                    numberOfHashKeyReleased++;
                    if (numberOfHashKeyReleased >= 2)
                    {
                        numberOfHashKeyReleased = 0;
                        numberOfCodeChars = 0;
                        codeComplete = false;
                        incorrectCodeState = false;


                    }
                }
            }
        }
    }
    }
}

    

static void userInterfaceDisplayInit()
{
    displayInit();
     
    displayCharPositionWrite ( 0,0 );
    displayStringWrite( "Enter code:" );

    displayCharPositionWrite ( 0,1 );
    displayStringWrite( "Num Tries:" );
    
 }

static void userInterfaceDisplayUpdate()
{
    static int accumulatedDisplayTime = 0;
    
    if( accumulatedDisplayTime >=
        DISPLAY_REFRESH_TIME_MS ) {

        accumulatedDisplayTime = 0;


    } else {
        accumulatedDisplayTime =
            accumulatedDisplayTime + SYSTEM_TIME_INCREMENT_MS;        
    } 
}


static void incorrectCodeIndicatorUpdate()
{
    incorrectCodeLed = incorrectCodeStateRead();
}

static void systemBlockedIndicatorUpdate()
{
    systemBlockedLed = systemBlockedState;
}

