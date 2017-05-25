% FIGURE 1.3.1
      function X = DRESLV(A,N,C,IPERM)
%
%  FUNCTION DRESLV SOLVES THE LINEAR SYSTEM A*X=C IN O(N**2) TIME,
%    AFTER DLINEQ HAS PRODUCED AN LU DECOMPOSITION OF PA.
%
%  ARGUMENTS
%
%             ON INPUT                          ON OUTPUT
%             --------                          ---------
%
%    A      - THE N BY N COEFFICIENT MATRIX
%             AFTER PROCESSING BY DLINEQ.
%             AS OUTPUT BY DLINEQ, A CONTAINS
%             AN LU DECOMPOSITION OF PA.
%
%    N      - THE SIZE OF MATRIX A.
%
%    X      -                                   AN N-VECTOR CONTAINING
%                                               THE SOLUTION.
%
%    C      - THE RIGHT HAND SIDE N-VECTOR.
%
%    IPERM  - THE PERMUTATION VECTOR OF
%             LENGTH N OUTPUT BY DLINEQ.
%
%-----------------------------------------------------------------------
%                              CALCULATE C=P*C, WHERE P IS PERMUTATION
%                              MATRIX DEFINED BY IPERM.
      for K=1:N
         J = IPERM(K);
         X(K) = C(J);
      end
      for K=1:N
         C(K) = X(K);
      end
%                              BEGIN FORWARD ELIMINATION, TO CALCULATE
%                              C = L^(-1)*C
      for I=1:N-1
         for J=I+1:N
%                              RETRIEVE MULTIPLIER SAVED IN A(J,I)
            LJI = A(J,I);
%                              SUBTRACT LJI TIMES C(I) FROM C(J)
            C(J) = C(J) - LJI*C(I);
         end
      end
%                              SOLVE U*X = C USING BACK SUBSTITUTION.
      X(N) = C(N)/A(N,N);
      for I=N-1:-1:1
         SUM = 0.0;
         for J=I+1:N
            SUM = SUM + A(I,J)*X(J);
         end
         X(I) = (C(I)-SUM)/A(I,I);
      end
 
 