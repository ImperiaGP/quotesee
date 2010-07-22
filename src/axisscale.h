#ifndef AXISSCALE_H
#define AXISSCALE_H

class AxisScale
{
public:
    AxisScale();
    static void DefineAxis(double *Min, double *Max, double *Inc)
    {
        // define local variables...

        double Test_inc,              // candidate increment value
               Test_min,              // minimum scale value
               Test_max,              // maximum scale value
               Range = *Max - *Min ;  // range of data

        int i = 0 ;                   // counter

        // don't create problems -- solve them

        if( Range < 0 ) {
           *Inc = 0 ;
           return ;
           }

        // handle special case of repeated values

        else if( Range == 0) {
                *Min = ceil(*Max) - 1 ;
                *Max = *Min + 1 ;
                *Inc = 1 ;
                return ;
                }

        // compute candidate for increment

        Test_inc = pow10( ceil( log10( Range/10 ) ) ) ;

        // establish maximum scale value...

        Test_max = ( (long)(*Max / Test_inc)) * Test_inc ;

        if( Test_max < *Max )
           Test_max += Test_inc ;

        // establish minimum scale value...

        Test_min = Test_max ;

        do {
           ++i ;
           Test_min -= Test_inc ;
           }
        while( Test_min > *Min ) ;

        // subtracting small values can screw up the scale limits,
        // eg: if DefineAxis is called with (min,max)=(0.01, 0.1),
        // then the calculated scale is 1.0408E17 TO 0.05 BY 0.01.
        // the following if statment corrects for this...

        //if( fabs(Test_min) < 1E-10 )
        //   Test_min = 0 ;

        // adjust for too few tick marks

        if( i < 6 ) {
           Test_inc /= 2 ;
           if( (Test_min + Test_inc) <= *Min )
              Test_min += Test_inc ;
           if( (Test_max - Test_inc) >= *Max )
              Test_max -= Test_inc ;
           }

        // pass back axis definition to caller

        *Min = Test_min ;
        *Max = Test_max ;
        *Inc = Test_inc ;
    }
};

#endif // AXISSCALE_H
