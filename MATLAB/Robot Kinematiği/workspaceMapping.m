% Çalışma uzayındaki robotun çalışma noktalarını gösterir
% Ayrıca tekillik noktalarını, her noktanın tekillik riskini hesaplar

function workspaceMap = workspaceMapping()

robotParameters;

% Random: Noktaları rastgele üretir,
%   hızlıdır ancak bazı bölgelerde kümelenme veya boşluk oluşabilir.
% Sobol: Noktaları joint space'e daha homojen dağıtır,
%   aynı sample sayısında daha dengeli coverage sağlar.
samplingMethod = "sobol"; % "random" veya "sobol"

sampleCount = mappingParameters.sampleCount;
jointCount = 6;

qSamples = zeros(jointCount, sampleCount);
toolPositions = zeros(3, sampleCount);
sigmaMinValues = zeros(1, sampleCount);
conditionNumberValues = zeros(1, sampleCount);
nearSingularityValues = false(1, sampleCount);

unitSamples = generateSamples( ...
    sampleCount, ...
    jointCount, ...
    samplingMethod, ...
    mappingParameters.randomSeed);

for sample = 1:sampleCount
    q = zeros(jointCount, 1);

    for joint = 1:jointCount
        qMin = jointLimits(joint, 1);
        qMax = jointLimits(joint, 2);

        q(joint) = qMin + unitSamples(sample, joint) * (qMax - qMin);
    end

    [p, ~, ~] = forwardKinematics(q, robotKinematics);
    J = calculateJacobian(q, robotKinematics);

    [sigmaMin, conditionNumber, nearSingularity] = ...
        singularityAnalysis(J, singularityParameters);

    qSamples(:, sample) = q;
    toolPositions(:, sample) = p;
    sigmaMinValues(sample) = sigmaMin;
    conditionNumberValues(sample) = conditionNumber;
    nearSingularityValues(sample) = nearSingularity;
end

[minimumSigma, worstIndex] = min(sigmaMinValues);

workspaceMap.samplingMethod = samplingMethod;
workspaceMap.sampleCount = sampleCount;

workspaceMap.qSamples = qSamples;
workspaceMap.toolPositions = toolPositions;
workspaceMap.sigmaMinValues = sigmaMinValues;
workspaceMap.conditionNumberValues = conditionNumberValues;
workspaceMap.nearSingularityValues = nearSingularityValues;

workspaceMap.minimumSigma = minimumSigma;
workspaceMap.worstConfiguration = qSamples(:, worstIndex);
workspaceMap.worstToolPosition = toolPositions(:, worstIndex);

workspaceMap.nearSingularityCount = sum(nearSingularityValues);
workspaceMap.nearSingularityRatio = ...
    workspaceMap.nearSingularityCount / sampleCount;

assignin("base", "workspaceMap", workspaceMap);

plotWorkspaceMap(workspaceMap);

fprintf("\nWorkspace Mapping\n");
fprintf("Sampling Method: %s\n", samplingMethod);
fprintf("Sample Count: %d\n", sampleCount);
fprintf("Near Singularity Samples: %d\n", workspaceMap.nearSingularityCount);
fprintf("Near Singularity Ratio: %.2f %%\n", workspaceMap.nearSingularityRatio * 100);
fprintf("Minimum Sigma: %.6g\n", minimumSigma);

fprintf("Worst Configuration (deg):\n");
disp(rad2deg(workspaceMap.worstConfiguration).');

end

function samples = generateSamples(sampleCount, jointCount, samplingMethod, randomSeed)

switch lower(samplingMethod)
    case "random"
        rng(randomSeed);
        samples = rand(sampleCount, jointCount);

    case "sobol"
        sequence = sobolset(jointCount);
        sequence = scramble(sequence, "MatousekAffineOwen");
        samples = net(sequence, sampleCount);

    otherwise
        error('samplingMethod must be "random" or "sobol".');
end

end

function plotWorkspaceMap(workspaceMap)

p = workspaceMap.toolPositions;
sigma = workspaceMap.sigmaMinValues;
nearSingularity = workspaceMap.nearSingularityValues;

figure;

scatter3(p(1, :), p(2, :), p(3, :), 8, sigma, "filled");

axis equal;
grid on;

xlabel("X (m)");
ylabel("Y (m)");
zlabel("Z (m)");

title("Robot Workspace - Minimum Singular Value");
colorbar;

figure;

normalPoints = ~nearSingularity;
singularPoints = nearSingularity;

scatter3( ...
    p(1, normalPoints), ...
    p(2, normalPoints), ...
    p(3, normalPoints), ...
    6, ...
    "filled");

hold on;

scatter3( ...
    p(1, singularPoints), ...
    p(2, singularPoints), ...
    p(3, singularPoints), ...
    14, ...
    "filled");

axis equal;
grid on;

xlabel("X (m)");
ylabel("Y (m)");
zlabel("Z (m)");

title("Robot Workspace ve Singularity Map");
legend("Normal", "Near Singularity");

hold off;

end