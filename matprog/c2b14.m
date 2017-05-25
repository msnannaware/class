x = [1;1];
for iter=1:100
    f = [x(1)+exp(-(x(1)))+(x(2)^3);
        (x(1)^2)+2*(x(1))*(x(2))-(x(2)^2)+tan(x(1))];
    J=[1-exp(-(x(1)))    3*(x(2)^2);
    2*x(1)+2*(x(2))+(sec(x(1)))^2  2*(x(1))-2*(x(2))]
    d = J\f;
    x=x-d;
    if (norm(d,2) < 1.e-10)
        break
    end    
end
disp x
