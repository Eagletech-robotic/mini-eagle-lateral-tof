#include <stdint.h>

#include "tof_sensor.h"
#include "motor_control.h"

void mainLoop(void)
{
    while (1)
    {
        uint32_t distance = readFirstTargetDistance();

        if (distance == 0)
            printf("No target detected\r\n");
        else
            printf("Distance: %d mm\r\n", distance);

        // If the distance is 100 mm or more, move the robot forward
        if (distance >= 100)
        {
            setMotor(MOTOR_LEFT, 1.0);
            setMotor(MOTOR_RIGHT, 1.0);
        }
        // Otherwise, turn left
        else
        {
            setMotor(MOTOR_LEFT, -0.5);
            setMotor(MOTOR_RIGHT, 0.5);
        }

        HAL_Delay(100);
    }
}