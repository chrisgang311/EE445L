/** Plot.h
 * Plot for data acquisition system.
 * 
 * Author: Ronald Macmaster
 * Date: 11/07/2016
 */
 #include <stdint.h>
  
 /**************Plot_Init()***************
 Specify the plot axes and range of LCD plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
          min   smallest data value allowed, resolution= 0.01
          max   largest data value allowed, resolution= 0.01
 Outputs: none
*/
void Plot_Init(char *title, int32_t min, int32_t max);
 
/** Plot_PrintData()
 * Print the data value in fixed point
 * Special place right above plot for data string
 */
void Plot_PrintData(uint16_t data);


/** Plot_PlotData()
 * Plots the next data point in the sequence.
 * plot pointer is incremented one place
 */
void Plot_PlotData(uint16_t data);
