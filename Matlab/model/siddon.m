function [ ii, jj, l ] = siddon(p1x, p1y, p2x, p2y, param)

% This function performs the 2D siddon algorithm

% number of slice
Nx = param.Nx + 1;
Ny = param.Ny + 1;

% difference
dpx = p2x - p1x;
dpy = p2y - p1y;

% degenerate to 1d
if(dpx == 0)
    [ii, jj, l] = siddon1y(p1x, p1y, p2x, p2y, param);
    return
end

% degenerate to 1d
if(dpy == 0)
    [ii, jj, l] = siddon1x(p1x, p1y, p2x, p2y, param);
    return
end

% alpha is the paramter for the line connecting p1 and p2
% p_x(alpha) = p1x + alpha(p2x - p1x)
% p_y(alpha) = p1y + alpha(p2y - p1y)
alphaX0 = (param.bx - p1x) / dpx;
alphaXEnd = ((Nx - 1) * param.dx + param.bx - p1x) / dpx;
alphaY0 = (param.by - p1y) / dpy;
alphaYEnd = ((Ny - 1) * param.dy + param.by - p1y) / dpy;

% find the first alpha and the last alpha
alphaXMin = min(alphaX0, alphaXEnd);
alphaXMax = max(alphaX0, alphaXEnd);

alphaYMin = min(alphaY0, alphaYEnd);
alphaYMax = max(alphaY0, alphaYEnd);

alphaMin = max(alphaXMin, alphaYMin);
alphaMax = min(alphaXMax, alphaYMax);

% find i_min and i_max
% case1 : p1x < p2x
% case2 : p1x > p2x
if(p1x < p2x)
    if(alphaMin == alphaXMin)
        iMin = 1;
    else
        iMin = ceil(phi(alphaMin, p1x, dpx, param.bx, param.dx));
    end
    
    if(alphaMax == alphaXMax)
        iMax = Nx - 1;
    else
        iMax = floor(phi(alphaMax, p1x, dpx, param.bx, param.dx));
    end
else
    if(alphaMin == alphaXMin)
        iMax = Nx - 2;
    else
        iMax = floor(phi(alphaMin, p1x, dpx, param.bx, param.dx));
    end
    
    if(alphaMax == alphaXMax)
        iMin = 0;
    else
        iMin = ceil(phi(alphaMax, p1x, dpx, param.bx, param.dx));
    end 
end

% find j_min and j_max
% case1 : p1y < p2y
% case2 : p1y > p2y
if(p1y < p2y)
    if(alphaMin == alphaYMin)
        jMin = 1;
    else
        jMin = ceil(phi(alphaMin, p1y, dpy, param.by, param.dy));
    end
    
    if(alphaMax == alphaYMax)
        jMax = Ny - 1;
    else
        jMax = floor(phi(alphaMax, p1y, dpy, param.by, param.dy));
    end
else
    if(alphaMin == alphaYMin)
        jMax = Ny - 2;
    else
        jMax = floor(phi(alphaMin, p1y, dpy, param.by, param.dy));
    end
    
    if(alphaMax == alphaYMax)
        jMin = 0;
    else
        jMin = ceil(phi(alphaMax, p1y, dpy, param.by, param.dy));
    end
end

% return if the line dose not intersect with the VOI
if(iMin < 0 || jMin < 0 || iMax < 0 || jMax < 0 ||...
        iMin > Nx-1 || jMin > Ny-1 ||...
        iMax > Nx-1 || jMax > Ny-1)
    ii = [];
    jj = [];
    l = [];
    return
end

% calculate all alphax
if(p1x < p2x)
    alphaX = (param.bx - p1x + (iMin : iMax) * param.dx) / dpx;
else
    alphaX = (param.bx - p1x + (iMax : -1 :iMin) * param.dx) / dpx;
end

% calculate all alphay
if(p1y < p2y)
    alphaY = (param.by - p1y + (jMin : jMax) * param.dy) / dpy;
else
    alphaY = (param.by - p1y + (jMax : -1 :jMin) * param.dy) / dpy;
end

% combine alphax and alphay
alphaXY = unique([alphaMin alphaX alphaY]);
alphaXYavg = (alphaXY(2:end) + alphaXY(1:end-1)) / 2;

% find all index
ii = floor(phi(alphaXYavg, p1x, dpx, param.bx, param.dx));
jj = floor(phi(alphaXYavg, p1y, dpy, param.by, param.dy));
% find all length of intersection
l = alphaXY(2:end) - alphaXY(1:end-1);
end


function [ii, jj, l] = siddon1y(p1x, p1y, p2x, p2y, param)

% This function perform the 1d siddon algorithm

% number of slice
Nx = param.Nx + 1;
Ny = param.Ny + 1;

% i index
i = floor((p1x - param.bx) / param.dx);

% return if the line dose not intersect with the VOI
if(i < 0 || i > Nx - 2)
    ii = [];
    jj = [];
    l = [];
    return
end

% calculate all index
jj = 0:Ny-2;
ii = ones(size(jj)) * i;
% calculate all length
l = ones(size(ii)) * param.dy;

end

function [ii, jj ,l] = siddon1x(p1x, p1y, p2x, p2y, param)

% This function perform the 1d siddon algorithm

% number of slice
Nx = param.Nx + 1;
Ny = param.Ny + 1;

% j index
j = floor((p1y - param.by) / param.dy);

% return if the line dose not intersect with the VOI
if(j < 0 || j > Ny - 2)
    ii = [];
    jj = [];
    l = [];
    return;
end

% calculate all index
ii = 0 : Nx-2;
jj = ones(size(ii)) * j;
% calculate all length
l = ones(size(ii)) * param.dx;

end

function ph = phi(alpha, p1, dp, b, d)

% This is a simple helper function

ph = (p1 + alpha * dp - b) / d;
end

 
