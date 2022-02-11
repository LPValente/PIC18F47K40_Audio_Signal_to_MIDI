/* Master 
 * 1. The user generates a Start condition by setting the 26.9.3.8 SEN bit.
 * 2. SSPxIF is set by hardware on completion of the Start.
 * 3. SSPxIF is cleared by software.
 * 4. The MSSP module will wait the required start time before any other operation takes place.
 * 5. The user loads the SSPxBUF with the slave address to transmit.
 * 6. Address is shifted out the SDA pin until all eight bits are transmitted. Transmission begins as soon
 * as SSPxBUF is written to.
 * 7. The MSSP module shifts in the ACK bit from the slave device and writes its value into the 26.9.3.2
 * ACKSTAT bit.
 * 8. The MSSP module generates an interrupt at the end of the ninth clock cycle by setting the SSPxIF
 * bit.
 * 9. The user loads the SSPxBUF with eight bits of data.
 * 10. Data is shifted out the SDA pin until all eight bits are transmitted.
 * 11. The MSSP module shifts in the ACK bit from the slave device and writes its value into the 26.9.3.2
 * ACKSTAT bit.
 * 12. Steps 8-11 are repeated for all transmitted data bytes.
 * 13. The user generates a Stop or Restart condition by setting the 26.9.3.6 PEN or 26.9.3.7 RSEN bits.
 * Interrupt is generated once the Stop/Restart condition is complete.
 * 
 * Slave
 * 1. Start bit detected.
 * 2. 26.9.1.5 S bit is set; SSPxIF is set if interrupt on Start detect is enabled.
 * 3. Matching address with 26.9.1.6 R/W bit clear is received.
 * 4. The slave pulls SDA low sending an ACK to the master, and sets SSPxIF bit.
 * 5. Software clears the SSPxIF bit.
 * 6. Software reads received address from SSPxBUF clearing the BF flag.
 * 7. If 26.9.3.8 SEN = 1; Slave software sets 26.9.2.4 CKP bit to release the SCL line.
 * 8. The master clocks out a data byte.
 * 9. Slave drives SDA low sending an ACK to the master, and sets SSPxIF bit.
 * 10. Software clears SSPxIF.
 * 11. Software reads the received byte from SSPxBUF clearing BF.
 * 12. Steps 8-12 are repeated for all received bytes from the master.
 * 13. Master sends Stop condition, setting 26.9.1.4 P bit, and the bus goes idle.
 * 
 */

#include <xc.h>
#include "I2C.h"