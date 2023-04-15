%Define receiver and source Locations
N=4;
c = 3e8;
reciever_locs = [[0,0,0];[50,0,10];[0,50,20];[50,50,15]];
source_loc = [10,10,-10];

%calc distance between each recevier and source and convert to time of
%arrival (toa)
distances = zeros(1,3);
for i = 1:N
    r_loc = reciever_locs(i,1:end);
    distances(i) = norm(r_loc - source_loc);
end
toas = distances/c; %this is the measurement we expect to get from the USRPS

%use the toa data to estimate the source_location
%base on papaer by 
tdoas = toas-toas(1);
tdoas = tdoas';
S = reciever_locs;
d = tdoas .* c;
z = zeros(N,1);
size(z)
for j = 1:N
    a = norm(S(j,1:end))^2;
    b = d(j)^2;
    z(i) = a-b;
end
z = 0.5 .* z;
R = eye(N,N);
a = inv(S'*inv(R)*S)*S'*inv(R)*z;
b = inv(S'*inv(R)*S)*S'*inv(R)*d;

%implement equation 22 from paper
a1 = a(1);
a2 = a(2);
a3 = a(3);
b1 = b(1);
b2 = b(2);
b3 = b(3);
k0 = a1*b1 + a2*b2 + + a3*b3;
k1 = (a1*b1 + a2*b2 + a3*b3)^2;
k2 = (b1^2+b2^2+b3^2-1)*(a1^2+a2^2+a3^2);
num1 = sqrt(k0+k1-k2);
num2 = sqrt(k0-k1-k2);
denom = (b1^2+b2^2+b3^2-1);
Rs_hat1 = num1 / denom;
Rs_hat2 = num2 / denom;

%use result from eq 22 to solve eq 19 from paper
x_hat1 = a - b*Rs_hat1
x_hat2 = a - b*Rs_hat2



    


                