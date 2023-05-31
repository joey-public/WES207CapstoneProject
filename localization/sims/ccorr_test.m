s1 = [235+1i,47+2i,-87+3i,987+4i,1+5i,5+6i];
s2 = [2+7i,3+8i,4+9i,5,6+99i,7+2i];

[cxc,cidx] = ccorr(s1,s2,length(s1));
[~, maxidxc] = max(cxc);
maxidxc_corrected = cidx(maxidxc);

[xxc,xidx] = xcorr(s1,s2);
[~, maxidxx] = max(xxc);
maxidxx_corrected = xidx(maxidxx);

figure(1)
plot(abs(cxc))

figure(2)
plot(fftshift(abs(cxc)))
hold on
plot(abs(xxc))
hold off
