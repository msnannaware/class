 function [x]=mar_linesearch-mimimizer(x, epsilon,k,mu,maxiter)
%
% find the minimum value of f. 
% i.e.     x=arg{min(f)}
% maxiter;number of possibilities for linesearch  k; descent search
% descent direction mu > 1
%

  f=feval(ff,x); 
  gradf=feval(Dff,x); %function and gradient evaluation
  iter=0;
  while iter <= maxiter
            error=norm(gradf);
                      if error <= epsilon
               return % finish
         H=feval(Hf,x); %Hessian evaluation
         dx=-(H/gradf); %Newton's direction
                if gradf' dx>0    % check decent direction  
                   landa=min{eig(H)}; %minimum eigenvalue
                   dx=-((H+mu*landa)/gradf); % descent Newton's direction 
                 end % if
                     for p=1:k    %linesearch( sufficient decrease)
                           alpha=.5^p;
                           X=x+alpha*dx;
                           f_update=feval(ff,X);
                                 if f_update <= f+1e-4*alpha*(grad'*dx)
                                  end %if
                       end  % for
         x=X; grad=feval(Dff); %update direction and gradient
         iter=iter+1;
     end  % while
end  % function
