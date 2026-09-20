% Cartesian trajectory'nin IK ve Jacobian dönüşümünden sonra oluşturduğu
% joint position, velocity ve acceleration değerlerini kontrol eder.
%
% Cartesian path belirli sayıda noktada örneklenir. Her noktada IK ile
% joint configuration, Jacobian ile joint velocity ve acceleration bulunur.
%
% Joint position soft/hard limit ihlali varsa trajectory güvenli değildir.
% Position limit ihlali duration artırılarak çözülemeyeceği için trajectory
% doğrudan reject edilmek üzere unsafe olarak işaretlenir.
%
% Velocity veya acceleration limitleri aşılırsa trajectory süresi artırılır.
% Velocity yaklaşık 1/T, acceleration yaklaşık 1/T^2 ile ölçeklenir.

function [trajectoryDuration, maxJointVelocity, maxJointAcceleration, solutionAvailable, hardLimitSafe, softLimitSafe] = ...
    cartesianJointLimitDuration(pStart, RStart, pTarget, RTarget, qStart, requestedDuration, robotKinematics, motionLimits, motionParameters, safetyParameters)

pStart = reshape(pStart, 3, 1);
pTarget = reshape(pTarget, 3, 1);
qStart = qStart(:);

jointCount = robotKinematics.jointCount;

maxJointVelocity = zeros(jointCount, 1);
maxJointAcceleration = zeros(jointCount, 1);

solutionAvailable = true;
hardLimitSafe = true;
softLimitSafe = true;

planningLimits = motionLimits;
planningLimits.speedScale = 1;

trajectoryDuration = cartesianTrajectoryDuration(pStart, RStart, pTarget, RTarget, requestedDuration, planningLimits);

sampleCount = motionParameters.cartesianJointLimitSampleCount;
maxIterations = motionParameters.cartesianJointLimitMaxIterations;
tolerance = motionParameters.cartesianJointLimitTolerance;
safetyFactor = motionParameters.cartesianJointLimitSafetyFactor;

for iteration = 1:maxIterations

    qCurrent = qStart;

    maxJointVelocity(:) = 0;
    maxJointAcceleration(:) = 0;

    hardLimitSafe = true;
    softLimitSafe = true;

    for sampleIndex = 1:sampleCount

        normalizedTime = (sampleIndex - 1) / (sampleCount - 1);
        currentTime = normalizedTime * trajectoryDuration;

        [pDesired, RDesired, vDesired, omegaDesired, aDesired, alphaDesired] = ...
            cartesianSpaceTrajectory(pStart, RStart, pTarget, RTarget, 0, trajectoryDuration, currentTime, planningLimits);

        [qDesired, ikAvailable] = analyticalInverseKinematics(pDesired, RDesired, qCurrent, robotKinematics);

        if ~ikAvailable
            solutionAvailable = false;
            return;
        end

        [sampleHardSafe, sampleSoftSafe] = ...
            checkJointLimits(qDesired, robotKinematics, safetyParameters);

        hardLimitSafe = hardLimitSafe && sampleHardSafe;
        softLimitSafe = softLimitSafe && sampleSoftSafe;

        if ~hardLimitSafe || ~softLimitSafe
            return;
        end

        [qDotDesired, qDDotDesired] = ...
            cartesianToJointDerivatives(qDesired, vDesired, omegaDesired, aDesired, alphaDesired, robotKinematics, motionParameters);

        maxJointVelocity = max(maxJointVelocity, abs(qDotDesired));
        maxJointAcceleration = max(maxJointAcceleration, abs(qDDotDesired));

        qCurrent = qDesired;

    end

    velocityRatio = max(maxJointVelocity ./ motionLimits.jointVelocityMax(:));
    accelerationRatio = max(maxJointAcceleration ./ motionLimits.jointAccelerationMax(:));

    durationScale = max([1, velocityRatio, sqrt(accelerationRatio)]);

    if durationScale <= 1 + tolerance
        return;
    end

    trajectoryDuration = trajectoryDuration * durationScale * safetyFactor;

end

end