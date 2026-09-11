function [qDesired, qDotDesired, qDDotDesired] = ...
    jointSpaceTrajectory(qStart, qTarget, startTime, endTime, currentTime, motionLimits)

qStart = qStart(:);
qTarget = qTarget(:);

jointCount = numel(qStart);

qDesired = zeros(jointCount, 1);
qDotDesired = zeros(jointCount, 1);
qDDotDesired = zeros(jointCount, 1);

requestedDuration = endTime - startTime;

if requestedDuration <= 0
    qDesired = qTarget;
    return;
end

% İstenen trajectory süresini velocity, acceleration ve speedScale
% değerlerine göre gerekirse otomatik olarak uzatır.
duration = jointTrajectoryDuration(qStart, qTarget, requestedDuration, motionLimits);
effectiveEndTime = startTime + duration;

if currentTime <= startTime
    qDesired = qStart;
    return;
end

if currentTime >= effectiveEndTime
    qDesired = qTarget;
    return;
end

% Normalize edilmiş zaman: başlangıçta 0, bitişte 1 olur.
s = (currentTime - startTime) / duration;

% Quintic scaling başlangıç ve bitişte position, velocity ve acceleration
% sınır şartlarını sağlar:
% h(0) = 0, h(1) = 1
% h'(0) = 0, h'(1) = 0
% h''(0) = 0, h''(1) = 0

% Bu 6 sınır şartını sağlamak için 6 katsayılı 5. dereceden polinom kullanılır:
% h(s) = a0 + a1*s + a2*s^2 + a3*s^3 + a4*s^4 + a5*s^5
% Sınır şartları çözülünce:
% a0 = 0, a1 = 0, a2 = 0, a3 = 10, a4 = -15, a5 = 6
% ve sonuç olarak:
% h(s) = 10*s^3 - 15*s^4 + 6*s^5

positionScale = 10 * s^3 - 15 * s^4 + 6 * s^5;
velocityScale = (30 * s^2 - 60 * s^3 + 30 * s^4) / duration;
accelerationScale = (60 * s - 180 * s^2 + 120 * s^3) / duration^2;

jointDifference = qTarget - qStart;

qDesired = qStart + jointDifference * positionScale;
qDotDesired = jointDifference * velocityScale;
qDDotDesired = jointDifference * accelerationScale;

end