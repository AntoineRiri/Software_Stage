% Colour lines in distinct colours
%
% distinctcolour                : colour all lines on the current axes
% distinctcolour(hAxis)         : colour all lines in axis hAxis
% distinctcolour(hLines)        : colour lines with handles in hLines
% distinctcolour(...,mask)      : colour lines specified by mask, which may be
%                                EITHER a logical array indicating whether a
%                                line should be included OR a vector of line
%                                numbers to colour.
% distinctcolour(...,fnum,lnum) : colour all lines except the first fnum
%                                and the last lnum.
%
% colours = distinctcolour('getcolours',numLines)
%                              : return the colours that would be used for
%                                a certain number of lines.

% Copyright Chris Rodgers, University of Oxford, 2008-13.
% $Id$

function huesOut = distinctcolour(axish,varargin)
narginchk(0,3)

if nargin<1
    axish=gca;
end

% Check if returning list of colours...
if ischar(axish) && strcmp(axish,'getcolours') && numel(varargin) == 1
    huesOut = distinguishable_colors(varargin{1},[1 1 1; 0 0 0]);
    return
end

if strcmp(get(axish(1),'type'),'axes')
    allch=flipud(get(axish,'Children'));

    ch=[];
    for idx=1:numel(allch)
        if strcmp(get(allch(idx),'Type'),'errorbar') || ...
                strcmp(get(allch(idx),'Type'),'hggroup')
            ch(end+1)=allch(idx);
        end
    end
else
    ch=axish;
end

if nargin<=1
  mask=true(size(ch));
elseif nargin==2
  if islogical(varargin{1})
    mask=varargin{1};
  else
    mask=false(size(ch));
    mask(varargin{1})=1;
  end
else
  mask=true(size(ch));
  mask([1:varargin{1},(end-varargin{2}+1):end])=0;
end

if all(size(mask.')==size(ch))
  mask=mask.';
elseif ~all(size(mask)==size(ch))
  error('The mask must fit the number of curves!')
end

num=sum(mask);

hues=distinguishable_colors(num,[1 1 1; 0 0 0]);

ch=ch(mask);
for idx=1:num
    set(ch(idx),'Color',hues(idx,:));
    
    % Colour children of a hggroup object.
    if strcmp(get(ch(idx),'type'),'hggroup')
        set(findobj(ch(idx)),'Color',hues(idx,:));
    end
end
end
% 
% function hues = getHues(num)
% % Darken yellows
% yellowcentre = 1/6;
% yellowwidth = 1/5;
% yellowdip = 1/10; % How much to knock off brightness.
% 
% huevals=linspace(0,2/3,num).';
% luminancevals=repmat(0.5,num,1);
% luminancevals(abs(huevals-yellowcentre)<yellowwidth) = ...
%     0.5 - yellowdip*...
%     cos(pi * (huevals(abs(huevals-yellowcentre)<yellowwidth)-yellowcentre)/yellowwidth);
% 
% hues=hsl2rgb([huevals repmat(1,num,1) luminancevals]);
% end