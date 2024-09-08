#include <stdint.h>
#include <stdio.h>

#include "tof_sensor.h"
#include "motor_control.h"
#include "stm32f4xx_hal.h"

#define CYCLE_DURATION_MS 20

struct Sensors
{
    uint32_t distance;
};

struct Decision
{
    float leftMotorSpeed;
    float rightMotorSpeed;
};

void actuateMotors(struct Decision *decision)
{
    setMotor(MOTOR_LEFT, decision->leftMotorSpeed);
    setMotor(MOTOR_RIGHT, decision->rightMotorSpeed);
}

void readSensors(struct Sensors *sensors)
{
    sensors->distance = readFirstTargetDistance();
}

void decide(struct Sensors *sensors, struct Decision *decision)
{
    if (sensors->distance == 0)
    {
        printf("No target detected\r\n");
    }
    else
    {
        printf("Distance: %lu mm\r\n", sensors->distance);
        if (sensors->distance >= 100)
        {
            // If the distance is 100 mm or more, move the robot forward
            decision->leftMotorSpeed = 1.0;
            decision->rightMotorSpeed = 1.0;
        }
        else
        {
            // Otherwise, turn left
            decision->leftMotorSpeed = -0.5;
            decision->rightMotorSpeed = 0.5;
        }
    }
}

uint32_t waitForEndOfCycle(uint32_t cycleStartTick)
{
    // Wait until the end of the cycle, and print a warning if it took too long
    if (HAL_GetTick() >= cycleStartTick + CYCLE_DURATION_MS)
    {
        printf("Cycle took too long: %lu ms > %d ms - tick = %lu\r\n", HAL_GetTick() - cycleStartTick, CYCLE_DURATION_MS, HAL_GetTick());
        return HAL_GetTick();
    }
    else
    {
        printf("Cycle took %lu ms - tick = %lu\r\n", HAL_GetTick() - cycleStartTick, HAL_GetTick());
        while (HAL_GetTick() < cycleStartTick + CYCLE_DURATION_MS)
            ;
        return cycleStartTick + CYCLE_DURATION_MS;
    }
}

void mainLoop(void)
{
    uint32_t cycleStartTick = HAL_GetTick();
    struct Decision decision = {
        .leftMotorSpeed = 0.0,
        .rightMotorSpeed = 0.0,
    };
    struct Sensors sensors;

    // The main loop of the robot.
    while (1)
    {
        // 1. Actuate the motors according to the last decision. The motors will keep moving for the whole cycle.
        actuateMotors(&decision);

        // 2. Read the sensors.
        readSensors(&sensors);

        // 3. Decide what to do on the next cycle.
        decide(&sensors, &decision);

        // 4. Wait until the end of the cycle.
        cycleStartTick = waitForEndOfCycle(cycleStartTick);
    }
}
