/** FIFO.c
 * FIFO Sampling data buffer
 * Non-blocking queue.
 * queue flows right to left
 * 
 * Author: Ronald Macmaster
 * Date: 11/07/2016
 */
#include <stdint.h>
#include <stdbool.h>

/** FIFO_Read()
 * Fetch a data value from the FIFO
 * returns: first value in the queue.
 */
 uint16_t FIFO_Read(void);
 
 /** FIFO_Write()
 * Write a data value to the FIFO
 * returns: success if value written into the queue.
 */
 bool FIFO_Write(uint16_t data);

 /** FIFO_Empty()
  * Check if the fifo is empty
  * returns: true if the FIFO is empty
  */
 bool FIFO_Empty(void);
