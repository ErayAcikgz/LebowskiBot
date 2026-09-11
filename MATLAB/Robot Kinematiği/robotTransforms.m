% Forward Kinematics ve Jacobian için helper function.
% Tekrar tekrar yazmamak için.

function [jointFrame, T0Tool] = robotTransforms(q, robotKinematics)

jointCount = robotKinematics.jointCount;

jointFrame = zeros(4, 4, jointCount);
T = eye(4);

for i = 1:jointCount
    fixedRotation = axisRotation( ...
        robotKinematics.jointFixedRotationAxis(i, :), ...
        robotKinematics.jointFixedRotationAngle(i));

    T = T * makeTransform( ...
        fixedRotation, ...
        robotKinematics.jointFixedTranslation(i, :));

    jointFrame(:, :, i) = T;

    if robotKinematics.jointType(i) == 0
        jointRotation = axisRotation( ...
            robotKinematics.jointAxis(i, :), ...
            q(i));

        jointMotion = makeTransform(jointRotation, [0 0 0]);
    else
        jointTranslation = robotKinematics.jointAxis(i, :) * q(i);
        jointMotion = makeTransform(eye(3), jointTranslation);
    end

    T = T * jointMotion;
end

toolRotation = axisRotation( ...
    robotKinematics.toolFixedRotationAxis, ...
    robotKinematics.toolFixedRotationAngle);

T0Tool = T * makeTransform( ...
    toolRotation, ...
    robotKinematics.toolFixedTranslation);

end

function T = makeTransform(R, p)

T = [R p(:); 0 0 0 1];

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