function error=lverr(p)
%LVERR: Function defining error function for Lotka-Volterra model

run('/Users/madhurinannaware/Documents/Natasha_sharma/lab2/lv_data.m');
years=[1862:2:1910];
[t,y]=ode45(@lv_rhs,years,[H(1);L(1)],[],p);
value=((y(:,1)-H').^2)+((y(:,2)-L').^2);
%Primes transpose H & L vectors
error=sum(value);
end
