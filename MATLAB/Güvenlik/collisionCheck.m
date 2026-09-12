% Robotun verilen joint configuration'ında self-collision durumunu kontrol eder.
%
% Her robot linki iki joint merkezi arasındaki bir capsule olarak modellenir.
% Capsule'lar arasındaki surface distance hesaplanır ve tanımlanan minimum
% safety distance değerinden küçükse configuration unsafe kabul edilir.
%
% Birbirine doğrudan bağlı linkler collision pair olarak kontrol edilmez.

function [collisionSafe, selfCollisionSafe, minimumSelfDistance, closestPair] = ...
    collisionCheck(q, robotKinematics, collisionParameters)

q = q(:);

[jointFrame, T0Tool] = robotTransforms(q, robotKinematics);

jointCount = robotKinematics.jointCount;
linkCount = jointCount;

linkStart = zeros(3, linkCount);
linkEnd = zeros(3, linkCount);

% Joint frame origin'lerinden capsule başlangıç ve bitiş noktaları oluşturulur.
for linkIndex = 1:jointCount - 1
    linkStart(:, linkIndex) = jointFrame(1:3, 4, linkIndex);
    linkEnd(:, linkIndex) = jointFrame(1:3, 4, linkIndex + 1);
end

% Son link J6'dan Tool frame'e kadar uzanır.
linkStart(:, jointCount) = jointFrame(1:3, 4, jointCount);
linkEnd(:, jointCount) = T0Tool(1:3, 4);

linkRadius = collisionParameters.linkRadius(:);
collisionPairs = collisionParameters.selfCollisionPairs;
minimumDistance = collisionParameters.minimumDistance;

selfCollisionSafe = true;
minimumSelfDistance = inf;
closestPair = zeros(2, 1);

% Tanımlanan bütün non-adjacent link çiftleri kontrol edilir.
for pairIndex = 1:size(collisionPairs, 1)

    linkA = collisionPairs(pairIndex, 1);
    linkB = collisionPairs(pairIndex, 2);

    centerlineDistance = segmentSegmentDistance( ...
        linkStart(:, linkA), linkEnd(:, linkA), ...
        linkStart(:, linkB), linkEnd(:, linkB));

    surfaceDistance = centerlineDistance - linkRadius(linkA) - linkRadius(linkB);

    if surfaceDistance < minimumSelfDistance
        minimumSelfDistance = surfaceDistance;
        closestPair = [linkA; linkB];
    end

    if surfaceDistance <= minimumDistance
        selfCollisionSafe = false;
    end

end

collisionSafe = selfCollisionSafe;

end

function distance = segmentSegmentDistance(p1, q1, p2, q2)

% İki 3D line segment arasındaki minimum mesafeyi hesaplar.

d1 = q1 - p1;
d2 = q2 - p2;
r = p1 - p2;

a = dot(d1, d1);
e = dot(d2, d2);
f = dot(d2, r);

epsilon = 1e-12;

if a <= epsilon && e <= epsilon
    distance = norm(p1 - p2);
    return;
end

if a <= epsilon

    s = 0;
    t = min(max(f / e, 0), 1);

else

    c = dot(d1, r);

    if e <= epsilon

        t = 0;
        s = min(max(-c / a, 0), 1);

    else

        b = dot(d1, d2);
        denominator = a * e - b^2;

        if denominator ~= 0
            s = min(max((b * f - c * e) / denominator, 0), 1);
        else
            s = 0;
        end

        t = (b * s + f) / e;

        if t < 0
            t = 0;
            s = min(max(-c / a, 0), 1);
        elseif t > 1
            t = 1;
            s = min(max((b - c) / a, 0), 1);
        end

    end

end

closestPoint1 = p1 + d1 * s;
closestPoint2 = p2 + d2 * t;

distance = norm(closestPoint1 - closestPoint2);

end