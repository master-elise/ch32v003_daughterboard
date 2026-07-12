% connect PD2 and PD3
% minicom: ctrl-a w and ctrl-a u ; ctrl-a l

load capture_CC2P1
load capture_CC2P0
subplot(211)
s=(diff(capture_CC2P0(:,3)));
k=find(s<0);
s(k)=s(k)+65536;
plot(s(100:400))
ylabel('CC2P=0')
subplot(212)
s=(diff(capture_CC2P1(:,3)));
k=find(s<0);
s(k)=s(k)+65536;
plot(s(100:400))
ylim([9990 10010])
xlabel('measurement number')
ylabel('CC2P=1')
