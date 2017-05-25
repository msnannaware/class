function root = secant(f,x,eps,true)
%x(1) = x1;
%x(2) = x2;
for b=2:20
    x(b+1) =x(b) - f(x(b))*(x(b)-x(b-1))/(f(x(b))-f(x(b-1)));
    root = x(b+1);
    error = root - true
    if (abs(x(b+1)-x(b)) < eps)
        return
    end
end
disp(' Secant does not converge')
 