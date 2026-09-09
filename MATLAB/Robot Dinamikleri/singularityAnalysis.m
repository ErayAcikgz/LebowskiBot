function [sigmaMin, conditionNumber, nearSingularity] = ...
    singularityAnalysis(J, singularityParameters)

characteristicLength = singularityParameters.characteristicLength;
sigmaThreshold = singularityParameters.sigmaThreshold;

JNormalized = J;

JNormalized(1:3, :) = ...
    JNormalized(1:3, :) / characteristicLength;

singularValues = svd(JNormalized);

sigmaMin = min(singularValues);
sigmaMax = max(singularValues);

if sigmaMin > 1e-12
    conditionNumber = sigmaMax / sigmaMin;
else
    conditionNumber = 1e12;
end

nearSingularity = sigmaMin < sigmaThreshold;

end