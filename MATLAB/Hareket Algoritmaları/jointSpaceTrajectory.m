function [qDesired, qDotDesired, qDDotDesired] = ...
    jointSpaceTrajectory(qStart, qTarget, startTime, endTime, currentTime)

qStart = qStart(:);
qTarget = qTarget(:);

jointCount = numel(qStart);

qDesired = zeros(jointCount, 1);
qDotDesired = zeros(jointCount, 1);
qDDotDesired = zeros(jointCount, 1);

duration = endTime - startTime;

if duration <= 0
    qDesired = qTarget;
    return;
end

if currentTime <= startTime
    qDesired = qStart;
    return;
end

if currentTime >= endTime
    qDesired = qTarget;
    return;
end

% Normalize edilmiş zaman: başlangıçta 0, bitişte 1 olur
s = (currentTime - startTime) / duration;

%% Quintic position scaling: pozisyonun zaman içindeki yumuşak geçiş oranı
% "Hareket süresinin yüzde kaçındayız?"
% Quintic scaling başlangıç ve bitişte position, velocity ve acceleration
% sınır şartlarını sağlar
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

%% Position scaling'in zamana göre birinci türevi; velocity profilini belirler
velocityScale = (30 * s^2 - 60 * s^3 + 30 * s^4) / duration;

% Position scaling'in zamana göre ikinci türevi; acceleration profilini belirler
accelerationScale = (60 * s - 180 * s^2 + 120 * s^3) / duration^2;

jointDifference = qTarget - qStart;

qDesired = qStart + jointDifference * positionScale;
qDotDesired = jointDifference * velocityScale;
qDDotDesired = jointDifference * accelerationScale;

end