// http://widgets.vvo-online.de/abfahrtsmonitor/Abfahrten.do?hst=südhöhe
#include <vector>
#define contrast 0x20

struct line{
  char number[3]; // Liniennummer
  char dest[64];  // Linienziel
  char timeDep[2];// Abfahrzeit(in Minuten)
} typedef line;

vector<line> haltestelle;

// TODO: include Boost or other internet-library http://www.boost.org/
// TODO: try do reach http Webside
// TODO: parse information
// TODO: get Raspberry GPIO Running http://raspberrypiguide.de/howtos/raspberry-pi-gpio-how-to/
// http://www.element14.com/community/servlet/JiveServlet/previewBody/73950-102-4-309126/GPIO_Pi2.png?01AD=3GZgWMTpg1jDSMy2YO8hD1OqtjdCp9XqniBSBHoUhsn1I8J5raM6jOQ&01RI=B51CF70A37220D9&01NA=na

void main(int argc, char** argv){
  
  // initialize screen
  st7565_init(contrast);

  // clear buffer and screen
  st7565_clear();

  // Hello World
  drawstring_P(1, 1, "Hello World");

  // update screen
  st7565_display();

  // main-loop
  while(1){
    // get bus/tram information
    
    // parse information into vector<line>
    
    // prepare information 
  }
}
