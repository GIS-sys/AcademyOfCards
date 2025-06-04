// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultProbability.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"

WalkingResultProbability::WalkingResultProbability(TSharedPtr<FJsonObject> data)
{
	for (auto& [name, inner_data] : data->Values) {
		auto inners = WalkingResultProbability::WalkingResultProbabilityInner{ .Name = name, .Probability = inner_data->AsObject()->GetStringField("probability")};
		for (auto& [x, y] : inner_data->AsObject()->GetObjectField("result")->Values) {
			inners.Results.Add(WalkingResult::FactoryCreate(x, y->AsObject()));
		}
		ProbabilityInners.Add(inners);
	}
}

WalkingResultProbability::~WalkingResultProbability()
{
}

void WalkingResultProbability::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model)
{
	// Calculate probabilities
	std::vector<float> probabilities;
	for (const auto& inner : ProbabilityInners) {
		probabilities.push_back(inner.Evaluate(player_model));
	}
	// Normalize them
	float sum = 0;
	for (float p : probabilities) sum += p;
	if (sum == 0) {
		for (int i = 0; i < probabilities.size(); ++i) probabilities[i] = 1.0 / probabilities.size();
	} else {
		for (int i = 0; i < probabilities.size(); ++i) probabilities[i] /= sum;
	}
	// Choose one depending on probability
	float random = rand() * 1.0 / RAND_MAX;
	int chosen_i = -1;
	while (random >= 0) {
		++chosen_i;
		random -= probabilities[chosen_i];
	}
	if (chosen_i >= ProbabilityInners.Num()) chosen_i = ProbabilityInners.Num();
	// Execute chosen results
	WalkingResultProbabilityInner ChosenInner = ProbabilityInners[chosen_i];
	walking_event->TextFromResult += "Random chosen: " + ChosenInner.Name + "\n";
	for (auto& result : ChosenInner.Results) {
		result->Execute(walking_event, player_model);
	}
}

float WalkingResultProbability::WalkingResultProbabilityInner::Evaluate(ABActorWalkingPlayerModel* player_model) const {
    FString Expression = Probability;

    // Substitute keywords with constants
    Expression.ReplaceInline(TEXT("light"), *FString::SanitizeFloat(player_model->PlayerStats.AlignmentLight));
    Expression.ReplaceInline(TEXT("dark"), *FString::SanitizeFloat(player_model->PlayerStats.AlignmentDark));
    Expression.ReplaceInline(TEXT("ice"), *FString::SanitizeFloat(player_model->PlayerStats.AlignmentIce));
    Expression.ReplaceInline(TEXT("fire"), *FString::SanitizeFloat(player_model->PlayerStats.AlignmentFire));

    // Remove all whitespace
    Expression.ReplaceInline(TEXT(" "), TEXT(""));
    Expression.ReplaceInline(TEXT("\t"), TEXT(""));
    Expression.ReplaceInline(TEXT("\n"), TEXT(""));

    // Simple recursive descent parser
    std::function<float(const FString& X, int& Pos)> ParseExpression;
    std::function<float(const FString& X, int& Pos)> ParseTerm;
    std::function<float(const FString& X, int& Pos)> ParseFactor;
    std::function<float(const FString& X, int& Pos)> ParseNumber;
    ParseExpression = [&ParseExpression, &ParseTerm, &ParseFactor, &ParseNumber](const FString& X, int& Pos) {
        // read smth until +-
        float Result = ParseTerm(X, Pos);
        while (Pos < X.Len()) {
            // read +- and then next smth
            TCHAR Op = X[Pos];
            if (Op != '+' && Op != '-') break;
            ++Pos;
            float Term = ParseTerm(X, Pos);
            Result = (Op == '+') ? (Result + Term) : (Result - Term);
        }
        return Result;
    };
    ParseTerm = [&ParseExpression, &ParseTerm, &ParseFactor, &ParseNumber](const FString& X, int& Pos) {
        // read smth until */
        float Result = ParseFactor(X, Pos);
        while (Pos < X.Len()) {
            // read */ and then next smth
            TCHAR Op = X[Pos];
            if (Op != '*' && Op != '/') break;
            ++Pos;
            float Factor = ParseFactor(X, Pos);
            if (Op == '/' && Factor == 0) {
                UE_LOG(LogTemp, Error, TEXT("WalkingResultProbabilityInner::Evaluate::ParseTerm: error in parsing '%s' pos=%d"), *X, Pos);
                return 0.0f;
            }
            Result = (Op == '*') ? (Result * Factor) : (Result / Factor);
        }
        return Result;
    };
    ParseFactor = [&ParseExpression, &ParseTerm, &ParseFactor, &ParseNumber](const FString& X, int& Pos) {
        // if parenthesis - parse again until closing bracket
        if (X[Pos] == '(') {
            ++Pos;
            float Result = ParseExpression(X, Pos);
            if (X[Pos] != ')') {
                UE_LOG(LogTemp, Error, TEXT("WalkingResultProbabilityInner::Evaluate::ParseFactor: error in parsing '%s' pos=%d"), *X, Pos);
                return 0.0f;
            }
            ++Pos;
            return Result;
        }
        // else just parse the number
        return ParseNumber(X, Pos);
    };
    ParseNumber = [&ParseExpression, &ParseTerm, &ParseFactor, &ParseNumber](const FString& X, int& Pos) {
        float Number = 0.0f;
        float power = 0.1;
        bool before_dot = true;
        while (Pos < X.Len() && (FChar::IsDigit(X[Pos]) || X[Pos] == '.')) {
            if (X[Pos] == '.') {
                if (!before_dot) {
                    UE_LOG(LogTemp, Error, TEXT("WalkingResultProbabilityInner::Evaluate::ParseNumber: error in parsing '%s' pos=%d"), *X, Pos);
                    return 0.0f;
                }
                before_dot = false;
            } else {
                int digit = (X[Pos] - '0');
                if (before_dot) {
                    Number = Number * 10 + digit;
                } else {
                    Number = Number + power * digit;
                    power *= 0.1;
                }
            }
            ++Pos;
        }
        return Number;
     };
    
    int Pos = 0;
    return ParseExpression(Expression, Pos);
}
