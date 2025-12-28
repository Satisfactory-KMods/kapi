#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogKApi, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogKApiCDOHelper, Log, All);

class FKAPIModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;

	virtual bool IsGameModule() const override { return true; }
};