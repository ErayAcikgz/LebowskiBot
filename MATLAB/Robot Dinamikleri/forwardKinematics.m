% Simscape Robot Kinematik Modeli içindeki "Transform Sensor" Tool konum
% ve rotasyonunu Simscape modelini çalıştırarak verir. ROS2 kullanımı 
% için Simscape modelini çalıştıramayız. Bu fonksiyon robot Tool konum ve
% rotasyonunu matematiksel olarak hesaplar.

function [p, R, T0Tool] = forwardKinematics(q, robotKinematics)

[~, T0Tool] = robotTransforms(q, robotKinematics);

p = T0Tool(1:3, 4);
R = T0Tool(1:3, 1:3);

end