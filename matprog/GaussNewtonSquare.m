 function [unknowns,steps,S] = GaussNewtonMethod()
%GaussNewtonPopulation - Uses the Gauss-Newton Method to find the initial(x1, x2)
format long
tol = 1e-8; %set a value for the accuracy
maxstep = 30; %set maximum number of steps to run for

t = [1,2, 4, 5,8];

y=[3, 4 , 6, 11, 20];
a = [1.5;1]; %set initial guess for x1 and x2
m=length(t); %determine number of functions
n=length(a); %determine number of unkowns
aold = a;
for k=1:maxstep %iterate through process
 S = 0;
 for i=1:m
 for j=1:n
 J(i,j) = df(t(i),y(i),a(1,1),a(2,1),j); %calculate Jacobian
 JT(j,i) = J(i,j); %and its trnaspose
 end
 end
 Jz = -JT*J; %multiply Jacobian and
 %negative transpose
 for i=1:m
 r(i,1) = (a(1,1)*exp((t(i))*a(2,1)))-y(i) ;%calculate r
 S = S + r(i,1)^2; %calculate the sum of the squares of the residuals

 end
 S
 g = Jz\JT; %mulitply Jz inverse by J transpose
 a = aold-g*r; %calculate new approximation
 unknowns = a; %update

 abs(a(1,1)-aold(1,1)); %calculate error
 if (abs(a(1,1)-aold(1,1)) <= tol); %if less than tolerance break
 break
 end
 aold = a; %set aold to a
end
steps = k;
f = unknowns(1,1)*exp(unknowns(2,1)*t); 

 end

function value = df(t,y,a1,a2,index) %calculate partial derivatives
switch index
 case 1
 value = a1
 case 2
 value = t*a1*exp(a2*t);
end
end
