% Position tarafında başlangıç ve hedef arasındaki konum farkı quintic
% time scaling ile interpolate edilir, buradan lineer hız ve ivme elde edilir.
%
% Orientation tarafında rotation matrix'ler doğrudan interpolate edilmez,
% çünkü ara değerler geçerli rotation matrix olmayabilir. Bu nedenle
% rotation matrix'ler quaternion'a çevrilir ve SLERP ile en kısa, düzgün
% orientation geçişi üretilir.
%
% Relative quaternion başlangıçtan hedefe gereken toplam dönüşü verir.
% Buradan rotation axis ve rotation angle çıkarılarak angular velocity
% ve angular acceleration hesaplanır.
%
% Helper functionlar:
% rotationMatrixToQuaternion  -> rotation matrix'i quaternion'a çevirir
% quaternionSlerp             -> iki orientation arasında düzgün interpolation yapar
% quaternionToRotationMatrix  -> quaternion'ı rotation matrix'e geri çevirir
% quaternionConjugate         -> unit quaternion'ın inverse'ini elde etmekte kullanılır
% quaternionMultiply          -> iki quaternion rotation'ını birleştirir

function [pDesired, RDesired, vDesired, omegaDesired, aDesired, alphaDesired] = ...
    cartesianSpaceTrajectory(pStart, RStart, pTarget, RTarget, startTime, endTime, currentTime)

pStart = reshape(pStart, 3, 1);
pTarget = reshape(pTarget, 3, 1);

pDesired = pStart;
RDesired = RStart;

vDesired = zeros(3, 1);
omegaDesired = zeros(3, 1);

aDesired = zeros(3, 1);
alphaDesired = zeros(3, 1);

duration = endTime - startTime;

if duration <= 0
    pDesired = pTarget;
    RDesired = RTarget;
    return;
end

if currentTime <= startTime
    return;
end

if currentTime >= endTime
    pDesired = pTarget;
    RDesired = RTarget;
    return;
end

% Joint Space ile aynı mantık
s = (currentTime - startTime) / duration;
positionScale = 10 * s^3 - 15 * s^4 + 6 * s^5;
velocityScale = (30 * s^2 - 60 * s^3 + 30 * s^4) / duration;
accelerationScale = (60 * s - 180 * s^2 + 120 * s^3) / duration^2;

positionDifference = pTarget - pStart;

pDesired = pStart + positionDifference * positionScale;
vDesired = positionDifference * velocityScale;
aDesired = positionDifference * accelerationScale;

qStart = rotationMatrixToQuaternion(RStart);
qTarget = rotationMatrixToQuaternion(RTarget);

if dot(qStart, qTarget) < 0
    qTarget = -qTarget;
end

qDesired = quaternionSlerp(qStart, qTarget, positionScale);
RDesired = quaternionToRotationMatrix(qDesired);

qRelative = quaternionMultiply(quaternionConjugate(qStart), qTarget);
qRelative = qRelative / norm(qRelative);

vectorMagnitude = norm(qRelative(2:4));

if vectorMagnitude > 1e-12
    rotationAxisLocal = qRelative(2:4) / vectorMagnitude;

    rotationAngle = 2 * atan2(vectorMagnitude, qRelative(1));

    rotationAxisWorld = RStart * rotationAxisLocal;

    omegaDesired = rotationAxisWorld * rotationAngle * velocityScale;
    alphaDesired = rotationAxisWorld * rotationAngle * accelerationScale;
end

end

function q = quaternionSlerp(qStart, qTarget, scale)

dotProduct = dot(qStart, qTarget);

if dotProduct < 0
    qTarget = -qTarget;
    dotProduct = -dotProduct;
end

dotProduct = min(max(dotProduct, -1), 1);

if dotProduct > 0.9995
    q = qStart + scale * (qTarget - qStart);
    q = q / norm(q);
    return;
end

theta = acos(dotProduct);
sinTheta = sin(theta);

startScale = sin((1 - scale) * theta) / sinTheta;
targetScale = sin(scale * theta) / sinTheta;

q = startScale * qStart + targetScale * qTarget;
q = q / norm(q);

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
    scale = 2 * sqrt(1 - R(1, 1) + R(2, 2) - R(3, 3));

    qw = (R(1, 3) - R(3, 1)) / scale;
    qx = (R(1, 2) + R(2, 1)) / scale;
    qy = 0.25 * scale;
    qz = (R(2, 3) + R(3, 2)) / scale;

else
    scale = 2 * sqrt(1 - R(1, 1) - R(2, 2) + R(3, 3));

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
    1 - 2 * (y^2 + z^2), 2 * (x*y - z*w), 2 * (x*z + y*w)
    2 * (x*y + z*w), 1 - 2 * (x^2 + z^2), 2 * (y*z - x*w)
    2 * (x*z - y*w), 2 * (y*z + x*w), 1 - 2 * (x^2 + y^2)
];

end

function qConjugate = quaternionConjugate(q)

qConjugate = [q(1); -q(2); -q(3); -q(4)];

end

function q = quaternionMultiply(q1, q2)

w1 = q1(1);
v1 = q1(2:4);

w2 = q2(1);
v2 = q2(2:4);

q = [
    w1 * w2 - dot(v1, v2)
    w1 * v2 + w2 * v1 + cross(v1, v2)
];

end