function [qSelected, solutionAvailable, solutionIndex, ...
    qSolutions, validSolutions, solutionCount] = ...
    analyticalInverseKinematics( ...
        pDesired, RDesired, qCurrent, robotKinematics)

qSelected = zeros(6, 1);
solutionAvailable = false;
solutionIndex = 0;

qSolutions = nan(6, 8);
validSolutions = false(1, 8);
solutionCount = 0;

qCurrent = reshape(qCurrent, 6, 1);
pDesired = reshape(pDesired, 3, 1);

jointLimits = robotKinematics.jointLimits;

geometryTolerance = ...
    robotKinematics.ik.geometryTolerance;

wristTolerance = ...
    robotKinematics.ik.wristSingularityTolerance;

% Wrist Center

pWrist = pDesired - RDesired * robotKinematics.wristCenterToTool.';

xWrist = pWrist(1);
yWrist = pWrist(2);

lateralOffset = robotKinematics.shoulderLateralOffset;

rho = hypot(xWrist, yWrist);

if rho < abs(lateralOffset) - geometryTolerance
    return;
end

radialSquared = rho^2 - lateralOffset^2;

radialSquared = max(radialSquared, 0);

radialDistance = sqrt(radialSquared);

phi = atan2(yWrist, xWrist);

radialBranches = [
    radialDistance
    -radialDistance
];

% Planar Arm Geometry

shoulderTranslation = robotKinematics.jointFixedTranslation(2, :);

link2Vector = robotKinematics.jointFixedTranslation(3, 1:2);

joint3ToWrist = robotKinematics.jointFixedTranslation(4, :) + ...
    robotKinematics.j4ToWristCenter;

link3Vector = joint3ToWrist(1:2);

link2Magnitude = norm(link2Vector);
link3Magnitude = norm(link3Vector);

link2Angle = atan2(link2Vector(2), link2Vector(1));

link3Angle = atan2(link3Vector(2), link3Vector(1));

joint2FixedRotation = axisRotation( ...
    robotKinematics.jointFixedRotationAxis(2, :), ...
    robotKinematics.jointFixedRotationAngle(2));

toolFixedRotation = axisRotation( ...
    robotKinematics.toolFixedRotationAxis, ...
    robotKinematics.toolFixedRotationAngle);

RDesiredJ6 = RDesired * toolFixedRotation.';

% Shoulder Branches

for shoulderBranch = 1:2
    radialCurrent = radialBranches(shoulderBranch);

    q1 = phi - atan2(lateralOffset, radialCurrent);

    q1 = wrapAngle(q1);

    wristAfterJ1 = rotationZ(-q1) * pWrist;

    wristRelativeShoulder = wristAfterJ1 - shoulderTranslation.';

    wristJoint2Frame = joint2FixedRotation.' * wristRelativeShoulder;

    xPlanar = wristJoint2Frame(1);
    yPlanar = wristJoint2Frame(2);

    planarDistanceSquared = ...
        xPlanar^2 + yPlanar^2;

    cosElbow = ...
        (planarDistanceSquared - ...
        link2Magnitude^2 - ...
        link3Magnitude^2) / ...
        (2 * link2Magnitude * link3Magnitude);

    if cosElbow > 1 + geometryTolerance || ...
            cosElbow < -1 - geometryTolerance
        continue;
    end

    cosElbow = ...
        min(max(cosElbow, -1), 1);

    elbowMagnitude = acos(cosElbow);

    elbowBranches = [
        elbowMagnitude
        -elbowMagnitude
    ];

    % Elbow Branches

    for elbowBranch = 1:2
        relativeElbowAngle = ...
            elbowBranches(elbowBranch);

        effectiveShoulderAngle = ...
            atan2(yPlanar, xPlanar) - ...
            atan2( ...
                link3Magnitude * sin(relativeElbowAngle), ...
                link2Magnitude + ...
                link3Magnitude * cos(relativeElbowAngle));

        q2 = ...
            effectiveShoulderAngle - link2Angle;

        q3 = ...
            relativeElbowAngle - ...
            link3Angle + ...
            link2Angle;

        q2 = wrapAngle(q2);
        q3 = wrapAngle(q3);

        qPosition = [
            q1
            q2
            q3
            0
            0
            0
        ];

        [jointFrame, ~] = ...
            robotTransforms( ...
                qPosition, ...
                robotKinematics);

        RPreJ4 = ...
            jointFrame(1:3, 1:3, 4);

        RWrist = ...
            RPreJ4.' * RDesiredJ6;

        cosQ5 = ...
            min(max(RWrist(3, 3), -1), 1);

        sinQ5Magnitude = ...
            hypot( ...
                RWrist(1, 3), ...
                RWrist(2, 3));

        baseIndex = ...
            (shoulderBranch - 1) * 4 + ...
            (elbowBranch - 1) * 2;

        % Regular Wrist

        if sinQ5Magnitude > wristTolerance
            sinQ5Branches = [
                sinQ5Magnitude
                -sinQ5Magnitude
            ];

            for wristBranch = 1:2
                sinQ5 = ...
                    sinQ5Branches(wristBranch);

                q5 = ...
                    atan2(sinQ5, cosQ5);

                q4 = ...
                    atan2( ...
                        RWrist(2, 3) / sinQ5, ...
                        RWrist(1, 3) / sinQ5);

                q6 = ...
                    atan2( ...
                        RWrist(3, 2) / sinQ5, ...
                        -RWrist(3, 1) / sinQ5);

                qCandidate = [
                    q1
                    q2
                    q3
                    wrapAngle(q4)
                    wrapAngle(q5)
                    wrapAngle(q6)
                ];

                candidateIndex = ...
                    baseIndex + wristBranch;

                [qCandidate, withinLimits] = ...
                    fitConfigurationToLimits( ...
                        qCandidate, ...
                        jointLimits, ...
                        geometryTolerance);

                qSolutions(:, candidateIndex) = ...
                    qCandidate;

                if ~withinLimits
                    continue;
                end

                if validateSolution( ...
                        qCandidate, ...
                        pDesired, ...
                        RDesired, ...
                        robotKinematics)

                    validSolutions(candidateIndex) = true;

                    solutionCount = ...
                        solutionCount + 1;
                end
            end

        % Wrist Singularity

        else
            if cosQ5 >= 0
                q4 = 0;
                q5 = 0;

                q6 = ...
                    atan2( ...
                        RWrist(2, 1), ...
                        RWrist(1, 1));
            else
                q4 = 0;
                q5 = pi;

                q6 = ...
                    atan2( ...
                        RWrist(2, 1), ...
                        RWrist(2, 2));
            end

            qCandidate = [
                q1
                q2
                q3
                q4
                q5
                wrapAngle(q6)
            ];

            candidateIndex = ...
                baseIndex + 1;

            [qCandidate, withinLimits] = ...
                fitConfigurationToLimits( ...
                    qCandidate, ...
                    jointLimits, ...
                    geometryTolerance);

            qSolutions(:, candidateIndex) = ...
                qCandidate;

            if withinLimits && ...
                    validateSolution( ...
                        qCandidate, ...
                        pDesired, ...
                        RDesired, ...
                        robotKinematics)

                validSolutions(candidateIndex) = true;

                solutionCount = ...
                    solutionCount + 1;
            end
        end
    end
end

% Solution Selection

minimumDistance = inf;

for i = 1:8
    if ~validSolutions(i)
        continue;
    end

    qCandidate = qSolutions(:, i);

    jointDifference = ...
        atan2(sin(qCandidate - qCurrent), cos(qCandidate - qCurrent));

    configurationDistance = norm(jointDifference);                         

    if configurationDistance < minimumDistance
        minimumDistance = configurationDistance;

        qSelected = qCandidate;
        solutionIndex = i;
        solutionAvailable = true;
    end
end

end

function valid = validateSolution( ...
    q, pDesired, RDesired, robotKinematics)

[pCheck, RCheck, ~] = ...
    forwardKinematics(q, robotKinematics);

positionError = ...
    norm(pDesired - pCheck);

rotationDifference = ...
    RDesired.' * RCheck;

rotationCosine = ...
    (trace(rotationDifference) - 1) / 2;

rotationCosine = ...
    min(max(rotationCosine, -1), 1);

orientationError = ...
    acos(rotationCosine);

valid = ...
    positionError <= ...
        robotKinematics.ik.positionTolerance && ...
    orientationError <= ...
        robotKinematics.ik.orientationTolerance;

end

function [qAdjusted, valid] = ...
    fitConfigurationToLimits( ...
        q, jointLimits, tolerance)

qAdjusted = q;
valid = true;

for joint = 1:6
    qMin = jointLimits(joint, 1);
    qMax = jointLimits(joint, 2);

    angleFound = false;

    bestAngle = q(joint);
    bestDistance = inf;

    jointCenter = ...
        (qMin + qMax) / 2;

    for revolution = -2:2
        candidate = ...
            q(joint) + ...
            2 * pi * revolution;

        if candidate >= qMin - tolerance && ...
                candidate <= qMax + tolerance

            distance = ...
                abs(candidate - jointCenter);

            if distance < bestDistance
                bestAngle = candidate;
                bestDistance = distance;
                angleFound = true;
            end
        end
    end

    if angleFound
        qAdjusted(joint) = bestAngle;
    else
        qAdjusted(joint) = ...
            wrapAngle(q(joint));

        valid = false;
    end
end

end

function angle = wrapAngle(angle)

angle = ...
    atan2(sin(angle), cos(angle));

end

function R = rotationZ(theta)

c = cos(theta);
s = sin(theta);

R = [
    c -s 0
    s  c 0
    0  0 1
];

end

function R = axisRotation(axis, theta)

axis = axis(:) / norm(axis);

x = axis(1);
y = axis(2);
z = axis(3);

c = cos(theta);
s = sin(theta);
v = 1 - c;

R = [
    x*x*v + c,   x*y*v - z*s, x*z*v + y*s
    y*x*v + z*s, y*y*v + c,   y*z*v - x*s
    z*x*v - y*s, z*y*v + x*s, z*z*v + c
];

end