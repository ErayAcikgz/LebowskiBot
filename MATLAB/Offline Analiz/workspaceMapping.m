% Çalışma uzayındaki robotun çalışma noktalarını gösterir
% Ayrıca tekillik noktalarını, her noktanın tekillik riskini hesaplar

function [toolPositions, sigmaMinValues, nearSingularityValues] = ...
    workspaceMapping(robotKinematics, jointLimits, singularityParameters, mappingParameters)

sampleCount = mappingParameters.sampleCount;
jointCount = 6;

toolPositions = zeros(3, sampleCount);
sigmaMinValues = zeros(1, sampleCount);
nearSingularityValues = false(1, sampleCount);

rng(mappingParameters.randomSeed);

for sample = 1:sampleCount
    q = zeros(jointCount, 1);

    for joint = 1:jointCount
        qMin = jointLimits(joint, 1);
        qMax = jointLimits(joint, 2);

        q(joint) = qMin + rand * (qMax - qMin);
    end

    [p, ~, ~] = forwardKinematics(q, robotKinematics);

    J = calculateJacobian(q, robotKinematics);

    [sigmaMin, ~, nearSingularity] = ...
        singularityAnalysis(J, singularityParameters);

    toolPositions(:, sample) = p;
    sigmaMinValues(sample) = sigmaMin;
    nearSingularityValues(sample) = nearSingularity;
end

end