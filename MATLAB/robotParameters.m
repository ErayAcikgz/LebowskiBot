clc, clear;
addpath(fullfile(fileparts(mfilename("fullpath")), "Robot Dinamikleri"));

% Birimler: m, rad, rad/s, rad/s^2

simTime = 10.0;

%% Robot Geometrisi

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

%% Kinematik Parametreler

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

robotKinematics.wristCenterToTool = ...
    [0 0 link5Length + toolFrameOffset(3)];

robotKinematics.shoulderLateralOffset = ...
    j23Offset(3) + j34Offset(3) + link4Length;

% Joint Limitleri

robotKinematics.jointLimits = deg2rad([
    -170 170
    -120 120
    -140 140
    -180 180
    -120 120
    -360 360
]);

%% Inverse Kinematics Parametreleri

robotKinematics.ik.geometryTolerance = 1e-9;
robotKinematics.ik.positionTolerance = 1e-6;
robotKinematics.ik.orientationTolerance = 1e-6;
robotKinematics.ik.wristSingularityTolerance = 1e-9;

%% Singularity Parametreleri

singularityParameters.characteristicLength = ...
    link2Length + link3Length + link4Length + link5Length + norm(toolFrameOffset);

singularityParameters.sigmaThreshold = 0.05;

% Workspace Mapping Parametreleri

mappingParameters.sampleCount = 20000;
mappingParameters.randomSeed = 1;

%% Joint Test Hareketi

jointAmplitude = deg2rad([0 0 0 0 0 0]);
jointFrequency = [0.8 1.1 0.7 1.3 0.9 1.2];
jointPhase = [0 pi/6 pi/4 0 pi/3 pi/8];

qTest = deg2rad([10 -20 60 15 -30 25]);
jointBias = qTest;

qAmplitude = jointAmplitude;
qFrequency = jointFrequency;
qPhase = jointPhase;
qBias = jointBias;

qDotAmplitude = jointAmplitude .* jointFrequency;
qDotFrequency = jointFrequency;
qDotPhase = jointPhase + pi/2;
qDotBias = zeros(1, jointCount);

qDDotAmplitude = jointAmplitude .* jointFrequency.^2;
qDDotFrequency = jointFrequency;
qDDotPhase = jointPhase + pi;
qDDotBias = zeros(1, jointCount);

%% Home Konfigürasyonu

homeConfiguration = deg2rad([0 -25 80 0 -35 0]);

%% Compatibility

armWidth = 0.08;
armDepth = 0.08;

wristWidth = 0.06;
wristDepth = 0.06;

%% Trajectory

% Joint Trajectory

jointTrajectoryStartTime = 0;
jointTrajectoryEndTime = 5;

qStart = deg2rad([0 -20 60 0 -30 0]).';
qTarget = deg2rad([40 10 90 -30 20 45]).';

% Cartesian Trajectory

cartesianTrajectoryStartTime = 0;
cartesianTrajectoryEndTime = 5;

cartesianQStart = deg2rad([0 -20 60 0 -30 0]).';
cartesianQTarget = deg2rad([30 5 80 20 -40 35]).';

[pStart, RStart, ~] = ...
    forwardKinematics(cartesianQStart, robotKinematics);

[pTarget, RTarget, ~] = ...
    forwardKinematics(cartesianQTarget, robotKinematics);

% Motion Parametreleri

motionParameters.damping = 1e-6;
motionParameters.jacobianDerivativeStep = 1e-6;