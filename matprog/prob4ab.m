Gradf = gradient(f); Hf = hessian(f);

Jach = jacobian(h); Hh = hessian(h);

% Declare functions for problem

F = [Gradf+Jach’*y1;h];

F = inline(F,’x1’,’x2’,’y1’);% evaluating

dF = [Hf+Hh*y Jach’; Jach 0];

dF = inline(dF,’x1’,’x2’,’y1’);% evaluating

fprintf(’Iter\t|\t\tx1\t\t|\t\tx2\t\t|\ty\t\t|\t\tError\t\t|\n’);

fprintf(’%6.0f\t\t\t%.8f\t\t%.8f\t\t%.8f\t%.8f\t\n’, ...

iter,x’,y,norm(F(x(1),x(2),y)));

while (iter<=maxiter && norm(F(x(1),x(2),y))>epsilon)

% Newton Step

dx = -dF(x(1),x(2),y)\F(x(1),x(2),y);

% Updating x and y

x = x+dx(1:2)’;

y = y+dx(3);

%updating iterations

iter = iter+1;

fprintf(’%6.0f\t\t\t%.8f\t\t%.8f\t\t%.8f\t%.8f\t\n’, ...

iter,x’,y,norm(F(x(1),x(2),y)));

end