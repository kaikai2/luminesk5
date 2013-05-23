#ifndef __OGL2D_BASE_MATH_H_
#define __OGL2D_BASE_MATH_H_

#define IS_NOT_POWER_OF_2(a) ((a)-1 & (a))

#define IS_POWER_OF_2(a) (!IS_NOT_POWER_OF_2(a))

#define max(a,b)            (((a) > (b)) ? (a) : (b))

#define min(a,b)            (((a) < (b)) ? (a) : (b))



#endif // __OGL2D_BASE_MATH_H_