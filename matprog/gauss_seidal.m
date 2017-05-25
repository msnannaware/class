clear;clc
format compact
%%   Input Matrix
A = [3 1 1;
     1 3 -5;
     1 3 -1]   % coefficients matrix
C = [5 ; -1 ; 3];% equal to vector
n = length(C);
X = zeros(n,1);
Error_eval = ones(n,1);

%% Check  diagonally dominancy of A
for i = 1:n
    j = 1:n;
    j(i) = [];
    B = abs(A(i,j));
    Check(i) = abs(A(i,i)) - sum(B); % check if diagonal value > sum of the remaining row values 
    if Check(i) < 0
        fprintf('The matrix is not diagonally dominant at row %2i\n\n',i)
    end
end

%% Start the Iterative method

iter = 0;
while max(Error_eval) > 0.000001
    iter = iter + 1;
    Z = X;  % save current values 
    for i = 1:n
        j = 1:n; % array of the coefficients' elements
        j(i) = [];  
        Xtemp = X;  % 
        Xtemp(i) = [];  % eliminate the unknown under question from the set of values
        X(i) = (C(i) - sum(A(i,j) * Xtemp)) / A(i,i);
    end
    Xsolution(:,iter) = X;
    Error_eval = sqrt((X - Z).^2);
end

%% Display Results
GaussSeidelTable = [1:iter;Xsolution]'
Result_Matrix = [A X C]