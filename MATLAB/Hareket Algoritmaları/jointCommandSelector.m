% Joint Space çıktılarını 18x1 vektöre yerleştirmeden önce çağrılan
% helper function.

function [qDesired, qDotDesired, qDDotDesired] = ...
    jointCommandSelector(trajectoryCommand, jointIndex)

jointCount = numel(trajectoryCommand) / 3;

qDesired = trajectoryCommand(jointIndex);
qDotDesired = trajectoryCommand(jointCount + jointIndex);
qDDotDesired = trajectoryCommand(2 * jointCount + jointIndex);

end