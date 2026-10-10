#include "lcd.h"
#include "gpio.h"

/* ATmega2560 PORT A registers */
#define DDRA_REG  (*(volatile uint8_t *)0x21)
#define PORTA_REG (*(volatile uint8_t *)0x22)

/* LCD connections: Arduino Mega D22-D27 */
#define LCD_RS  0       /* PA0 / D22 */
#define LCD_E   1       /* PA1 / D23 */
#define LCD_D4  2       /* PA2 / D24 */
#define LCD_D5  3       /* PA3 / D25 */
#define LCD_D6  4       /* PA4 / D26 */
#define LCD_D7  5       /* PA5 / D27 */

#define LCD_DATA_MASK  0x3CU
#define LCD_PIN_MASK   0x3FU

static void LCD_DelayShort(void)
{
    volatile unsigned int i;

    for (i = 0; i < 500U; i++)
    {
    }
}

static void LCD_DelayLong(void)
{
    volatile unsigned int i;

    for (i = 0; i < 5000U; i++)
    {
    }
}

static void LCD_Enable(void)
{
    PORTA_REG |= (1U << LCD_E);
    LCD_DelayShort();

    PORTA_REG &= (uint8_t)~(1U << LCD_E);
    LCD_DelayShort();
}

static void LCD_Send4Bits(uint8_t data)
{
    /* Clear only PA2-PA5 */
    PORTA_REG &= (uint8_t)~LCD_DATA_MASK;

    if (data & 0x01U)
        PORTA_REG |= (1U << LCD_D4);

    if (data & 0x02U)
        PORTA_REG |= (1U << LCD_D5);

    if (data & 0x04U)
        PORTA_REG |= (1U << LCD_D6);

    if (data & 0x08U)
        PORTA_REG |= (1U << LCD_D7);

    LCD_Enable();
}

static void LCD_SendCommand(uint8_t command)
{
    PORTA_REG &= (uint8_t)~(1U << LCD_RS);

    LCD_Send4Bits((uint8_t)(command >> 4));
    LCD_Send4Bits(command & 0x0FU);

    if (command == 0x01U || command == 0x02U)
        LCD_DelayLong();
    else
        LCD_DelayShort();
}

void LCD_Init(void)
{
    /* PA0-PA5 are LCD outputs */
    DDRA_REG |= LCD_PIN_MASK;

    /* Initialize LCD control and data outputs LOW */
    PORTA_REG &= (uint8_t)~LCD_PIN_MASK;

    /* Allow LCD to power up */
    for (volatile unsigned long i = 0; i < 250000UL; i++)
    {
    }

    PORTA_REG &= (uint8_t)~(1U << LCD_RS);
    PORTA_REG &= (uint8_t)~(1U << LCD_E);

    /* HD44780 4-bit initialization sequence */
    LCD_Send4Bits(0x03U);
    LCD_DelayLong();

    LCD_Send4Bits(0x03U);
    LCD_DelayLong();

    LCD_Send4Bits(0x03U);
    LCD_DelayLong();

    LCD_Send4Bits(0x02U);
    LCD_DelayLong();

    /* 4-bit, 2-line, 5x8 font */
    LCD_SendCommand(0x28U);

    /* Display OFF */
    LCD_SendCommand(0x08U);

    /* Clear display */
    LCD_SendCommand(0x01U);

    /* Increment cursor, no display shift */
    LCD_SendCommand(0x06U);

    /* Display ON, cursor OFF, blink OFF */
    LCD_SendCommand(0x0CU);
}

void LCD_Clear(void)
{
    LCD_SendCommand(0x01U);
}

void LCD_SetCursor(uint8_t row, uint8_t column)
{
    uint8_t address;

    if (row > 1U || column > 15U)
        return;

    if (row == 0U)
        address = column;
    else
        address = (uint8_t)(0x40U + column);

    LCD_SendCommand(0x80U | address);
}

void LCD_WriteChar(char data)
{
    PORTA_REG |= (1U << LCD_RS);

    LCD_Send4Bits((uint8_t)data >> 4);
    LCD_Send4Bits((uint8_t)data & 0x0FU);

    LCD_DelayShort();
}

void LCD_WriteString(const char *str)
{
    if (str == 0)
        return;

    while (*str != '\0')
    {
        LCD_WriteChar(*str);
        str++;
    }
}

void LCD_PrintNumber(unsigned int number)
{
    char buffer[5];
    uint8_t i = 0;

    if (number == 0U)
    {
        LCD_WriteChar('0');
        return;
    }

    while (number > 0U && i < sizeof(buffer))
    {
        buffer[i++] = (char)('0' + (number % 10U));
        number /= 10U;
    }

    while (i > 0U)
    {
        LCD_WriteChar(buffer[--i]);
    }
}

void LCD_PrintDistance(unsigned int distance)
{
    LCD_SetCursor(0, 0);
    LCD_WriteString("Distance:       ");

    LCD_SetCursor(1, 0);

    if (distance == 0U)
    {
        LCD_WriteString("No Echo         ");
    }
    else
    {
        LCD_PrintNumber(distance);
        LCD_WriteString(" cm             ");
    }
}

void LCD_PrintIR(uint8_t status)
{
    LCD_SetCursor(0, 0);
    LCD_WriteString("IR STATUS:      ");

    LCD_SetCursor(1, 0);

    if (status == 0U)
        LCD_WriteString("OBJECT DETECTED ");
    else
        LCD_WriteString("NO OBJECT       ");
}
