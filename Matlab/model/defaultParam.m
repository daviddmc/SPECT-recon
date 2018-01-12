function param = defaultParam()

% This function returns the default parameter.

% size of 3D image
% 128 x 128 x 128
param.Nx = 128;
param.Ny = 128;
param.Nz = 128;

% size of projection data
% number of angles(views)
param.Na = 60;
% number of tangent sampling
param.Nr = 128;
% interval of tangent sampling
param.dr = 4.0625;

% size of voxel
param.dx = 4.0625;
param.dy = 4.0625;
param.dz = 3.125;

% shift of origin
param.bx = -260;
param.by = -260;
param.bz = -200;

end

