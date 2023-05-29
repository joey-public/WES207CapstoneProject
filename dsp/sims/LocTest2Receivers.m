function xCoord = LocTest2Receivers(TDoA)
%Tests localization with 2 receivers by using each value twice
%inputs
len = 100; %length (x dimension, meters)
wid = 100; %width (y dimension, meters)
ht = 10; %height of highest receiver (z dimension, meters)
height_diff = 2; %difference of height between each receiver (meters)

%constants
c = 3e8; %speed of light (m/s)
numRec = 4; %number of receivers

%% Get expected ToA for each receiver based on that location

%receiver locations
xlocs = {0,len,0,len};
ylocs = {0,0,wid,wid};
zlocs = {ht-3*height_diff,ht-height_diff,ht,ht-2*height_diff};

rec = struct('x',xlocs,'y',ylocs,'z',zlocs);

rotAngle = [0 0 pi/2 atan2(rec(4).y,rec(4).x); ...
            pi 0 atan2(rec(3).y,-rec(2).x) pi/2; ...
            3*pi/2 atan2(-rec(3).y,rec(2).x) 0 0; ...
            atan2(-rec(4).y,-rec(4).x) 3*pi/2 pi 0];

%calculation
t = zeros(4,1);
t(1) = 0;
t(2) = TDoA;
t(3) = 0;
t(4) = TDoA;


%% Convert ToA to TDoA
[first, idx] = min(t);
tdiff = t-min(t);

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
    plot(rec(k).x,rec(k).y,'*','Color','b','MarkerSize',12)
end
%plot(x,y,'x','Color','r','MarkerSize',12,'LineWidth',2)
hold off
grid on
grid minor

%% Solve equations to get estimated location
%follow equations from Schau/Robinson and Smith/Abel papers
delta = zeros(numRec-1,1); 
S = zeros(numRec-1,3); %receiver coordinates (reference receiver at origin)
d = zeros(numRec-1,1); %difference matrix without the reference receiver

k = 1;
for recIdx = 1:numRec
    if recIdx ~= idx
        d(k) = ddiff(recIdx);
        S(k,1) = rec(recIdx).x - rec(idx).x;
        S(k,2) = rec(recIdx).y - rec(idx).y;
        S(k,3) = rec(recIdx).z - rec(idx).z;
        delta(k) = ((S(k,1)^2)+(S(k,2)^2)+(S(k,3)^2)-(d(k)^2));
        k = k+1;
    end
end

W = eye(numRec-1); %weight matrix - for now consider equal weight
S_star = (S'*W*S)\S'*W;
%S_star = inv(S'*S)*S';
%P_s = S*S_star;
%T = eye(numRec-1) - P_s;

%a = S_star*delta;
%b = S_star*d;

%Rs1 = (dot(a,b))+((sqrt((dot(a,b)^2)-((dot(b,b)-1)*dot(a,a))))/(dot(b,b)-1));
%Rs2 = (dot(a,b))-((sqrt((dot(a,b)^2)-((dot(b,b)-1)*dot(a,a))))/(dot(b,b)-1));

Rs_a = 4-4*d'*S_star'*S_star*d;
Rs_b = 2*d'*S_star'*S_star*delta+2*delta'*S_star'*S_star*d;
Rs_c = -delta'*S_star'*S_star*delta;

Rs1 = (-Rs_b+sqrt(Rs_b^2-4*Rs_a*Rs_c))/(2*Rs_a);
Rs2 = (-Rs_b+abs(sqrt(Rs_b^2-4*Rs_a*Rs_c)))/(2*Rs_a);

%Rs = d.'*T*delta/(2*d.'*T*d);
%Rs = d.'*delta/(2*d.'*d);
%loc_est = 0.5*S_star*(delta-2*Rs*d);
%loc_est1 = a-b*Rs1;
%loc_est2 = a-b*Rs2;

loc_est1 = 0.5*S_star*(delta-2*Rs1*d);
loc_est2 = 0.5*S_star*(delta-2*Rs2*d);

%switch origin back
loc_est1(1) = loc_est1(1)+rec(idx).x;
loc_est1(2) = loc_est1(2)+rec(idx).y;
loc_est1(3) = loc_est1(3)+rec(idx).z;
loc_est2(1) = loc_est2(1)+rec(idx).x;
loc_est2(2) = loc_est2(2)+rec(idx).y;
loc_est2(3) = loc_est2(3)+rec(idx).z;

%This produces 2 location estimates due to the quadratic equation
%Through 100 tests, the first was more accurate 100% of the time
%Going with that for now

figure(1)
hold on
plot(loc_est1(1),loc_est1(2),'o','Color','k','MarkerSize',12,'LineWidth',2)
plot(loc_est2(1),loc_est2(2),'o','Color','b','MarkerSize',12,'LineWidth',2)
hold off

xCoord = loc_est1(1);
