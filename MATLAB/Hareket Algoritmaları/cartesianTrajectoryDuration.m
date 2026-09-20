% Cartesian trajectory için linear ve angular velocity/acceleration
% limitlerine göre gerekli minimum hareket süresini hesaplar.
%
% Position hareketinde başlangıç ve hedef arasındaki doğrusal mesafe,
% orientation hareketinde ise başlangıçtan hedefe gereken toplam rotation
% angle kullanılır.
%
% Quintic profile için maksimum normalize velocity katsayısı 1.875,
% maksimum normalize acceleration katsayısı ise 10/sqrt(3)'tür.
%
% Linear ve angular hareket için gerekli süreler ayrı ayrı hesaplanır.
% Bütün limitleri aynı anda sağlayan en büyük süre trajectory süresi olarak
% seçilir. speedScale küçültüldüğünde trajectory süresi artırılır.

function trajectoryDuration = cartesianTrajectoryDuration(pStart, RStart, pTarget, RTarget, requestedDuration, motionLimits)

pStart = reshape(pStart, 3, 1);
pTarget = reshape(pTarget, 3, 1);

% Tool'un gerçekleştirmesi gereken toplam doğrusal mesafe.
positionDistance = norm(pTarget - pStart);

% Başlangıç orientation'ından hedef orientation'a gereken relative rotation.
RRelative = RStart.' * RTarget;

% Rotation matrix trace'i kullanılarak toplam rotation angle hesaplanır.
% Sayısal yuvarlama hatalarına karşı acos girişi [-1, 1] aralığında tutulur.
cosRotationAngle = (trace(RRelative) - 1) / 2;
cosRotationAngle = min(max(cosRotationAngle, -1), 1);
rotationAngle = acos(cosRotationAngle);

% Linear velocity ve acceleration limitlerinden gereken minimum süreler.
linearVelocityDuration = 1.875 * positionDistance / motionLimits.cartesianLinearVelocityMax;
linearAccelerationDuration = sqrt((10 / sqrt(3)) * positionDistance / motionLimits.cartesianLinearAccelerationMax);

% Angular velocity ve acceleration limitlerinden gereken minimum süreler.
angularVelocityDuration = 1.875 * rotationAngle / motionLimits.cartesianAngularVelocityMax;
angularAccelerationDuration = sqrt((10 / sqrt(3)) * rotationAngle / motionLimits.cartesianAngularAccelerationMax);

% Position ve orientation hareketlerinin bütün limitlerini sağlayan
% en büyük süre belirleyici olur.
minimumDuration = max([linearVelocityDuration, linearAccelerationDuration, angularVelocityDuration, angularAccelerationDuration]);

% Kullanıcının istediği süre güvenliyse korunur, değilse uzatılır.
trajectoryDuration = max(requestedDuration, minimumDuration);

% Global speedScale ile bütün Cartesian hareket aynı oranda yavaşlatılır.
speedScale = min(max(motionLimits.speedScale, 1e-6), 1);
trajectoryDuration = trajectoryDuration / speedScale;

end