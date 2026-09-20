% Unsafe bir trajectory command'ın robota gönderilmesini engeller.
%
% Command güvenliyse trajectory değişmeden geçirilir.
% Güvenli değilse robot başlangıç configuration'ında tutulur ve joint
% velocity/acceleration komutları sıfırlanır.

function safeTrajectoryCommand = trajectorySafetyGate(trajectoryCommand, commandSafe, qHold)

qHold = qHold(:);

jointCount = numel(qHold);

safeTrajectoryCommand = zeros(3 * jointCount, 1);

if commandSafe
    safeTrajectoryCommand = trajectoryCommand;
else
    safeTrajectoryCommand(1:jointCount) = qHold;
end

end