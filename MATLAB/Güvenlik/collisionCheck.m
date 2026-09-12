% Robotun verilen joint configuration'ında self-collision ve environment
% collision durumunu kontrol eder.
%
% Her robot linki iki joint merkezi arasındaki bir capsule olarak modellenir.
% Ground plane ve environment box'ları ile minimum surface distance hesaplanır.
%
% Surface distance sıfır veya negatifse fiziksel collision vardır.
% Pozitif ancak tanımlanan safety clearance değerinden küçükse fiziksel
% collision olmamasına rağmen configuration unsafe kabul edilir.

function [collisionSafe, selfCollisionSafe, minimumSelfDistance, closestPair, physicalCollisionFree, clearanceSafe, ...
    environmentCollisionSafe, minimumEnvironmentDistance, closestEnvironmentLink, closestEnvironmentObject] = ...
    collisionCheck(q, robotKinematics, collisionParameters)

q = q(:);

[jointFrame, T0Tool] = robotTransforms(q, robotKinematics);

jointCount = robotKinematics.jointCount;

linkStart = zeros(3, jointCount);
linkEnd = zeros(3, jointCount);

for linkIndex = 1:jointCount - 1
    linkStart(:, linkIndex) = jointFrame(1:3, 4, linkIndex);
    linkEnd(:, linkIndex) = jointFrame(1:3, 4, linkIndex + 1);
end

linkStart(:, jointCount) = jointFrame(1:3, 4, jointCount);
linkEnd(:, jointCount) = T0Tool(1:3, 4);

linkRadius = collisionParameters.linkRadius(:);
collisionPairs = collisionParameters.selfCollisionPairs;
collisionClearance = collisionParameters.selfCollisionClearance(:);

selfPhysicalCollisionFree = true;
selfClearanceSafe = true;

minimumSelfDistance = inf;
closestPair = zeros(2, 1);

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

    if surfaceDistance <= 0
        selfPhysicalCollisionFree = false;
    end

    if surfaceDistance <= collisionClearance(pairIndex)
        selfClearanceSafe = false;
    end

end

selfCollisionSafe = selfPhysicalCollisionFree && selfClearanceSafe;

environmentCheckLinks = collisionParameters.environmentCheckLinks;

environmentPhysicalCollisionFree = true;
environmentClearanceSafe = true;

minimumEnvironmentDistance = inf;
closestEnvironmentLink = 0;
closestEnvironmentObject = 0;

groundHeight = collisionParameters.groundHeight;
groundClearance = collisionParameters.groundClearance;

for linkIndex = environmentCheckLinks

    minimumCenterHeight = min(linkStart(3, linkIndex), linkEnd(3, linkIndex));
    groundDistance = minimumCenterHeight - linkRadius(linkIndex) - groundHeight;

    if groundDistance < minimumEnvironmentDistance
        minimumEnvironmentDistance = groundDistance;
        closestEnvironmentLink = linkIndex;
        closestEnvironmentObject = 0;
    end

    if groundDistance <= 0
        environmentPhysicalCollisionFree = false;
    end

    if groundDistance <= groundClearance
        environmentClearanceSafe = false;
    end

end

environmentBoxCenter = collisionParameters.environmentBoxCenter;
environmentBoxSize = collisionParameters.environmentBoxSize;
environmentBoxClearance = collisionParameters.environmentBoxClearance(:);

for boxIndex = 1:size(environmentBoxCenter, 1)

    boxCenter = environmentBoxCenter(boxIndex, :).';
    boxSize = environmentBoxSize(boxIndex, :).';

    boxMinimum = boxCenter - boxSize / 2;
    boxMaximum = boxCenter + boxSize / 2;

    for linkIndex = environmentCheckLinks

        centerlineDistance = segmentAABBDistance( ...
            linkStart(:, linkIndex), linkEnd(:, linkIndex), boxMinimum, boxMaximum);

        boxDistance = centerlineDistance - linkRadius(linkIndex);

        if boxDistance < minimumEnvironmentDistance
            minimumEnvironmentDistance = boxDistance;
            closestEnvironmentLink = linkIndex;
            closestEnvironmentObject = boxIndex;
        end

        if boxDistance <= 0
            environmentPhysicalCollisionFree = false;
        end

        if boxDistance <= environmentBoxClearance(boxIndex)
            environmentClearanceSafe = false;
        end

    end

end

environmentCollisionSafe = environmentPhysicalCollisionFree && environmentClearanceSafe;

physicalCollisionFree = selfPhysicalCollisionFree && environmentPhysicalCollisionFree;
clearanceSafe = selfClearanceSafe && environmentClearanceSafe;

collisionSafe = selfCollisionSafe && environmentCollisionSafe;

end

function distance = segmentSegmentDistance(p1, q1, p2, q2)

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

        if abs(denominator) > epsilon
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

function distance = segmentAABBDistance(segmentStart, segmentEnd, boxMinimum, boxMaximum)

segmentDirection = segmentEnd - segmentStart;

breakpoints = zeros(8, 1);
breakpoints(1) = 0;
breakpoints(2) = 1;

breakpointIndex = 3;
epsilon = 1e-12;

for axisIndex = 1:3

    if abs(segmentDirection(axisIndex)) > epsilon

        tMinimum = (boxMinimum(axisIndex) - segmentStart(axisIndex)) / segmentDirection(axisIndex);
        tMaximum = (boxMaximum(axisIndex) - segmentStart(axisIndex)) / segmentDirection(axisIndex);

        breakpoints(breakpointIndex) = min(max(tMinimum, 0), 1);
        breakpoints(breakpointIndex + 1) = min(max(tMaximum, 0), 1);

    else

        breakpoints(breakpointIndex) = 0;
        breakpoints(breakpointIndex + 1) = 1;

    end

    breakpointIndex = breakpointIndex + 2;

end

breakpoints = sort(breakpoints);

minimumDistanceSquared = inf;

for intervalIndex = 1:7

    tLeft = breakpoints(intervalIndex);
    tRight = breakpoints(intervalIndex + 1);

    if tRight - tLeft <= epsilon
        continue;
    end

    tMiddle = (tLeft + tRight) / 2;
    middlePoint = segmentStart + segmentDirection * tMiddle;

    coefficientA = zeros(3, 1);
    coefficientB = zeros(3, 1);

    for axisIndex = 1:3

        if middlePoint(axisIndex) < boxMinimum(axisIndex)

            coefficientA(axisIndex) = -segmentDirection(axisIndex);
            coefficientB(axisIndex) = boxMinimum(axisIndex) - segmentStart(axisIndex);

        elseif middlePoint(axisIndex) > boxMaximum(axisIndex)

            coefficientA(axisIndex) = segmentDirection(axisIndex);
            coefficientB(axisIndex) = segmentStart(axisIndex) - boxMaximum(axisIndex);

        end

    end

    quadraticCoefficient = dot(coefficientA, coefficientA);

    minimumDistanceSquared = min(minimumDistanceSquared, ...
        pointAABBDistanceSquared(segmentStart + segmentDirection * tLeft, boxMinimum, boxMaximum));

    minimumDistanceSquared = min(minimumDistanceSquared, ...
        pointAABBDistanceSquared(segmentStart + segmentDirection * tRight, boxMinimum, boxMaximum));

    if quadraticCoefficient > epsilon

        stationaryPoint = -dot(coefficientA, coefficientB) / quadraticCoefficient;

        if stationaryPoint > tLeft && stationaryPoint < tRight

            minimumDistanceSquared = min(minimumDistanceSquared, ...
                pointAABBDistanceSquared(segmentStart + segmentDirection * stationaryPoint, boxMinimum, boxMaximum));

        end

    end

end

distance = sqrt(minimumDistanceSquared);

end

function distanceSquared = pointAABBDistanceSquared(point, boxMinimum, boxMaximum)

belowBox = max(boxMinimum - point, 0);
aboveBox = max(point - boxMaximum, 0);

offset = belowBox + aboveBox;

distanceSquared = dot(offset, offset);

end