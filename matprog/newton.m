function root=newton(fname,fdname,x,xtol,ftol,n_max)
n = 0;
x= 4.5
xtol= 0.0000001
ftol =0.000001
fx = feval(f,x);
%if display,
%disp(’ n x f(x)’)
%disp(’-------------------------------------’)
%disp(sprintf(’%4d %23.15e %23.15e’, n, x, fx))
%end
if abs(fx) <= xtol
root = x;
return
end
for n = 1:n_max
fdx = feval(fdname,x);
d = fx/fdx;
x = x - d;
fx = feval(fname,x);
%if display,
%disp(sprintf(’%4d %23.15e %23.15e’,n,x,fx))
%end
if abs(d) <= xtol | abs(fx) <= ftol
root = x;
return
end
end