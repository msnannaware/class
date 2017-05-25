x(1) = 4;
x(2) = 5;
f = @(x) (x^3-(12*(x^2))+3*x+1) ;

tolerance = 0.00000000000000000000001;

%// Let's try from 3 to 25 
for k=3:15

    x(k) = x(k-1) - (f(x(k-1)))*((x(k-1) - x(k-2))/(f(x(k-1)) - f(x(k-2))));

    if abs(x(k)-x(k-1)) < tolerance
        break
    end
end
    