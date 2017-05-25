function [val1,dval]=def_fn(x)

%% part-a
% f=x^2-1;
% df=2*x;
%% part-b
% f=x^2-2*x-1;
% df=2*x-2;
%% part-c
f=tan(x);      
df= sec(x)*sec(x) ;
%% part-d
% f=sin(x)-cos(2*x);
% df=cos(x)+2*sin(2*x);
%% part-e
% f=x^3-7*x^2+11*x-5;
% df=3*x^2-14*x+11;
%% part-f
% f=sin(x)-cos(x);
% df=cos(x)+sin(x);
%% part-g
% f=x^4*cos(1/x)+2*x^4;
% df=-x^4*sin(1/x)*(1/x^2)+cos(1/x)*4*x^3+8*x^3;


val1=f;
dval=df;
end