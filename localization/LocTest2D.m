%inputs
len = 100; %length (x dimension, meters)
wid = 100; %width (y dimension, meters)
ht = 0; %height of highest receiver (z dimension, meters)
height_diff = 0; %difference of height between each receiver (meters)

sigma = 0; %standard deviation of distance estimate noise (meters)

%constants
c = 3e8; %speed of light (m/s)
numRec = 4;

%% Generate Location for testing
% Receivers have nonnegative coordinates
% begin with transmitter location between 0 and 1, then scale
x = rand();
y = rand();
z = 0; % not currently concerned about height of transmitter

x = len*x;
y = wid*y;

%% Get expected ToA for each receiver based on that location
xlocs = {0,len,0,len};
ylocs = {0,0,wid,wid};
zlocs = {ht-3*height_diff,ht-height_diff,ht,ht-2*height_diff};

rec = struct('x',xlocs,'y',ylocs,'z',zlocs);

rotAngle = [0 0 pi/2 atan2(rec(4).y,rec(4).x); ...
            pi 0 atan2(rec(3).y,-rec(2).x) pi/2; ...
            3*pi/2 atan2(-rec(3).y,rec(2).x) 0 0; ...
            atan2(-rec(4).y,-rec(4).x) 3*pi/2 pi 0];

%calculation
d = zeros(numRec,1); %distances
for k = 1:numRec
    d(k) = sqrt((rec(k).x-x)^2+(rec(k).y-y)^2+(rec(k).z-z)^2);
end

t = d/c; %times of arrival


%% Optionally, add noise
n = sigma*randn(numRec,1);
n_time = n/c;
tnoise = t + n_time;

%% Convert ToA to TDoA
[first, idx] = min(tnoise);
tdiff = tnoise-min(tnoise);

%difference in distance to each receiver of transmitter
ddiff = tdiff*c;

%% Using one receiver as a reference, determine locations

recDist = zeros(numRec,1); %distance between receivers
hyperC = zeros(numRec,1); %C parameters of hyperbola (distance from center to focus)
hyperA = zeros(numRec,1); %A parameter of hyperbola (distance from center to hyperbola)
hyperH = zeros(numRec,1); %H parameter of hyperbola (x coordinate of center)
hyperK = zeros(numRec,1); %K parameter of hyperbola (y coordinate of center)

for k = 1:numRec
    recDist(k) = sqrt((rec(k).x-rec(idx).x)^2+(rec(k).y-rec(idx).y)^2+(rec(k).z-rec(idx).z)^2);
    hyperC(k) = recDist(k)/2;
    hyperA(k) = ddiff(k)/2;
    hyperH(k) = (rec(k).x+rec(idx).x)/2;
    hyperK(k) = (rec(k).y+rec(idx).y)/2;
end

hyperE = hyperC./hyperA; %eccentricity
hyperB = sqrt((hyperE.^2-1).*hyperA.^2); %B parameter of hyperbola


%% 2D - show that the generated test location lies on Hyperbola
figure(1)
for k = 1:numRec
    ang = rotAngle(idx,k);
    f = @(x,y) ((((y-hyperK(k))*sin(ang)+(x-hyperH(k))*cos(ang)).^2)/hyperA(k).^2) - ((((y-hyperK(k))*cos(ang)-(x-hyperH(k))*sin(ang)).^2)/hyperB(k).^2) - 1;
    fimplicit(f,[-20 120 -20 120])
    hold on
    plot(rec(k).x,rec(k).y,'*','Color','r','MarkerSize',12)
end
plot(x,y,'x','Color','k','MarkerSize',12,'LineWidth',2)
hold off
grid minor

%% TO DO: Solve equations to get estimated location


%error in meters

