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

#define FIFO_SIZE 32
static uint16_t queue[FIFO_SIZE];
static uint8_t head = 0, tail = 0;
static uint8_t next(uint8_t place);

/** FIFO_Read()
 * Fetch a data value from the FIFO
 * returns: first value in the queue.
 */
 uint16_t FIFO_Read(){
	 uint16_t value = 0x0000;
	 // queue not empty
	 if(head != tail){
		 value = queue[head];
		 head = next(head);
	 } return value; 
 }
 
 /** FIFO_Write()
 * Write a data value to the FIFO
 * returns: success if value written into the queue.
 */
 bool FIFO_Write(uint16_t data){
	 uint16_t place = next(tail);
	 // queue not full
	 if(place != tail){
		 queue[tail] = data;
		 tail = place;
		 return true;
	 } // queue full
	 else{ 
		return false; 
	 }
 }
 
 /** FIFO_Write()
  * Check if the fifo is empty
  * returns: true if the FIFO is empty
  */
 bool FIFO_Empty(){
	 return head == tail;
 }
 
 /** next queue position */
 static uint8_t next(uint8_t place){
	 if(place == 0){
		 place = FIFO_SIZE;
	 } return place - 1;
 }
