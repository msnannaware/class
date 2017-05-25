function y=GaussSeidel(A,b,x,NumIters)
% Runs the Gauss-Seidel method for solving Ax=b, starting with x and
% running a maximum of NumIters iterations.
%
% The matrix A should be diagonally dominant, and in particular, it should
% not have any diagonal elements that are zero (a division by zero error
% will be produced).
%
% The output y will be the whole sequence of outputs instead of the final
% value (if x is in R^n, then y will be n x NumIters

D=diag(A);

A=A-diag(D);
D=1./D; %We need the inverses
n=length(x);
x=x(:); %Make sure x is a column vector
y=zeros(n,NumIters);
for j=1:NumIters
for k=1:n
x(k)=(b(k)-A(k,:)*x)*D(k);
end
y(:,j)=x;
end
