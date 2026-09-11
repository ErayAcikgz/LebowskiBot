% Quintic joint trajectory için velocity ve acceleration limitlerine göre
% gerekli minimum hareket süresini hesaplar.
%
% Quintic profile için maksimum normalize velocity katsayısı 1.875,
% maksimum normalize acceleration katsayısı ise 10/sqrt(3)'tür.
% Her joint için velocity ve acceleration kaynaklı minimum süre ayrı ayrı
% hesaplanır ve bütün joint'ler arasında en büyük değer seçilir.
%
% Kullanıcının istediği süre minimum güvenli süreden uzunsa değiştirilmez.
% speedScale ile trajectory süresi artırılarak hareketin genel hızı azaltılır.

function trajectoryDuration = jointTrajectoryDuration(qStart, qTarget, requestedDuration, motionLimits)

qStart = qStart(:);
qTarget = qTarget(:);

% Her joint'in gerçekleştirmesi gereken toplam açısal yer değiştirme.
jointDifference = abs(qTarget - qStart);

velocityMax = motionLimits.jointVelocityMax(:);
accelerationMax = motionLimits.jointAccelerationMax(:);

% Quintic profile'ın peak velocity değerinin jointVelocityMax limitini
% aşmaması için her joint'in ihtiyaç duyduğu minimum süre.
velocityDuration = 1.875 * jointDifference ./ velocityMax;

% Quintic profile'ın peak acceleration değerinin jointAccelerationMax
% limitini aşmaması için her joint'in ihtiyaç duyduğu minimum süre.
accelerationDuration = sqrt((10 / sqrt(3)) * jointDifference ./ accelerationMax);

% Bütün joint'lerin aynı anda trajectory'yi tamamlayabilmesi için en fazla
% süreye ihtiyaç duyan joint belirleyici olur.
minimumDuration = max([velocityDuration; accelerationDuration]);

% İstenen süre zaten güvenliyse korunur, değilse otomatik olarak uzatılır.
trajectoryDuration = max(requestedDuration, minimumDuration);

% speedScale 0-1 aralığında tutulur. Değer küçüldükçe aynı trajectory
% daha uzun sürede tamamlanır.
speedScale = min(max(motionLimits.speedScale, 1e-6), 1);
trajectoryDuration = trajectoryDuration / speedScale;

end