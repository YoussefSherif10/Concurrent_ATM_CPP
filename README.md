# ATM System

This project is an ATM system that is handling interaction with the person trying to withdraw money and interaction with the bank. as well as controls the physical machinery to accept the card and display the appropriate messages on the screen, handle key presses, issue money, and return the card.

Here is a finite state machine that demonstrates the function of the system.
![](https://github.com/YoussefSherif10/Concurrent_ATM_CPP/blob/main/c9fb8fb3-3d35-410c-90cb-11d31f4e6087.jpg?raw=true)

the system is split into three independent threads: one to handle the physical machinery, one to handle the ATM logic, and one to communicate with the bank.
These threads communicate purely by passing messages rather than sharing data.

The system waits for the card to be inserted. then waits for the user to enter his PIN one digit at a time. they can delete the last digit. Once enough digits are entered, the PIN is verified. If it isn't right then the card returns to the user. Otherwise, wait for selecting the withdrawal amount. then, wait for bank confirmation to either display not enough funds or to issue the cash and return the card.

Having designed the state machine, we can implement it using a class in which each member function represents a state. Each function waits for sets of incoming messages to handle them and possibly trigger a switch to the next state. Each distinct message type is represented in a struct.
All the synchronization is entirely hidden from the main program.
All of the synchronization classes are built around a queue that is secured by a mutex and a condition variable and are all contained in one namespace.