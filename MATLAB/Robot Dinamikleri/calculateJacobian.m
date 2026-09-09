function J = calculateJacobian(q, robotKinematics)

[jointFrame, T0Tool] = robotTransforms(q, robotKinematics);

jointCount = robotKinematics.jointCount;
pTool = T0Tool(1:3, 4);

J = zeros(6, 6);

for i = 1:6
    pJoint = jointFrame(1:3, 4, i);
    RJoint = jointFrame(1:3, 1:3, i);

    axisWorld = RJoint * robotKinematics.jointAxis(i, :).';

    if robotKinematics.jointType(i) == 0
        J(1:3, i) = cross(axisWorld, pTool - pJoint);
        J(4:6, i) = axisWorld;
    else
        J(1:3, i) = axisWorld;
        J(4:6, i) = [0; 0; 0];
    end
end

end