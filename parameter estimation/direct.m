clc
clear all
close all
% MATLAB script to run on Lotka-Volterra model example

run('/Users/madhurinannaware/Documents/Natasha_sharma/lab2/lv_data.m');
guess =[1.2028 0.0324 0.2739 0.0044];
[p,error]=fminsearch(@lverr,guess);
disp(['Parameter values with direct Method :'])
disp(['a=' num2str(abs(p(1)))])
disp(['b=' num2str(abs(p(2)))])
disp(['r=' num2str(abs(p(3)))])
disp(['c=' num2str(abs(p(4)))])