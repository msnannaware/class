clc
clear
close all

input=[10,50,100];

for l=1:3
val=input(l);
sqval=val^2;
h=1/val;
iter_max=sqval;
u0=zeros(sqval,1);
b=ones(sqval,1);

for i=1:sqval
    for j=1:sqval
      if i==j
          A(i,j)=4/(h^2);
      elseif abs(i-j)==1
          A(i,j)=-1/(h^2);
      elseif abs(i-j)==val
          A(i,j)=-1/(h^2);
      else A(i,j)=0;
      end
    end
end
rslt=b-A*u0;
p=rslt;
Rho=rslt'*rslt;
n_iter=0;
tol=10^(-7);
u=u0;

while norm(rslt)/norm(b)>=tol
    a=A*p;
    alpha=Rho/(a'*p);
    u=u+alpha*p;
    rslt=rslt-alpha*a;
    Rho_new=rslt'*rslt;
    p=rslt+(Rho_new/Rho)*p;
    Rho=Rho_new;
    n_iter=n_iter+1;
    
    if n_iter==iter_max
        disp(['there is no convergence']);
        break
        
    end 
    
end
direct_inv= A\b   %%to talley conjugtate o/p u with direct inversion 


disp(['value=' num2str(val)])

disp(['iteration_number=' num2str(n_iter)])
disp(['||r||=' num2str(norm(rslt))])
disp(['direct_inv_norm=' num2str(norm(direct_inv))])
disp(['conjugate_norm=' num2str(norm(u))])
end




