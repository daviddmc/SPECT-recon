function P = sysmat( param )

% This function calculates the system matrix

% init a sparse matrix
P = sparse(param.Na * param.Nr, param.Nx * param.Ny);

% pre-calculate the cos and sin of the view angles.
cosList = cos(pi/(param.Na/2) * (0:param.Na-1));
sinList = sin(pi/(param.Na/2) * (0:param.Na-1));

% for each view angle
for a = 1:param.Na
    % for each tangent sampling
    for k = 1:param.Nr
        % coordinate along the tangent sampling direction
        s = param.dr * (k - param.Nr/2 - 0.5);
        % select 2 points on the projection line
        p1x = s * sinList(a);
        p1y = s * cosList(a);
        p2x = p1x + cosList(a);
        p2y = p1y - sinList(a);
        % use siddon algorithm
        [ii, jj, l] = siddon(p1x, p1y, p2x, p2y, param);
        % update system matrix
        if(~isempty(ii))
            P((a-1) * param.Nr + k, ii * param.Ny + jj + 1) = ...
                P((a-1) * param.Nr + k, ii * param.Ny + jj + 1) + l;
        end
    end
end

end

