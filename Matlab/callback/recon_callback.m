function recon_callback( button, ~, hs1, hs2, hs3, ...
    isFiltered, hfreq, hnIter, isMLEM, hsx, hsy, hsz, isFBP, projData,...
    hf1, hf2, hf3, ha1, ha2, ha3)

% This is the callback function for the reconstruction button. When the
% button is clicked, reconstruction process will be started. Parameters for
% reconstruction are get from the panels.

% get global image
global im

% disable the button and sliders to avoid conflict
button.Enable = 'off';
hs1.Enable = 'off';
hs2.Enable = 'off';
hs3.Enable = 'off';

% start recon
disp('recon start')
% parse input arguments
[freq, nIter, sx, sy, sz] = parseInput(hsx, hsy, hsz,hfreq, hnIter);
% set options
options.Niter = nIter;
if(isMLEM.Value)
    options.Ns = 1;
else
    options.Ns = 4;
end
options.sigma = [sx, sy, sz];
options.freq = freq;
options.isFiltered = isFiltered.Value;
% get parameters
param = defaultParam();
% use FBP or OSEM
if isFBP.Value
    im = FBP(projData,param, options);
else
    im = OSEM(projData, param, options);
end
% normalization
im = abs(im);
im = im / max(im(:));
disp('recon end')
% enable the button and sliders after recon
button.Enable = 'on';
hs1.Enable = 'on';
hs2.Enable = 'on';
hs3.Enable = 'on';
% update image
slider_callback(hs1,[],hf1,ha1,1);
slider_callback(hs2,[],hf2,ha2,2);
slider_callback(hs3,[],hf3,ha3,3);

end

function [freq, nIter, sx, sy, sz] = parseInput(hsx, hsy, hsz, ...
    hfreq, hnIter)

% This function is used to parse the input arugments, including cut-off
% frequence, number of iterations, sigma_x, sigma_y and sigma_z

% convert string to double
freq = str2double(hfreq.String);
nIter = floor(str2double(hnIter.String));
sx = str2double(hsx.String);
sy = str2double(hsy.String);
sz = str2double(hsz.String);

% check range of parameters
% if out of range, use default value
if(isempty(freq) || freq <=0 || freq > 1)
    freq = 1;
end

if(isempty(nIter) || nIter <= 0)
    nIter = 2;
end

if(isempty(sx) || sx <= 0)
    sx = 2.03;
end

if(isempty(sy) || sy <= 0)
    sy = 2.03;
end

if(isempty(sz) || sz <= 0)
    sz = 1.56;
end

end

