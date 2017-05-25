 format long
 eps_abs = 1e-10;
 eps_step = 1e-10;
 a = 5.5;
 b = 6.5 ;
 if ( f(a) * f(b) > 0 )
        error( 'f(a) and f(b) do not have opposite signs' );
    end
while (b - a >= eps_step || ( abs( f(a) ) >= eps_abs && abs( f(b) )  >= eps_abs ) )
    c = (a + b)/2;
    if ( f(c) == 0 )
       break;
    elseif ( f(a)*f(c) < 0 )
       b = c;
    else
       a = c;
    end
end