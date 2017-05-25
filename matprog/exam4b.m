f = [Gradf+Jach’*y1;h];

f = inline(F,’x1’,’x2’,’y1’);% evaluating

df = [Hf+Hh*y Jach’; Jach 0];

df = inline(df,’x1’,’x2’,’y1’);% evaluating

fprintf(’Iteration\t|\t\tx1\t\t|\t\tx2\t\t|\ty\t\t|\t\tError\t\t|\n’);

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