clc, clear;

% m, rad, rad/s, rad/s^2

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
j56Offset = [0.02 0 0];
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

%% Joint Test

jointCount = 6;

jointAmplitude = deg2rad([30 0 0 0 0 0]);
jointFrequency = [1 1 1 1 1 1];
jointPhase = [0 0 0 0 0 0];
jointBias = deg2rad([0 -25 80 0 -35 0]);

% Pozisyon
qAmplitude = jointAmplitude;
qFrequency = jointFrequency;
qPhase = jointPhase;
qBias = jointBias;

% Hız
qDotAmplitude = jointAmplitude .* jointFrequency;
qDotFrequency = jointFrequency;
qDotPhase = jointPhase + pi/2;
qDotBias = zeros(1, jointCount);

% İvme
qDDotAmplitude = jointAmplitude .* jointFrequency.^2;
qDDotFrequency = jointFrequency;
qDDotPhase = jointPhase + pi;
qDDotBias = zeros(1, jointCount);

%% Home Konfigürasyonu

homeConfiguration = deg2rad([0 -25 80 0 -35 0]);

%% Uyumluluk 

armWidth = 0.08;
armDepth = 0.08;

wristWidth = 0.06;
wristDepth = 0.06;