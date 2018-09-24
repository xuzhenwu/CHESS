#include <math.h>
#include <stdio.h>


float spaveg(float value1, float value2){
		float value;
		float value_sin,value_cos;
		const double RtoD=57.29577951;
            
		value_sin=(float)100.*sin(value1)*sin(value2);
		value_cos=(float)100.*cos(value1)*sin(value2);
		
		value = (float)atan(value_sin/value_cos)*(float)RtoD;
	
		if (value_cos < 0.0)
			value =  value + 180 ;
		else if (value_sin < 0.0)
			value = value + 360;
			else
			value = value;

		value = 360 -value;

		if (value <= 270)
			value += 90;
		else 
			value -= 270;
		return(value);
}