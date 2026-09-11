% Joint configuration'ın hard ve soft position limitleri içerisinde olup
% olmadığını kontrol eder.
%
% Hard limits robotun izin verilen fiziksel joint sınırlarını temsil eder.
% Soft limits ise hard limitlerden belirli bir safety margin kadar içeride
% oluşturulur.
%
% Fonksiyon her joint için hard/soft limit durumunu ve bütün robot için
% genel güvenlik sonucunu döndürür.

function [hardLimitSafe, softLimitSafe, jointHardLimitSafe, jointSoftLimitSafe, softLimits] = ...
    checkJointLimits(q, robotKinematics, safetyParameters)

q = q(:);

jointLimits = robotKinematics.jointLimits;
tolerance = safetyParameters.jointLimitTolerance;
softMargin = safetyParameters.jointSoftLimitMargin(:);

hardMinimum = jointLimits(:, 1);
hardMaximum = jointLimits(:, 2);

softMinimum = hardMinimum + softMargin;
softMaximum = hardMaximum - softMargin;

softLimits = [softMinimum softMaximum];

jointHardLimitSafe = q >= hardMinimum - tolerance & q <= hardMaximum + tolerance;
jointSoftLimitSafe = q >= softMinimum - tolerance & q <= softMaximum + tolerance;

hardLimitSafe = all(jointHardLimitSafe);
softLimitSafe = all(jointSoftLimitSafe);

end