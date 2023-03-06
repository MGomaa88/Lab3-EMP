

/**
 * main.c
 */


/**
 * main.c
 */
/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: main.c
*
* PROJECT....: EMP
*
* DESCRIPTION: Design a state mashine program and check the right sequence input. If the sequence is correct then the green led will light
* otherwise the red led.  
*
* Change Log:
******************************************************************************
* Date    Id    Change
* 02/28/2023
* --------------------
* 150215  Mohamed Gomaa    Module created.
*
*****************************************************************************/



/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "systick.h"
#include "emp_type.h"

/*****************************    Defines    *******************************/
#define TIM_1_SEC     200
#define SW2      0x01
#define SW1      0x10


typedef enum {
    LOCKED,
        FIRST,
        SECOND,
        THIRD,
        FOURTH,
        FIFTH,
    CHECK,
    UNLOCKED
} Mode;



/*****************************   Constants   *******************************/

extern int ticks;

int colorArray[8] = {
      0x02, /*Red*/
      0x04, /*Blue*/
      0x0C, /*Cyan*/
      0x0A, /*Yellow*/
      0x06, /*Magenta*/
      0x0E,  /*White*/
      0x08 /*Green*/
};


/*****************************   Functions   *******************************/
INT8U sw1_pushed();
INT8U sw2_pushed();


int main(void)
{
    int alive_timer = TIM_1_SEC;

    init_systick();

    int dummy;
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF; // 1) activate clock for Port F
    dummy = SYSCTL_RCGC2_R;           // allow time for clock to start
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
     GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0

    GPIO_PORTF_DIR_R = 0x0E;           // set the direction as output for LED pins on PortF (PF1 - PF3)
    GPIO_PORTF_DEN_R = 0x1F;
    GPIO_PORTF_PUR_R = 0x11;

    int counter = 0;

    int counter_color=0;
    Mode state = LOCKED;


    while(1)                                    // loop forever
    {


        switch(state)
        {
        case LOCKED:

            counter = 0;
            counter_color = 0;
            GPIO_PORTF_DATA_R = colorArray[counter_color] & 0x0E;
            state = FIRST;
            break;


            case FIRST:

             //   while (!sw1_pushed() && !sw2_pushed());
                if(sw1_pushed())
                     ++counter ;
                     state = SECOND;
                     ++counter_color;
                     GPIO_PORTF_DATA_R = colorArray[counter_color] & 0x0E;
                break;

            case SECOND:
                //while (!sw1_pushed() && !sw2_pushed());
              if(sw2_pushed())
                       ++counter ;
                       state = THIRD;
                       ++counter_color;
                       GPIO_PORTF_DATA_R = colorArray[counter_color] & 0x0E;
                       break;

            case THIRD:
                //while (!sw1_pushed() && !sw2_pushed());
              if(sw2_pushed())
                       ++counter ;
                       state = FOURTH;
                       ++counter_color;
                       GPIO_PORTF_DATA_R = colorArray[counter_color] & 0x0E;
                      break;

            case FOURTH:
               // while (!sw1_pushed() && !sw2_pushed());
                if(sw1_pushed())
                      ++counter ;
                      state = FIFTH;
                      ++counter_color;
                      GPIO_PORTF_DATA_R = colorArray[counter_color] & 0x0E;
                break;

            case FIFTH:
             //   while (!sw1_pushed() && !sw2_pushed());
              if(sw2_pushed())
                 ++counter;
            state = CHECK;
            ++counter_color;
            GPIO_PORTF_DATA_R = colorArray[counter_color] & 0x0E;
            break;

            case CHECK:
                if(counter == 5)
                    state = UNLOCKED;
                else
                    state = LOCKED;
                break;

            case UNLOCKED:
                counter_color++;
                GPIO_PORTF_DATA_R = colorArray[counter_color] & 0x0E;
                if (sw1_pushed() || sw2_pushed())
                state= LOCKED;
                break;


            }
                }



    return 0;
}

INT8U sw1_pushed()
/*****************************************************************************
*   Input    : -
*   Output   : - True or False
*   Function : Check if switch 1 is pushed
******************************************************************************/
{
  //  return( !(GPIO_PORTF_DATA_R & SW1) );

    while(1)
          {
              if( !(GPIO_PORTF_DATA_R & SW2) )
              {
                  while( !(GPIO_PORTF_DATA_R & SW2) ){}; // wait for button release
                  return 0;
              }
              if( !(GPIO_PORTF_DATA_R & SW1) )
                        {
                           while( !(GPIO_PORTF_DATA_R & SW1) ){}; // wait for button release
                            return 1;
                        }
          }
}
INT8U sw2_pushed()
/*****************************************************************************
*   Input    : -
*   Output   : - True or False
*   Function : Check if switch 2 is pushed
******************************************************************************/
{
   // return( !(GPIO_PORTF_DATA_R & SW2) );
    while(1)
       {
           if( !(GPIO_PORTF_DATA_R & SW2) )
           {
               while( !(GPIO_PORTF_DATA_R & SW2) ){}; // wait for button release
               return 1;
           }
           if( !(GPIO_PORTF_DATA_R & SW1) )
                     {
                        while( !(GPIO_PORTF_DATA_R & SW1) ){}; // wait for button release
                         return 0;
                     }
       }



}



