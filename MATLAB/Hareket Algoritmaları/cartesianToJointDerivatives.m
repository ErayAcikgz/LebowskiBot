% Cartesian trajectory'den gelen lineer ve açısal hız/ivme hedeflerini,
% mevcut hedef joint konfigürasyonundaki Jacobian'ı kullanarak joint hız
% ve joint ivme hedeflerine dönüştürür.
%
% xDot = J * qDot
% qDot = J# * xDot
% xDDot = J * qDDot + JDot * qDot
% qDDot = J# * (xDDot - JDot * qDot)

function [qDotDesired, qDDotDesired] = ...
    cartesianToJointDerivatives( ...
        qDesired, ...
        vDesired, ...
        omegaDesired, ...
        aDesired, ...
        alphaDesired, ...
        robotKinematics, ...
        motionParameters)

qDesired = qDesired(:);

cartesianVelocity = [vDesired; omegaDesired];
cartesianAcceleration = [aDesired; alphaDesired];

J = calculateJacobian(qDesired, robotKinematics);

% Singularity yakınında J^-1 çok büyük değer üretebilir, bu yüzden DLS
qDotDesired = dampedLeastSquares(J, cartesianVelocity, ...
    motionParameters.damping);

predictionStep = motionParameters.jacobianDerivativeStep;

qPredicted = qDesired + qDotDesired * predictionStep;

JPredicted = calculateJacobian(qPredicted, robotKinematics);

JDot = (JPredicted - J) / predictionStep;

jointAccelerationTarget = cartesianAcceleration - JDot * qDotDesired;

qDDotDesired = dampedLeastSquares(J, jointAccelerationTarget, ...
    motionParameters.damping);

end

% Jacobian'ı güvenli şekilde terslemek için DLS (pseudo-inverse):
function solution = dampedLeastSquares(J, target, damping)

taskDimension = size(J, 1);

solution = J.' * ...
    ((J * J.' + damping^2 * eye(taskDimension)) \ target);

end