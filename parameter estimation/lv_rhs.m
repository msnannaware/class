function value=lv_rhs(t,y,p)
%lv_rhs.m
% This function provides the RHS vector valued function for the Lotka
% Volterra system of ODEs
% dx/dt = ax - bxy;
% dy/dt = -ry + cxy;
%with parameters p=(a,b,r,c)'.
% y= (x,y)= (y(1),y(2))
% Author:msnannaware
% Date:04/17/2017
value=[p(1)*y(1)-p(2)*y(1)*y(2);-p(3)*y(2)+p(4)*y(1)*y(2)];
end

