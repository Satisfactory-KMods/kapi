#include "DataAssets/KAPISugHatchingData.h"

bool FKAPISlugIncubation::Roll() const
{
	return FMath::FRandRange(0.0f, 100.0f) <= mProbability;
}

bool FKAPISlugIncubation::Valid() const
{
	return IsValid(mSlug) && mProductionCount > 0 && mProbability > 0.0f;
}

bool FKAPISlugFeeling::IsTempInRange(float Temp) const
{
	return UKismetMathLibrary::InRange_FloatFloat(Temp, mMinHeat, mMaxHeat);
}

bool FKAPISlugFeeling::IsHumidityInRange(float Humidity) const
{
	return UKismetMathLibrary::InRange_FloatFloat(Humidity, mMinHumidity, mMaxHumidity);
}

bool FKAPISlugFeeling::IsDayTimeValid(EKAPISlugTime Time) const
{
	if (mDayTime == EKAPISlugTime::NONE || Time == EKAPISlugTime::NONE)
	{
		return false;
	}
	if (mDayTime == EKAPISlugTime::Any || Time == EKAPISlugTime::Any)
	{
		return true;
	}

	return Time == mDayTime;
}

bool UKAPISugHatchingData::IncubationFluidRequired() const
{
	return bRequireFluid && IsValid(mFluidClass);
}

void UKAPISugHatchingData::GetComfortableSlugs(TArray<TSubclassOf<UFGItemDescriptor>>& OutSlugs) const
{
	OutSlugs.Append(mComfortableWith);
	OutSlugs.AddUnique(mSlug);
}

TArray<FKAPISlugIncubation> UKAPISugHatchingData::GetSlugIncubationsSortedByChance() const
{
	TArray<FKAPISlugIncubation> FilteredSlugs = mPossibleSlugs.FilterByPredicate(
		[](const FKAPISlugIncubation& Incubation)
		{
			return Incubation.Valid();
		});

	FilteredSlugs.Sort([](const FKAPISlugIncubation& A, const FKAPISlugIncubation& B)
	{
		return A.mProbability > B.mProbability;
	});

	return FilteredSlugs;
}

TArray<FItemAmount> UKAPISugHatchingData::GetSlugsForThisCycle() const
{
	TArray<FItemAmount> Result;

	TArray<FKAPISlugIncubation> SortedSlugs = GetSlugIncubationsSortedByChance();

	for (const FKAPISlugIncubation& Incubation : SortedSlugs)
	{
		if (Incubation.Roll())
		{
			Result.Add(FItemAmount(Incubation.mSlug, Incubation.mProductionCount));
		}
	}

	return Result;
}

const UKAPISugHatchingData* UKAPISugHatchingData::GetHighterSlugStatic(UKAPISugHatchingData* A,
                                                                       UKAPISugHatchingData* B)
{
	if (!IsValid(A) && !IsValid(B))
	{
		return nullptr;
	}
	if (!IsValid(A) && IsValid(B))
	{
		return B;
	}
	if (IsValid(A) && !IsValid(B))
	{
		return B;
	}

	return A->GetHighterSlug(B);
}

const UKAPISugHatchingData* UKAPISugHatchingData::GetHighterSlug(UKAPISugHatchingData* Other) const
{
	if (mSlugTier < Other->mSlugTier)
	{
		return Other;
	}
	return this;
}
