# Delvin Marimo and Daniel


## Project Description

To the Home security System, we added a new functionality that opens the gate if the correct passcode is entered.
The user interacts with the system using a keypad matrix, and LCD display. If correct code is entered, the screen displays that the gate is
unlocked you can pass. The gate opens(rotating a servo motor 90 degrees) for 5 seconds before closes. if wrong code is entered, three attempts are allowed
before the system is locked out. 

## Functionality

### correct code

if 1805 is entered, automatically the screen displays that the gate is unlocked pass,  the gate is opened. Wait for 5 seconds before it closes
The system is initiliazed ready to accept code fromt the next user

### Incorrect code

if an incorrect code is entered, the system automatically clears the input and allows the user to try again. 

### After three failed attempts
The system is blocked and a respective message is displayed on the screen

