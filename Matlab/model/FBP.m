function im = FBP(projData, param, options)

% Flitered Back Projection

% try to load pre-calculated system matrix
try
    load SystemMatrix.mat
    disp('Loaded pre-calculated system matrix.')
catch
    % falid!
    disp('Can not find system matrix. Calculate it now.')
    % calcualte again
    P = sysmat(param);
    % save the result
    save('SystemMatrix.mat', 'P');
end

% reorder the projection data
projData = reshape(projData, param.Nr, param.Nz, param.Na);
projData = permute(projData, [1,3,2]);

% use ramp filter
if options.isFiltered
    % calcualte the filter in frequence domain
    F = getFilter(param.Nr, options.freq);
    len = size(projData, 1);
    % zero padding before filtering
    projData(length(F), 1, 1)=0;
    % DFT of the projection data 
    projData = fft(projData); 
    % filtering
    projData = bsxfun(@times, projData, F);
    % inverse DFT
    projData = ifft(projData,'symmetric');
    % Truncation
    projData(len+1:end,:,:) = [];      
end

% reshape the projection data
projData = reshape(projData, param.Nr * param.Na, param.Nz);
% back projection
im = P' * projData;

% reshape the image
im = reshape(im, param.Nx, param.Ny, param.Nz);
% post gaussian filtering
im = GaussFilter3D( im, param, options.sigma);

end

function filt = getFilter(Nr, d)

% This function calcualte the ramp filter in frequence domain

% length of filter
order = max(64,2^nextpow2(2*Nr));
% since the filter is symmetric, we can calculate half of it
n = 0:(order/2); 
% first, calculate the impulse response of the filter
resp = zeros(1,(order/2)+1); 
% DC term
resp(1) = 1/4;
resp(2:2:end) = -1./((pi*n(2:2:end)).^2);
resp = [resp resp(end-1:-1:2)]; 
% convert to frequence domain
filt = 2*real(fft(resp)); 
filt = filt(1:(order/2)+1);
% frequency
w = 2*pi*(0:size(filt,2)-1)/order;   
% cut-off frequency (rectangle window)
filt(w>pi*d) = 0;
% recall that the filter is symmetric
filt = [filt' ; filt(end-1:-1:2)'];    
end
