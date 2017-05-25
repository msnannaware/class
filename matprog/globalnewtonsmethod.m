function y = globalnewtonsmethod(x,f,df,error,maxiter,k)
%INITIALIZING
%syms x 
rho = 1/2; %initializing rho
 alpha=1;
  iter = 0; % initializing iterations
   f1 = feval(f,x);% evaluating f at x
   absf1=abs(f1);
  df1 = feval(df,x); 
   dx = -f1/df1; % Newton's step
  f2=feval(f,x+alpha*dx); 
 fprintf('iteration  root\n')

 while (absf1 > error && (iter <=maxiter))
     
         f1 = feval(f,x);
         absf1=abs(f1);
         df1 = feval(df,x); 
         dx = -f1/df1; % Newton's step
         f2=feval(f,x+alpha*dx);
         
        
         %Backtracking Line Search
            for t=0:k
                alpha=(rho)^t;
                X=x+alpha*dx; % update
                f2=f(X);
                abs_f2=abs(f2);
                    if (abs_f2)<=sqrt(1-(2*1e-4*alpha))*absf1
                        break
                    end %if
             end %for 
        
    x=X; % update
    absf1=abs_f2;
    iter=iter+1; % update of iteration
    fprintf('  %2d     %2d\n',iter,x)

 end % while
