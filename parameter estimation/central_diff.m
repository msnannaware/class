clear all; clc
%input data
t     = [1862:2:1910];

H     = 10*[.3 .85 14.8 .6 .9 2.5 5.0 9.0 7.0 1.0 1.1 4.2 13.0 5.0 1.8 4.0 7.8 3.5 .5 1.0 .5 5.8 3.6 2.3 3.0];
L     = 10*[.3 1.8 4.3 6.2 1.0 .8 3.0 4.7 4.2 1.2 1.3 3.5 7.2 3.0 2.2 1.5 4.0 3.6 2.3 .8 1.0 2.0 5.8 4.0 1.0];
%initialize
p=0
q=0
f=0
g=0

%a,b,r,c are the parameters in lotka-voltera's model
%delx =~dx/dt %dely =~ dy/dt %x=H, y=L
%i denotes the place from where we do the central difference, i=(10:18)
syms a b r c;
for i= 8:16;  % 8 to 16 is taken to avoid extreme values 
    %sll eqns converted to ax+by+c=0
equ1 = ((H(i+1)-H(i-1))/4)-(a*H(i)-b*H(i)*L(i));  
equ2 = ((L(i+1)-L(i-1))/(t(i+1)-t(i-1)))-(-r*L(i)+c*H(i)*L(i));
i=i+1;
equ3 = ((H(i+2)-H(i))/(t(i+2)-t(i)))-(a*H(i+1)-b*H(i+1)*L(i+1));
equ4 = ((L(i+2)-L(i))/(t(i+2)-t(i)))-(-r*L(i+1)+c*H(i+1)*L(i+1));

S=solve(equ1,equ2,equ3,equ4,[a b r c]);
p=(p+ (S.a))*-1; %compensating for changing the constant c sign in ax+by=c

q=(q+ (S.b))*-1;
f=(f+(S.r))*-1;
g=(g+ (S.c))*-1;
clear a b r c
syms a b r c

%averaging out the a,b,r,c values by dividin it by(i-10)
m1 =double (p /(i-7))   % m1= a , m2= b, m3=r, m4=c
m2=double(q/(i-7))
m3 =double(f/(i-7))
m4= double(g/(i-7))

%print(m1, m2 ,m3,m4)

end


