clc, clear;

%% Path

projectRoot = fileparts(mfilename("fullpath"));

robotKinematicsFolder = fullfile(projectRoot, "Robot Kinematiği");
motionAlgorithmsFolder = fullfile(projectRoot, "Hareket Algoritmaları");
controlFolder = fullfile(projectRoot, "Kontrol");
safetyFolder = fullfile(projectRoot, "Güvenlik");
testsFolder = fullfile(projectRoot, "Testler");
robotModelFolder = fullfile(projectRoot, "Robot Modeli");

projectFolders = {robotKinematicsFolder, motionAlgorithmsFolder, controlFolder, safetyFolder, testsFolder, robotModelFolder};

for folderIndex = 1:numel(projectFolders)
    if isfolder(projectFolders{folderIndex})
        addpath(projectFolders{folderIndex});
    end
end

%% Simülasyon Parametreleri

% Birimler: m, rad, rad/s, rad/s^2

simTime = 3.2224;

%% Robot Modeli

% Robot Geometrisi

link1Length = 0.35;
link1Width = 0.10;
link1Depth = 0.09;

link2Length = 0.40;
link2Width = 0.09;
link2Depth = 0.07;

link3Length = 0.40;
link3Width = 0.075;
link3Depth = 0.065;

link4Length = 0.12;
link4Radius = 0.050;

link5Length = 0.10;
link5Radius = 0.045;

toolLength = 0.12;

baseRadius = 0.14;
baseHeight = 0.10;

% Joint Merkez Offsetleri

j12Offset = [0.04 0 0.02];
j23Offset = [0.05 0 0];
j34Offset = [0.03 0 0.04];
j45Offset = [0 0.025 0];
j56Offset = [0 0 -0.025];
j6ToolOffset = [0 0 0.03];

% Joint Housing Geometrisi

joint1Radius = 0.11;
joint1Width = 0.10;

joint2Radius = 0.09;
joint2Width = 0.10;

joint3Radius = 0.08;
joint3Width = 0.09;

joint4Radius = 0.065;
joint4Width = 0.08;

joint5Radius = 0.060;
joint5Width = 0.07;

joint6Radius = 0.055;
joint6Width = 0.07;

jointRadius = [joint1Radius joint2Radius joint3Radius joint4Radius joint5Radius joint6Radius];
jointWidth = [joint1Width joint2Width joint3Width joint4Width joint5Width joint6Width];

% Tool Geometrisi

toolWidth = 0.10;
toolHeight = 0.07;

toolAdapterRadius = 0.045;
toolAdapterLength = 0.05;
toolPlateThickness = 0.04;

toolFrameOffset = j6ToolOffset + [0 0 toolAdapterLength + toolPlateThickness];

% Compatibility

armWidth = 0.08;
armDepth = 0.08;

wristWidth = 0.06;
wristDepth = 0.06;

%% Robot Kinematiği

% Kinematik Parametreler

jointCount = 6;

robotKinematics.jointCount = jointCount;
robotKinematics.jointType = [0 0 0 0 0 0];

robotKinematics.jointAxis = [
    0 0 1
    0 0 1
    0 0 1
    0 0 1
    0 0 1
    0 0 1
];

robotKinematics.jointFixedTranslation = [
    0 0 0
    [0 0 link1Length] + j12Offset
    [0 -link2Length 0] + j23Offset
    [0 -link3Length 0] + j34Offset
    [0 0 link4Length] + j45Offset
    [0 -link5Length 0] + j56Offset
];

robotKinematics.jointFixedRotationAxis = [
    1 0 0
    1 0 0
    1 0 0
    1 0 0
    1 0 0
    1 0 0
];

robotKinematics.jointFixedRotationAngle = deg2rad([0 -90 0 0 -90 90]);

robotKinematics.toolFixedTranslation = toolFrameOffset;
robotKinematics.toolFixedRotationAxis = [1 0 0];
robotKinematics.toolFixedRotationAngle = 0;

% Wrist Parametreleri

robotKinematics.j4ToWristCenter = [0 0 link4Length];
robotKinematics.wristCenterToTool = [0 0 link5Length + toolFrameOffset(3)];
robotKinematics.shoulderLateralOffset = j23Offset(3) + j34Offset(3) + link4Length;

% Joint Limitleri

robotKinematics.jointLimits = deg2rad([
    -170 170
    -120 120
    -140 140
    -180 180
    -120 120
    -360 360
]);

% Inverse Kinematics Parametreleri

robotKinematics.ik.geometryTolerance = 1e-9;
robotKinematics.ik.positionTolerance = 1e-6;
robotKinematics.ik.orientationTolerance = 1e-6;
robotKinematics.ik.wristSingularityTolerance = 1e-9;

% Singularity Parametreleri

singularityParameters.characteristicLength = link2Length + link3Length + link4Length + link5Length + norm(toolFrameOffset);
singularityParameters.sigmaThreshold = 0.05;

% Workspace Mapping Parametreleri

mappingParameters.sampleCount = 20000;
mappingParameters.randomSeed = 1;

%% Robot Konfigürasyonları

% Home Konfigürasyonu

homeConfiguration = deg2rad([0 -25 80 0 -35 0]);

%% Motion Parametreleri

% Motion Limit Parametreleri

motionLimits.speedScale = 1.0; % 1.0 -> 100%

motionLimits.jointVelocityMax = deg2rad([120 100 120 180 180 250]).';
motionLimits.jointAccelerationMax = deg2rad([200 180 200 300 300 400]).';

motionLimits.cartesianLinearVelocityMax = 0.5;
motionLimits.cartesianLinearAccelerationMax = 1.0;

motionLimits.cartesianAngularVelocityMax = deg2rad(90);
motionLimits.cartesianAngularAccelerationMax = deg2rad(180);

% Cartesian Joint Limit Parametreleri

motionParameters.cartesianJointLimitSampleCount = 101;
motionParameters.cartesianJointLimitMaxIterations = 3;
motionParameters.cartesianJointLimitTolerance = 1e-6;
motionParameters.cartesianJointLimitSafetyFactor = 1.02;

% Cartesian To Joint Derivative Parametreleri

motionParameters.damping = 1e-6;
motionParameters.jacobianDerivativeStep = 1e-6;

%% Güvenlik Parametreleri

% Joint Position Limit Parametreleri

safetyParameters.jointSoftLimitMargin = deg2rad([5 5 5 5 5 5]).';
safetyParameters.jointLimitTolerance = 1e-9;

% Collision Parametreleri

collisionParameters.trajectorySampleCount = 101;

collisionParameters.linkRadius = [
max(link1Width, link1Depth) / 2
max(link2Width, link2Depth) / 2
max(link3Width, link3Depth) / 2
link4Radius
link5Radius
max(toolWidth, toolHeight) / 2
];

collisionParameters.selfCollisionPairs = [
1 3
1 4
1 5
1 6
2 4
2 5
2 6
3 5
3 6
4 6
];

collisionParameters.selfCollisionClearance = [
0.020
0.020
0.020
0.020
0.020
0.020
0.020
0.020
0.020
0.002
];

collisionParameters.groundHeight = 0;
collisionParameters.groundClearance = 0.02;
collisionParameters.environmentCheckLinks = [2 3 4 5 6];

collisionParameters.environmentBoxCenter = [
    2.00 0 0.50
];

collisionParameters.environmentBoxSize = [
    0.30 0.30 1.00
];

collisionParameters.environmentBoxClearance = [
    0.020
    0.020
    0.020
];

%% Trajectory

% Başlangıç ve Hedef Açıları

qStart = deg2rad([0 -20 60 0 -30 0]).';
qTarget = deg2rad([-127.3010 -70.2617 -125.5892 -21.3085 -112.8297 -31.0801]).';

% 0: Cartesian, 1: Joint

chosenTrajectory = 0;

% Joint Trajectory

jointTrajectoryStartTime = 0;
jointTrajectoryEndTime = 0.5;

[jointTrajectorySafe, jointTrajectoryHardLimitSafe, jointTrajectorySoftLimitSafe] = ...
    validateJointTrajectory(qStart, qTarget, robotKinematics, safetyParameters);

% Cartesian Trajectory

cartesianTrajectoryStartTime = 0;
cartesianTrajectoryRequestedEndTime = 0.5;

[pStart, RStart, ~] = forwardKinematics(qStart, robotKinematics);
[pTarget, RTarget, ~] = forwardKinematics(qTarget, robotKinematics);

cartesianRequestedDuration = cartesianTrajectoryRequestedEndTime - cartesianTrajectoryStartTime;

[cartesianJointSafeDuration, cartesianPlannedMaxJointVelocity, cartesianPlannedMaxJointAcceleration, ...
    cartesianJointPathAvailable, cartesianJointHardLimitSafe, cartesianJointSoftLimitSafe] = ...
    cartesianJointLimitDuration(pStart, RStart, pTarget, RTarget, qStart, cartesianRequestedDuration, ...
    robotKinematics, motionLimits, motionParameters, safetyParameters);

cartesianTrajectoryEndTime = cartesianTrajectoryStartTime + cartesianJointSafeDuration;

cartesianTrajectorySafe = ...
    cartesianJointPathAvailable && cartesianJointHardLimitSafe && cartesianJointSoftLimitSafe;

% Collision Trajectory Validation

[jointTrajectoryCollisionSafe, jointSelfCollisionSafe, jointEnvironmentCollisionSafe, ...
    jointCollisionPathAvailable, jointMinimumSelfDistance, jointMinimumEnvironmentDistance, jointFirstUnsafeSample] = ...
    validateCollisionTrajectory(qStart, qTarget, pStart, RStart, pTarget, RTarget, 1, ...
    robotKinematics, collisionParameters);

[cartesianTrajectoryCollisionSafe, cartesianSelfCollisionSafe, cartesianEnvironmentCollisionSafe, ...
    cartesianCollisionPathAvailable, cartesianMinimumSelfDistance, cartesianMinimumEnvironmentDistance, cartesianFirstUnsafeSample] = ...
    validateCollisionTrajectory(qStart, qTarget, pStart, RStart, pTarget, RTarget, 0, ...
    robotKinematics, collisionParameters);

jointPlanningSafe = jointTrajectorySafe && jointTrajectoryCollisionSafe;

cartesianPlanningSafe = cartesianTrajectorySafe && cartesianTrajectoryCollisionSafe;7