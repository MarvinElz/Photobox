
// Setup for ST7565R in SPI mode
/** The chip select pin */
#define GLCD_CS1_DDR     DDRB
#define GLCD_CS1_PORT    PORTB    //connect to D8
#define GLCD_CS1_PIN     PB4
/** The reset pin (this is required and should not be tied high) */
#define GLCD_RESET_DDR   DDRB
#define GLCD_RESET_PORT  PORTB  //connect to D10
#define GLCD_RESET_PIN   PB6
/** The A0 pin, which selects command or data mode */
#define GLCD_A0_DDR      DDRC
#define GLCD_A0_PORT     PORTC    //connect to D5
#define GLCD_A0_PIN      PC6
/** The clock pin */
#define GLCD_SCL_DDR     DDRD
#define GLCD_SCL_PORT    PORTD   //   -> connect to D3
#define GLCD_SCL_PIN     PD0
/** The data pin */
#define GLCD_SDA_DDR     DDRD
#define GLCD_SDA_PORT    PORTD   //  -> connect to D2
#define GLCD_SDA_PIN     PD1
/** Screen width in pixels (tested with 128) */
#define SCREEN_WIDTH 128
/** Screen height in pixels (tested with 64) */
#define SCREEN_HEIGHT 64
/** Define this if your screen is incorrectly shifted by 4 pixels */
#define ST7565_REVERSE
/** By default we only write pages that have changed. Undefine this
if you want less/faster code at the expense of more SPI operations. */
#undef ST7565_DIRTY_PAGES 1
