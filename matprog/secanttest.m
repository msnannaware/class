format long
root = secant(@sin(x/2)-1,[2.0;2.05],1.e-10,pi)
%root = secant(@fpol,1.0,2, 1.e-10,1.1)