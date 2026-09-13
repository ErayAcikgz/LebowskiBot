function tauCommand = jointServoController(trajectoryCommand, qActual, qDotActual, Kp, Kd)

jointCount = numel(qActual);

qDesired = trajectoryCommand(1:jointCount);
qDotDesired = trajectoryCommand(jointCount+1:2*jointCount);

positionError = qDesired - qActual;
velocityError = qDotDesired - qDotActual;

tauCommand = Kp .* positionError + Kd .* velocityError;

end