function [ im ] = GaussFilter3D( im, param, sigma)

% This function performs the 3D Gaussian filter

% normalize the standard deviation of 3 direction
sigma(1) = sigma(1) / param.dx;
sigma(2) = sigma(2) / param.dy;
sigma(3) = sigma(3) / param.dz;
% 3d filtering
im = imgaussfilt3(im, sigma);

end

