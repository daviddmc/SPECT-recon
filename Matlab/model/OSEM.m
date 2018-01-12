function im = OSEM(projData, param, options)

% Ordered Subset Expectation Maximization

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

% get subset
subset = getSubset(options.Ns);

% init
Ptmp = P;
P = {};
projData = reshape(projData, param.Nr, param.Nz, param.Na);
projData = permute(projData, [1,3,2]);
projDataTmp = reshape(projData, param.Nr * param.Na, param.Nz);
projData = {};
% for each subset
for ii = 1:options.Ns
    % calcualte the idx for this subset
    idx = [];
    for jj = 1:length(subset{ii})
        idx = [idx, (subset{ii}(jj) * param.Nr) + (1:param.Nr)];
    end
    % get the projection data of this subset
    projData{ii} = projDataTmp(idx, :);
    % get the projection matrxi for this subset
    P{ii} = Ptmp(idx, :);
    % calculate the normalization factor for this subset
    tmp = full(sum(P{1},1));
    tmp = 1 ./ tmp(:);
    tmp(~isfinite(tmp)) = 0;
    normFac{ii} = tmp;
end

% init the image
im = zeros(param.Nx, param.Ny);
for ii = 1:param.Nx
    for jj = 1:param.Ny
        if((ii-param.Nx/2)^2 + (jj-param.Ny/2)^2 < ...
                ((param.Nx/2)^2 + (param.Ny/2)^2)/2)
            im(ii, jj) = 1;
        end
    end
end
% reshape the image
im = repmat(im, [1, 1, param.Nz]);
im = reshape(im, param.Nx * param.Ny, param.Nz);

% for each iteration
for iter = 1 : options.Niter
    disp(['iter ' num2str(iter)]);
    % for each subiteration
    for subiter = 1 : options.Ns
        disp(['subiter ' num2str(subiter)]);
        % projection
        proj = P{subiter} * im;
        tmp = projData{subiter} ./ proj;
        tmp(~isfinite(tmp)) = 0;
        % backprojection
        backproj = P{subiter}' * (tmp);
        im = im .* backproj;
        % normalize
        for z = 1:param.Nz
            im(:, z) = im(:, z) .* normFac{subiter};
        end
    end
    % post filter
    im = reshape(im, param.Nx, param.Ny, param.Nz);
    im = GaussFilter3D( im, param, options.sigma);
    im = reshape(im, param.Nx * param.Ny, param.Nz);
end
% reshape image
im = reshape(im, param.Nx, param.Ny, param.Nz);
end

function subset = getSubset(Ns)

% This function returns the subsets

if Ns == 1 % 1 subset only, MLEM
    subset{1} = 0:59;
elseif Ns == 4 % 4 subsets
    subset{1} = [0,1,2,3,4,5,6,7,30,31,32,33,34,35,36,37];    
    subset{2} = [15,16,17,18,19,20,21,22,45,46,47,48,49,50,51,52];
    subset{3} = [8,9,10,11,12,13,14,38,39,40,41,42,43,44];
    subset{4} = [23,24,25,26,27,28,29,53,54,55,56,57,58,59];
end

end