#include <stdio.h>

#include "53l4a2_ranging_sensor.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "tof_sensor.h"

#define TIMING_BUDGET (30U)   /* 8 ms < TimingBudget < 200 ms */
#define POLLING_PERIOD (250U) /* refresh rate for polling mode (ms, shall be consistent with TimingBudget value) */

static RANGING_SENSOR_Capabilities_t Capabilities;
static RANGING_SENSOR_ProfileConfig_t Profile;
static int32_t status = 0;
static RANGING_SENSOR_Result_t Result;

/* Private function prototypes -----------------------------------------------*/
static void print_result(RANGING_SENSOR_Result_t *Result);
static int32_t decimal_part(float_t x);

void haltAndBlink(void)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GreenLED_GPIO_Port, GreenLED_Pin);
        HAL_Delay(200);
    }
}

void initTofSensor(void)
{
    // Reset XSHUT (XSDN) pin
    HAL_GPIO_WritePin(GreenLED_GPIO_Port, GreenLED_Pin, GPIO_PIN_RESET);
    HAL_Delay(2);
    HAL_GPIO_WritePin(GreenLED_GPIO_Port, GreenLED_Pin, GPIO_PIN_SET);
    HAL_Delay(2);

    printf("Initialising the 53L4A2 TOF sensor\r\n");
    status = VL53L4A2_RANGING_SENSOR_Init(VL53L4A2_DEV_CENTER);

    if (status == BSP_ERROR_NONE)
    {
        printf("Initialisation successful\r\n");
    }
    else
    {
        printf("Initialisation failed - powering the board off and on usually fixes the issue.\r\n");
        printf("The program will halt here\r\n");
        haltAndBlink();
    }

    // Determine ID and capabilities of the ranging sensor
    uint32_t SensorId;
    VL53L4A2_RANGING_SENSOR_ReadID(VL53L4A2_DEV_CENTER, &SensorId);
    VL53L4A2_RANGING_SENSOR_GetCapabilities(VL53L4A2_DEV_CENTER, &Capabilities);
    printf("VL53L4A2 ranging sensor detected, ID: %lu\r\n", (unsigned long)SensorId);
    printf("Capabilities: %lu targets per zone, %lu zones\r\n",
           (unsigned long)Capabilities.MaxNumberOfTargetsPerZone,
           (unsigned long)Capabilities.NumberOfZones);

    // Set the profile
    Profile.RangingProfile = RS_MULTI_TARGET_MEDIUM_RANGE;
    Profile.TimingBudget = TIMING_BUDGET;
    Profile.Frequency = 0;     /* Induces intermeasurement period, NOT USED for normal ranging */
    Profile.EnableAmbient = 1; /* Enable: 1, Disable: 0 */
    Profile.EnableSignal = 1;  /* Enable: 1, Disable: 0 */
    VL53L4A2_RANGING_SENSOR_ConfigProfile(VL53L4A2_DEV_CENTER, &Profile);

    // Start the sensor
    status = VL53L4A2_RANGING_SENSOR_Start(VL53L4A2_DEV_CENTER, RS_MODE_BLOCKING_CONTINUOUS);
    if (status != BSP_ERROR_NONE)
    {
        printf("Failed to start the TOF sensor (status = %ld). Powering the board off and on usually fixes the issue.\r\n", status);
        haltAndBlink();
    }
}

void testTofUntilButtonPressed(void)
{
    printf("Starting TOF sensor test. Press the blue button to stop.\r\n");

    // Start ranging
    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)
    {
        status = VL53L4A2_RANGING_SENSOR_GetDistance(VL53L4A2_DEV_CENTER, &Result);

        if (status == BSP_ERROR_NONE)
        {
            print_result(&Result);
        }
        else
        {
            printf("Error reading the TOF sensor (status = %ld)\r\n", status);
        }

        HAL_Delay(POLLING_PERIOD);
    }

    printf("TOF sensor test stopped.\r\n");
}

static void print_result(RANGING_SENSOR_Result_t *Result)
{
    uint8_t i;
    uint8_t j;

    for (i = 0; i < RANGING_SENSOR_MAX_NB_ZONES; i++)
    {
        printf("\nTargets = %lu", (unsigned long)Result->ZoneResult[i].NumberOfTargets);

        for (j = 0; j < Result->ZoneResult[i].NumberOfTargets; j++)
        {
            printf("\r\n |---> ");

            printf("Status = %ld, Distance = %5ld mm ",
                   (long)Result->ZoneResult[i].Status[j],
                   (long)Result->ZoneResult[i].Distance[j]);

            if (Profile.EnableAmbient)
                printf(", Ambient = %ld.%02ld kcps/spad",
                       (long)Result->ZoneResult[i].Ambient[j],
                       (long)decimal_part(Result->ZoneResult[i].Ambient[j]));

            if (Profile.EnableSignal)
                printf(", Signal = %ld.%02ld kcps/spad",
                       (long)Result->ZoneResult[i].Signal[j],
                       (long)decimal_part(Result->ZoneResult[i].Signal[j]));
        }
    }
    printf("\r\n");
    fflush(stdout);
}

static int32_t decimal_part(float_t x)
{
    int32_t int_part = (int32_t)x;
    return (int32_t)((x - int_part) * 100);
}

/*
 * Read the distance from the TOF sensor.
 * @return The distance in mm, or 0 if no target is detected.
 * @note We assume that the sensor has a single zone and is only detecting one target.
 */
uint32_t readFirstTargetDistance(void)
{
    status = VL53L4A2_RANGING_SENSOR_GetDistance(VL53L4A2_DEV_CENTER, &Result);

    if (status == BSP_ERROR_NONE)
    {
        if (Result.ZoneResult[0].NumberOfTargets == 0)
            return 0;
        else
            return Result.ZoneResult[0].Distance[0];
    }
    else
    {
        printf("Error reading the TOF sensor (status = %ld)\r\n", status);
        return 0;
    }
}
