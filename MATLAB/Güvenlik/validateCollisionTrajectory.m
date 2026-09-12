% Joint Space veya Cartesian trajectory boyunca collision kontrolü yapar.
%
% Trajectory belirli sayda sample'a bölünür ve her sample için collisionCheck
% çarlr. Cartesian trajectory'de her pose Analytical IK ile joint
% configuration'a çevrilir ve IK branch continuity korunur.
%
% Herhangi bir sample unsafe ise trajectory collision açsndan unsafe kabul edilir.

function [trajectoryCollisionSafe, selfCollisionTrajectorySafe, environmentCollisionTrajectorySafe, ...
    collisionPathAvailable, minimumTrajectorySelfDistance, minimumTrajectoryEnvironmentDistance, firstUnsafeSample] = ...
    validateCollisionTrajectory(qStart, qTarget, pStart, RStart, pTarget, RTarget, chosenTrajectory, ...
    robotKinematics, collisionParameters)

qStart = qStart(:);
qTarget = qTarget(:);
pStart = reshape(pStart, 3, 1);
pTarget = reshape(pTarget, 3, 1);

sampleCount = collisionParameters.trajectorySampleCount;

trajectoryCollisionSafe = true;
selfCollisionTrajectorySafe = true;
environmentCollisionTrajectorySafe = true;
collisionPathAvailable = true;

minimumTrajectorySelfDistance = inf;
minimumTrajectoryEnvironmentDistance = inf;
firstUnsafeSample = 0;

qCurrent = qStart;

for sampleIndex = 1:sampleCount

    normalizedTime = (sampleIndex - 1) / (sampleCount - 1);
    positionScale = 10 * normalizedTime^3 - 15 * normalizedTime^4 + 6 * normalizedTime^5;

    if chosenTrajectory >= 1

        qSample = qStart + (qTarget - qStart) * positionScale;

    else

        pSample = pStart + (pTarget - pStart) * positionScale;
        RSample = interpolateRotation(RStart, RTarget, positionScale);

        [qSample, ikAvailable] = analyticalInverseKinematics(pSample, RSample, qCurrent, robotKinematics);

        if ~ikAvailable
            collisionPathAvailable = false;
            trajectoryCollisionSafe = false;
            firstUnsafeSample = sampleIndex;
            return;
        end

        qCurrent = qSample;

    end

    [collisionSafe, selfCollisionSafe, minimumSelfDistance, ~, ~, ~, ...
        environmentCollisionSafe, minimumEnvironmentDistance] = ...
        collisionCheck(qSample, robotKinematics, collisionParameters);

    minimumTrajectorySelfDistance = min(minimumTrajectorySelfDistance, minimumSelfDistance);
    minimumTrajectoryEnvironmentDistance = min(minimumTrajectoryEnvironmentDistance, minimumEnvironmentDistance);

    selfCollisionTrajectorySafe = selfCollisionTrajectorySafe && selfCollisionSafe;
    environmentCollisionTrajectorySafe = environmentCollisionTrajectorySafe && environmentCollisionSafe;

    if ~collisionSafe && firstUnsafeSample == 0
        firstUnsafeSample = sampleIndex;
    end

end

trajectoryCollisionSafe = collisionPathAvailable && selfCollisionTrajectorySafe && environmentCollisionTrajectorySafe;

end

function R = interpolateRotation(RStart, RTarget, interpolationScale)

qStart = rotationMatrixToQuaternion(RStart);
qTarget = rotationMatrixToQuaternion(RTarget);

quaternionDot = dot(qStart, qTarget);

if quaternionDot < 0
    qTarget = -qTarget;
    quaternionDot = -quaternionDot;
end

quaternionDot = min(max(quaternionDot, -1), 1);

if quaternionDot > 0.9995

    q = qStart + interpolationScale * (qTarget - qStart);
    q = q / norm(q);

else

    theta = acos(quaternionDot);
    sinTheta = sin(theta);

    q = sin((1 - interpolationScale) * theta) / sinTheta * qStart + ...
        sin(interpolationScale * theta) / sinTheta * qTarget;

end

R = quaternionToRotationMatrix(q);

end

function q = rotationMatrixToQuaternion(R)

traceR = trace(R);

if traceR > 0

    scale = 2 * sqrt(traceR + 1);
    qw = 0.25 * scale;
    qx = (R(3, 2) - R(2, 3)) / scale;
    qy = (R(1, 3) - R(3, 1)) / scale;
    qz = (R(2, 1) - R(1, 2)) / scale;

elseif R(1, 1) > R(2, 2) && R(1, 1) > R(3, 3)

    scale = 2 * sqrt(1 + R(1, 1) - R(2, 2) - R(3, 3));
    qw = (R(3, 2) - R(2, 3)) / scale;
    qx = 0.25 * scale;
    qy = (R(1, 2) + R(2, 1)) / scale;
    qz = (R(1, 3) + R(3, 1)) / scale;

elseif R(2, 2) > R(3, 3)

    scale = 2 * sqrt(1 + R(2, 2) - R(1, 1) - R(3, 3));
    qw = (R(1, 3) - R(3, 1)) / scale;
    qx = (R(1, 2) + R(2, 1)) / scale;
    qy = 0.25 * scale;
    qz = (R(2, 3) + R(3, 2)) / scale;

else

    scale = 2 * sqrt(1 + R(3, 3) - R(1, 1) - R(2, 2));
    qw = (R(2, 1) - R(1, 2)) / scale;
    qx = (R(1, 3) + R(3, 1)) / scale;
    qy = (R(2, 3) + R(3, 2)) / scale;
    qz = 0.25 * scale;

end

q = [qw; qx; qy; qz];
q = q / norm(q);

end

function R = quaternionToRotationMatrix(q)

q = q / norm(q);

w = q(1);
x = q(2);
y = q(3);
z = q(4);

R = [
    1 - 2 * (y^2 + z^2), 2 * (x * y - z * w), 2 * (x * z + y * w)
    2 * (x * y + z * w), 1 - 2 * (x^2 + z^2), 2 * (y * z - x * w)
    2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x^2 + y^2)
];

end