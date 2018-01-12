function slider_callback(hslider, ~,hf,ha,dir)

% This is the callback function for sliders. the sliders are used to show 
% different slice of the 3D volume.

% persistent variable to avoid conflict
persistent busy;
% get global image
global im;
% init persistent variable
if(isempty(busy))
    busy = 0;
end
% get slice number
slice = round(get(hslider,'Value'));
% set slice
set(hslider,'Value', slice);
% if not busy
if ~busy
    % set busy
    busy = 1;
    % get image slice
    if (dir == 1)
        imSlice = squeeze(im(:,:,slice));
    elseif(dir == 2)
        imSlice = squeeze(im(:,slice,:));
    elseif(dir == 3)
        imSlice = squeeze(im(slice,:,:));
    end
    % update image
    set(hf,'CData',imSlice);
    set(ha, 'CLim',[0, max(imSlice(:))]);
    drawnow;
    busy = 0;
end
end