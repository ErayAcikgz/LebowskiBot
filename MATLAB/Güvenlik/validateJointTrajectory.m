% Joint Space trajectory'nin başlangıç ve hedef configuration'larını
% joint position limitleri açısından kontrol eder.
%
% Quintic Joint Space trajectory başlangıç ile hedef arasında position
% overshoot oluşturmadığı için iki uç nokta soft/hard limit içerisindeyse
% trajectory'nin tamamı position açısından güvenlidir.

function [trajectorySafe, hardLimitSafe, softLimitSafe] = ...
    validateJointTrajectory(qStart, qTarget, robotKinematics, safetyParameters)

[startHardSafe, startSoftSafe] = checkJointLimits(qStart, robotKinematics, safetyParameters);
[targetHardSafe, targetSoftSafe] = checkJointLimits(qTarget, robotKinematics, safetyParameters);

hardLimitSafe = startHardSafe && targetHardSafe;
softLimitSafe = startSoftSafe && targetSoftSafe;

trajectorySafe = hardLimitSafe && softLimitSafe;

end